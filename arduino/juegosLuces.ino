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

int rx_ = -1;
int cnt = 0;
int pri = 0;
void loop()  {
		int rx = digitalRead(pinIRReceiverOut);
		if (rx != rx_) {
				// If value changed, set output
				rx_ = rx;
				cnt = 0;
		}
		else {
				if(cnt++ > 10) {
						cnt = 10;
						analogWrite(ledPin, rx?100:0);
				}
		}
		if(pri++ > 50) {
				Serial.println(rx);
				pri = 0;
		}
		delay(5);
}
