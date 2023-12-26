#include "DigiKeyboard.h"

void setup() {
  // put your setup code here, to run once:

 // Begining the Keyboard stream
//  Keyboard.begin();

  // this is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  DigiKeyboard.sendKeyStroke(0);


  // Wait 500ms
  DigiKeyboard.delay(500);
//  delay(500);

  DigiKeyboard.delay(3000);
//  delay(3000);

DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
//  Keyboard.press(KEY_LEFT_GUI);
//  Keyboard.press('r');
//  Keyboard.releaseAll();

   DigiKeyboard.delay(500);
//   delay(500);

   DigiKeyboard.print("notepad");
//  Keyboard.print("notepad");

   DigiKeyboard.delay(500);
//   delay(500);

  DigiKeyboard.sendKeyStroke(KEY_ENTER);
//  typeKey(KEY_RETURN);

   DigiKeyboard.delay(500);
// delay(750);

  DigiKeyboard.println("Hello Workd!!!");
//  Keyboard.print("Hello World!!!");
//  typeKey(KEY_RETURN);

  // Ending stream
//  Keyboard.end();

}

void loop() {
  // put your main code here, to run repeatedly:

}
