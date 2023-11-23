import uasyncio
from time import sleep
import math

from math import pi

from joystick import Joystick
from rgbled import RGBLed

joystick = Joystick(33,25)
rgbled = RGBLed(23,22,21)

async def task_1(id):
    while True:
        x,y = joystick.read_xy()
        x = round(x)
        y = round(y)
        print(f'joystick in task 3 generates -> {x}, {y} ')
        if x == 0 and y == 0:
            rgbled.set_color(0,0,0)
        elif x == -1 and y == 0:
            rgbled.set_color(255,0,0)
        elif x == 0 and y == 1:
            rgbled.set_color(0,255,0)
        elif x == 1 and y == 0:
            rgbled.set_color(0,0,255)
        elif x == 0 and y == -1:
            rgbled.set_color(255,0,255)
        else:
            rgbled.set_color(255,255,255)
        await uasyncio.sleep(1)
        
        # print('task ',id, ' done!')
    return id

async def task_2(id):
    while True:
        x, y = joystick.read_xy()
        #x = round(x)
        #y = round(y)

        if x == 0 and y == 0:
            rgbled.set_color(0, 0, 0) # Turn off
        else:
            angle = math.atan2(y, x)
            angle = math.degrees(angle)  # Convertir a grados

            if 30 <= angle <= 150:
                rgbled.set_color(255, 0, 0)  # RED
            elif 150 < angle <= 270:
                rgbled.set_color(0, 0, 255)  # BLUE
            elif 270 < angle <= 360 or 0 <= angle < 30:
                rgbled.set_color(0, 255, 0)  # GREEN
            else:
                rgbled.set_color(0, 0, 0) # Turn off

        print(f'joystick in task {id} generates -> {x}, {y}, {angle} ')
        await uasyncio.sleep(1)

    return id

async def task(id):
    while True:
        x, y = joystick.read_xy()
        #x = round(x)
        # y = round(y)
        
        angle = -1

        if round(x) == 0 and round(y) == 0:
            rgbled.set_color(0, 0, 0) # Turn off
        else:
            angle = math.acos(x) * (-y/abs(y))
            # angle = math.degrees(angle)  # Convertir a grados

            if  -pi <= angle <= 2 * pi / 3 - pi:
                rgbled.set_color(255, 0, 0)  # BLUE
            elif 2 * pi / 3 - pi < angle <= 4 * pi / 3 - pi:
                rgbled.set_color(0, 0, 255)  # RED
            elif 4 * pi / 3 - pi < angle <=  pi:
                rgbled.set_color(0, 255, 0)  # GREEN
            else:
                rgbled.set_color(0, 0, 0) # Turn off

        print(f'joystick in task {id} generates -> {x}, {y}, {angle}')
        await uasyncio.sleep(0.1)

    return id




async def task_main():
    tasks = []
    #for i in range(10):
    '''
    t_1 = uasyncio.create_task(task_1(1))
    tasks.append(t_1)
    t_2 = uasyncio.create_task(task_2(2))
    tasks.append(t_2)
    '''
    t = uasyncio.create_task(task(1))
    tasks.append(t)
    print(await uasyncio.gather(*tasks))
    print('main done!')
        

evt_loop = uasyncio.new_event_loop()
evt_loop.create_task(task_main())
evt_loop.run_forever()



