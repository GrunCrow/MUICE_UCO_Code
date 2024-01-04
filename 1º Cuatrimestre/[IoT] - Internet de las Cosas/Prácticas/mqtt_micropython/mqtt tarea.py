# Alba Márquez Rodríguez

from mqtt_as import MQTTClient, config
import uasyncio as asyncio
from random import randint

from buzzer import Buzzer
from machine import Pin

buzzer = Buzzer(34)
button = Pin(13, Pin.IN, Pin.PULL_UP)

last_v = False

# Ts = 5
async def messages(client):
    async for topic, msg, retained in client.queue:
        print(f'[CAPA APLICACIÓN] Topic: "{topic.decode()}" Message: "{msg.decode()}" Retained: {retained}')
        if msg.decode() == '0':
            buzzer.beep(200,1)
            await asyncio.sleep(1)
        else:
            buzzer.beep_off()
            await asyncio.sleep(1)
            
        """
        global Ts
        if topic.decode() == 'T':
            Ts = int(msg.decode())
        elif topic.decode() == 'F':
            pass
            
        """

async def down(client):
    while True:
        await client.down.wait()  
        client.down.clear()
        print('[CAPA COMUNICACIÓN] Conexion MQTT cerrada')

async def up(client):
    while True:
        await client.up.wait()
        client.up.clear()
        print('[CAPA COMUNICACIÓN] Conexion MQTT establecida')
        
        # (re) suscripciones (tras evento de conexión o reconexión)
        for s in SUBS: 
            await client.subscribe(s, 1)

async def sensor(client):
    print('[CAPA PERCEPCIÓN] Sensor iniciado ...')
    while True:
        await asyncio.sleep(2)#Ts
        v = randint(0,100)
        print(f'[CAPA PERCEPCION] v=%d'%(v))
        await client.publish('grupo 3/N', '%d'%v, qos = 1)


async def button_function(client):
    # print('[CAPA PERCEPCIÓN] Sensor iniciado ...')
    value = 0
    
    while True:
        if button.value() != last_v:
            if button.value() == True:
                value = 1
            else:
                value = 0
            await client.publish('grupo 3\boton', '%d'%value, qos = 1)
            last_v = button.value()
        await uasyncio.sleep_ms(100)


async def buzzer_function(client):
    # print('[CAPA PERCEPCIÓN] Sensor iniciado ...')
    async for topic, msg, retained in client.queue:
    print(f'[CAPA APLICACIÓN] Topic: "{topic.decode()}" Message: "{msg.decode()}" Retained: {retained}')
    """
    global Ts
    if topic.decode() == 'T':
        Ts = int(msg.decode())
    elif topic.decode() == 'F':
        pass
        
    """
    
    if msg.decode() == '0': # pulsó el botón
        buzzer.beep(200,1)
        await uasyncio.sleep_ms(1)
    else:
        buzzer.beep_off()
        await uasyncio.sleep_ms(1)
                    
        # await client.publish('grupo 3/N', '%d'%v, qos = 1)        


async def main(client):
    try:
        print('[CAPA COMUNICACIÓN] Iniciando conexion...')
        await client.connect()
        print('[CAPA COMUNICACIÓN] Conexión establecida')
    except OSError:
        print('[CAPA COMUNICACIÓN] Conexión fallida')
        return
    for task in (up, down, messages):
        asyncio.create_task(task(client))
        
    # crear aquí las tareas que sean necesarias
    await button_function(client) 
    
if __name__ == '__main__':
    
    # configuración
    config['server'] = '192.168.2.3'
    config['ssid'] = 'IOTNET_2.4'
    config['wifi_pw'] = '10T@ATC_'
    config["user"]= ""
    config["password"]= ""
    config['keepalive'] = 120
    config["queue_len"]= 5
    config['will'] = ('topic_final', 'Mensaje de finalizacion', False, 0)

    # suscripciones
    SUBS = ("grupo 3/boton")

    # configuración y creación de la clase cliente
    MQTTClient.DEBUG = True
    client = MQTTClient(config)

    # ejecución de la rutina main
    try:
        asyncio.run(main(client))
    finally:
        client.close()
        asyncio.new_event_loop()

