from machine import Pin, PWM

class RGBLed:
    def __init__(self, R_pin, G_pin, B_pin):
        """
        Constructor de la clase RGBLed
        Recibe:
        R_pin: GPIO con PWM donde se conecta la señal R
        G_pin: GPIO con PWM donde se conecta la señal G
        B_pin: GPIO con PWM donde se conecta la señal B
        """
        self.pwmR = PWM(Pin(R_pin))
        self.pwmR.freq(100)
        self.pwmG = PWM(Pin(G_pin))
        self.pwmG.freq(100)
        self.pwmB = PWM(Pin(B_pin))
        self.pwmB.freq(100)
        
    def set_color(self,r, g, b):
        """
        Establece el color mediante la componente Roja(R), Verde(G) y Azul(B),
        cada componente tiene un rango de 0 a 255
        """
        self.pwmR.duty(r * 1023 // 255)
        self.pwmG.duty(g * 1023 // 255)
        self.pwmB.duty(b * 1023 // 255)
