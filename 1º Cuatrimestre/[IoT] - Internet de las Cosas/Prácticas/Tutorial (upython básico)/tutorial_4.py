"""
Tutorial micropython
Parte 4
-------
Pulsación de botón mediante interrupción.
NOTA: Probar a pulsar el botón varias veces mientras se escucha el beep -> Encolamiento. 

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

# manejador de interrupción (IRQ = Interruption ReQuest)
def irq_button(pin):
    led.on()
    buzzer.beep(200,1)
    led.off()

# código secuencial
button.irq(trigger=Pin.IRQ_FALLING, handler=irq_button)

# NOP
while True:        
    sleep_ms(100)
    