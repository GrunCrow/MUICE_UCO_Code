"""
Tutorial uasyncio (micropython)
Parte 4
-------
Sincronización mediante eventos

fernando.leon@uco.es
University of Córdoba, Spain
2021
"""

import uasyncio
from random import randint
from time import sleep

#
async def task_trigger(threshold, event_trigger):
    while True:
        n = randint(0,100)
        if n >= threshold:            
            print(f'sensor -> {n} (TRIGGER ->)')
            event_trigger.set()
        else:
             print(f'sensor -> {n}')
        
        await uasyncio.sleep(1)

async def task_handler(event_trigger):
    while True:
        await event_trigger.wait()
        print('(-> HANDLER)')
        event_trigger.clear()
    
async def task_main():
    event_trigger = uasyncio.Event()
    event_trigger.clear()
    tasks = []
    tasks.append(uasyncio.create_task(task_trigger(50, event_trigger)))
    tasks.append(uasyncio.create_task(task_handler(event_trigger)))
    await uasyncio.gather(*tasks)

try:
    uasyncio.run(task_main())
finally:
    uasyncio.new_event_loop()