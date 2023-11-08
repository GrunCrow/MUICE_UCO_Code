"""
Tutorial micropython
Parte 2
-------
Conexión con un dispositivo externo

BUZZER pinout:

- -> GND
+ -> GPIO4


fernando.leon@uco.es
University of Córdoba, Spain
2021
"""

# importación de librerías
from machine import Pin
from time import sleep
from buzzer import Buzzer

# variables globales
led = Pin(2, Pin.OUT)
buzzer = Buzzer(4)

# cambiando frecuencia
variable = 2

# código secuencial
while True:
    led.on()
    buzzer.beep(100 * variable,1) # frecuencia, continuo?
    led.off()
    sleep(1)
    variable *= 2

