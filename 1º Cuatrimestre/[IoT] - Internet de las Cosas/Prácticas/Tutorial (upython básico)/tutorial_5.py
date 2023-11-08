"""
Tutorial micropython
Parte 5
-------
Interrupción para activación/desactivación de beep.

NOTA: observar el error a causa del contexto de la variable "on"

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
on = False

# manejador de interrupción (IRQ = Interruption ReQuest)
def irq_button(pin):
    #global on
    if not on:
        led.on()
        buzzer.beep_on(200)
        on = True
    else:
        buzzer.beep_off()
        led.off()
        on = False

# código secuencial
button.irq(trigger=Pin.IRQ_FALLING, handler=irq_button)

# NOP
while True:        
    sleep_ms(100)