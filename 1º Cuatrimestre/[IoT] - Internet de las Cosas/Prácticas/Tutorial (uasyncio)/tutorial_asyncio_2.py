"""
Tutorial uasyncio (micropython)
Parte 2
-------
Múltiples Co-rutinas simultáneas

fernando.leon@uco.es
University of Córdoba, Spain
2021
"""

import uasyncio
from random import randint
from time import sleep

#
async def task(id, n, period):
    assert n > 1
    for i in range(n):
        n = randint(0,100)
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
        

evt_loop = uasyncio.new_event_loop()
evt_loop.create_task(task_main())
evt_loop.run_forever()



        

        