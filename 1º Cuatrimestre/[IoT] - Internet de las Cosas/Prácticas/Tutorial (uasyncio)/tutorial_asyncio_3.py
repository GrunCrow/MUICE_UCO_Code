"""
Tutorial uasyncio (micropython)
Parte 3
-------
Múltiples Co-rutinas con rearme (control de excepciones)

fernando.leon@uco.es
University of Córdoba, Spain
2021
"""

import uasyncio
from random import randint
from time import sleep

#
async def task(id, n, period):
    assert n > 0
    for i in range(n):
        n = randint(0,100)
        if n > 80: assert False
        print(f'task {id} ({period} s) generates -> {n} ', '*'*(i+1))
        await uasyncio.sleep(period)
    print('task ',id, ' done!')
    return id


async def task_main():
    tasks = []
    for i in range(1,6):
        t = uasyncio.create_task(task(i, randint(1,5),randint(1,5)))
        tasks.append(t)
    print(await uasyncio.gather(*tasks))
    print('main done!')
        

# control de excepciones
def handle_exception(loop, context):
    print(context['message'],':', context['exception'])
    loop.stop()

# rearme del sistema:
while True:
    evt_loop = uasyncio.new_event_loop()
    evt_loop.set_exception_handler(handle_exception)
    evt_loop.create_task(task_main())
    evt_loop.run_forever()
    for i in range(3,0,-1):
        print("rearme en ",i)
        sleep(1)

        

        