"""
Tutorial micropython
Parte 3
-------
Pulsación de botón mediante pooling (sondeo)

BUZZER pinout:

- -> GND
S -> GPIO4

BUTTON pinout:
- -> GND
S -> GPIO13


fernando.leon@uco.es
University of Córdoba, Spain
2021
"""

# importación de librerías
from machine import Pin
from time import sleep_ms
from buzzer import Buzzer

# variables globales
led = Pin(2, Pin.OUT)
buzzer = Buzzer(4)
button = Pin(13, Pin.IN, Pin.PULL_UP)


# código secuencial
while True:
    
    if button.value() == False:
        led.on()
        buzzer.beep(200,1)
    else:
        led.off()
        
    sleep_ms(100)
    