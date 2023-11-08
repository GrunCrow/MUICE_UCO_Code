from machine import ADC, Pin

class Joystick:
    def __init__(self, vrx_pin, vry_pin):
        """
        Constructor clase Joystick
        Recibe:
        vrx_pin: Puerto ADC donde se conecta la señal Vrx (expresado como GPIO)
        vry_pin: Puerto ADC donde se conecta la señal Vry (expresado como GPIO)
        """
        self.v_x = ADC(Pin(vrx_pin))
        self.v_x.atten(ADC.ATTN_11DB)
        self.v_y = ADC(Pin(vry_pin))
        self.v_y.atten(ADC.ATTN_11DB)
        
    def read_xy(self):
        """
        Lectura de punto cartesiano x,y normalizado en escala -1..1 en cada eje
        """
        fline = lambda x: x * 2 / 65535 -1
        
        x = self.v_x.read_u16() * 2 / 65535 -1
        y = self.v_y.read_u16() * 2 / 65535 -1
        
        return x, y
