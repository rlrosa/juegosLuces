estuve investigando un poco y logré complicarme un cacho.
me gustaría desentenderme de la parte de la alimentación x ahora, asumir q tenemos energía infinita (enchufe) y dsp ver como lidiar con el sol y sus compinches.
vamos x parte, jack-style.

# cerebro
con la IMU (Inertial Measurement Unit) q hay tirada en el lab de control de la facultad se puede implementar la lógica.
IMU: https://www.sparkfun.com/products/9184
se puede usar el gyro para medir la velocidad de giro, o sino el accelerometro para medir la fuerza centrifuga y traducir eso a velocidad de giro. el gyro creo q es mejor opcion, ya que no depende de las dimensiones del juego (la fuera centrifuga es funcion de la distancia al centro).

la IMU tiene al menos un pin de PWM expuesto. eso es una onda cuadrada. el período de la onda cuadrada se puede traducir a un nivel de intensidad de luz.
la señal es o prendida (1) o apagada (0), entonces un ejemplo sería algo tipo:
00000000000000000000000000000000 = nivel 0 (apagado)
...
11000000110000001100000011000000 = nivel 2 (luz NO visible)
11100000111000001110000011100000 = nivel 3 (luz visible)
...
11111111111111111111111111111111 = nivel 8 (luz visible - fuerte)
PWM: http://d32zx1or0t1x0y.cloudfront.net/2011/06/atmega168a_pwm_02_lrg.jpg

la movida es configurar el período del PWM en función de la velocidad de giro q detecta el gyro (chip q mide velocidad de giro en torno a su eje).
lógica:
  - ponemos la IMU en el medio del coso, le damos unos segundos para calibrarse (tomar la medida actual como velocidad de giro = 0).
  - cuando el coso gira (no importa la posición del a IMU) medimos giro distinto de 0 y con ese datos seteamos el PWM.

hasta acá creo q es razonablemente fácil. el cerebro sabe q está pasando y que es lo q quiere hacer. falta q pueda hacerlo.

# luz
para mi la posta sería una tira de leds http://lmgtfy.com/?q=led+strip
la cortamos en partes y recubrimos lo q se pueda (si el juego es grande quedan espacio libres, si es muy chico sobran tiras).
para el nivel de luz usar un dimmer q tome el PWM como entrada.
me estoy re limitando con la alimentacion. el consumo de los leds es bastante, no se vale enchufarlo a cualquier cosa (no puede comer de la IMU).
hay q hacer algo mas feliz.

acá explica un circuito para hacer un dimmer: http://www.circuitsgallery.com/2013/02/PWM-Led-Dimmer.html
el NE555 se consigue en Eneka, el resto tmb.
lo q no nos sirve es la parte de la alimentacion, necesitamos mas power.

