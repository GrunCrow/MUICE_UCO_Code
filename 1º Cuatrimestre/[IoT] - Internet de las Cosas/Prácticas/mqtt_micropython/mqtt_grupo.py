#Paola Montenegro
#David Cáceres Gómez


from mqtt_as import MQTTClient, config
import uasyncio as asyncio
from random import randint

from time import sleep
from machine import Pin
from buzzer import Buzzer

button = Pin(13, Pin.IN, Pin.PULL_UP)
buzzer = Buzzer(4)

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
        await client.publish('grupo1/N', '%d'%v, qos = 1)


async def boton(client):
    valor = 0;
    while True: 
        if button.value() == True:
            valor = 1;
            await client.publish('grupo1/boton', '%d'%valor, qos = 1)      
            await asyncio.sleep(1)
        else:
            valor = 0;
            await client.publish('grupo1/boton', '%d'%valor, qos = 1)
            await asyncio.sleep(1)


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
    #await sensor(client)
    await boton(client)
    
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
    SUBS = ('grupo2/boton', )

    # configuración y creación de la clase cliente
    MQTTClient.DEBUG = True
    client = MQTTClient(config)

    # ejecución de la rutina main
    try:
        asyncio.run(main(client))
    finally:
        client.close()
        asyncio.new_event_loop()

