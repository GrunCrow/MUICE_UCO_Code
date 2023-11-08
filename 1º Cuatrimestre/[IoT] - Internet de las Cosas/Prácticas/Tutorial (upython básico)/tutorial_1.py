"""
Tutorial micropython
Parte 1
-------
Ejecución del primer script en micropython


fernando.leon@uco.es
University of Córdoba, Spain
2021
"""


# importación de librerías
from machine import Pin
from time import sleep

# variable global
led = Pin(2, Pin.OUT)

# código secuencial
while True:
    led.on()
    sleep(1)
    led.off()
    sleep(1)




