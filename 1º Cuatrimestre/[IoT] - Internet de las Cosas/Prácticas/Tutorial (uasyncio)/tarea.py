"""

BUZZER pinout:

- -> GND
S -> GPIO4

"""


import uasyncio
from random import randint
from time import sleep
from buzzer import Buzzer
from machine import Pin

# variables globales
led = Pin(2, Pin.OUT)
buzzer = Buzzer(4)
button = Pin(13, Pin.IN, Pin.PULL_UP)
buzzer_on = False

"""
Tarea 1

Está el Beep on?
- Sí -> Apagar y wait 1 s
- No -> Encender y wait 1 s

"""
async def task_1(id):
    global buzzer_on
    for i in range(10):
        if buzzer_on:
            buzzer.beep_off()
            led.off()
            buzzer_on = False
        else:
            buzzer.beep_on(200)
            led.on()
            buzzer_on = True
        await uasyncio.sleep(1)
    return id
    # print('task ',1, ' done!')

"""
Tarea 2

Print random int
wait 5 s
"""
async def task_2(id):
    for i in range(10):
        n = randint(0,100)
        print(f'task 2 generates -> {n} ') 
        await uasyncio.sleep(5)
        # print('task ',id, ' done!')
    return id


async def task_main():
    tasks = []
    #for i in range(10):
    t_1 = uasyncio.create_task(task_1(1))
    tasks.append(t_1)
    t_2 = uasyncio.create_task(task_2(2))
    tasks.append(t_2)
    print(await uasyncio.gather(*tasks))
    print('main done!')
        

evt_loop = uasyncio.new_event_loop()
evt_loop.create_task(task_main())
evt_loop.run_forever()



        

        


