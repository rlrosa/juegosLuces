plan:
  - cuando pasa arturo se van prediendo las luces (es xa niños chicos, se prende cuando entra el pie y se apaga cuando sale el culo).

implementacion:
  - tira de "eslabon"="led + emisor IR + receptor IR".
  - controlador que genere señal PWM de ~40kHz para modular el IR.

problemas:
  - interferencia IR del sol, luces, etc
    -> solucion: modular IR 
  - circuito "largo". se puede romper si no lo hacemos prolijo. todo a lo largo de un costado van tres cables: tierra, vcc y PWM para modular.
    -> ?
  - alimentacion de los LED, supongo q 80 o mas, son 800mA.
    -> usar un buffer. probablemente lo mas fácil es usar un buffer cada X "eslabone"s. puede ser un CAT4101 o algo del estilo.

refs:
  - proximity arduino: http://www.youtube.com/watch?v=-uCG2RsV-MQ (email sent)
  - modular IR: http://arduinoguides.blogspot.com/2012/05/tsop-ir-receiver.html

