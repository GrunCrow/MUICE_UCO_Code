"""
Tutorial uasyncio (micropython)
Parte 1
-------
Co-rutinas y bucle de eventos

fernando.leon@uco.es
University of Córdoba, Spain
2021
"""

import uasyncio
from random import randint


def print_(i):
    print(i)

# una tarea simple
async def task_main():
    while True:
        print_('main task generates -> %d'%randint(0,100))
        await uasyncio.sleep(1)


# método 1
uasyncio.run(task_main())


"""
# método 2
evt_loop = uasyncio.get_event_loop()
task = evt_loop.create_task(task_main())
evt_loop.run_forever()
"""















        
    
    
