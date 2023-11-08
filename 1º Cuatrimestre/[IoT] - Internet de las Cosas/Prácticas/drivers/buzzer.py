from machine import Pin, PWM
from time import sleep


class Buzzer:    
    def __init__(self, pwm_pin):
        self.pin = Pin(pwm_pin, Pin.OUT)
        self.pwm = None
        
    def beep(self, freq, secs):
        self.pwm = PWM(self.pin, freq=freq, duty=512)
        sleep(secs)
        self.pwm.deinit()
        
    def beep_on(self,freq):
        self.pwm = PWM(self.pin, freq=freq, duty=512)
    
    def beep_off(self):
        assert self.pwm is not None
        self.pwm.deinit()