/*
bla.
 */

// Set up 38kHz IR TX on pin 3
#define IR_KHZ               38
#define TIMER_PWM_PIN        3
#define TIMER_ENABLE_PWM     (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
void enableIrTransmitters(int khz) {
		// Disable the timer 2 Interrupt (which is used for receiving IR)
		TIMER_DISABLE_INTR;

		pinMode(TIMER_PWM_PIN, OUTPUT);

		// When not sending PWM, we want it low
		digitalWrite(TIMER_PWM_PIN, LOW);

		// The modulation frequency is F_CPU / 2 / OCR2A (output compare register 2A)
		const uint8_t pwm_val = F_CPU / 2000 / IR_KHZ;

		TCCR2A = _BV(WGM20);
		TCCR2B = _BV(WGM22) | _BV(CS20);
		OCR2A  = pwm_val;
		OCR2B  = pwm_val / 3;

		// Enable PWM output on pin 3
		TIMER_ENABLE_PWM;
}

int ledPin = 9;           // LED connected to digital pin 9
// IR receiver
int pinIRReceiverOut = 7; // IR receiver - Output

void setup()  { 
		//// Setup IR receiver
		// Read RX output
		pinMode(pinIRReceiverOut, INPUT);

		//// Notification LED
		analogWrite(ledPin, 0);

		//// Setup IR emitter (@38kHz)
		enableIrTransmitters(IR_KHZ);

		//// Setup serial monitor
		Serial.begin(115200);

		delay(1000);
}

#define DEBUG            1

#define ON               255  // max led value when turned on
#define OFF              0    // min led value when turned off
#define STEP             5    // dimmer inc step size

// After FADE_OUT_SAMPLES at the setpoint, will turn the output off.
#define FADE_OUT         0    // enable(1)/disable(0)
#define FADE_OUT_SAMPLES 800  // samples before fading out

// Will wait for FILTER_SAMPLES stable successive samples before
// changing the output.
#define FILTER_SAMPLES 10

#define min(a,b)        (a<b?a:b)
#define max(a,b)        (a>b?a:b)
#define lim(a,low,high) min(max(a,low), high)
#define setpoint(s)     (s?ON:OFF)
#define steady(cnt)     (cnt==FADE_OUT_SAMPLES)
#define filter_ok(cnt)  (cnt==FILTER_SAMPLES)

int rx_ = -1;
int cnt_filter = 0;
int cnt_print  = 0;
int cnt_steady = 0;
int val = 0;
int out = 0;

void loop()  {
		int rx = digitalRead(pinIRReceiverOut);

		// Noise filter
		// Will change output only after FILTER_SAMPLES stable samples.
		if (rx != rx_) {
				// start counter again.
				cnt_filter = 0;
		}
		else {
				cnt_filter = min(cnt_filter++, FILTER_SAMPLES);
				if(filter_ok(cnt_filter)) {
						if(out != rx) {
								cnt_steady = 0;
						}
						out  = rx;
				}
		}

		// Dimm to setpoint
		int to_add = 0;
		if(val == (out?ON:OFF) || steady(cnt_steady)) {
				// setpoint has been reached
				cnt_steady = min(cnt_steady++, FADE_OUT_SAMPLES);
				if(FADE_OUT && steady(cnt_steady)) {
						// turn off led, nothing is happening.
						to_add = -STEP;
				}
		}
		else {
				// output is not yet at the setpoint
				to_add = out?STEP:-STEP;
				cnt_steady = 0;
		}
		val = lim(val+to_add, OFF, ON);
		analogWrite(ledPin, val);

		if(DEBUG && cnt_print++ > 50) {
				Serial.print("Rx:");Serial.print(rx);
				Serial.print("\tFilter:");Serial.print(filter_ok(cnt_filter));
				Serial.print("\tOut:");Serial.print(out);
				Serial.print("\tVal:");Serial.print(val);
				if(FADE_OUT) {
						Serial.print("\tSteady:");Serial.print(steady(cnt_steady));
						Serial.print("\tFadeOut:");Serial.print(FADE_OUT_SAMPLES - cnt_steady);
				}
				Serial.println("");
				cnt_print = 0;
		}

		// Update for next loop
		rx_ = rx;

		// No need to hurry.
		delay(5);
}
