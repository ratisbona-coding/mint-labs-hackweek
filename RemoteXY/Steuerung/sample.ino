/*
   -- New project --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.8 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.11.1 or later version;
     - for iOS 1.9.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "BMWRaceCar"

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 19 bytes
  { 255,2,0,0,0,12,0,16,31,1,5,0,9,17,30,30,2,26,31 };

// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t joystick_1_x; // from -100 to 100  
  int8_t joystick_1_y; // from -100 to 100  

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

// Definitionen für unser Auto
#include "ESP32Servo.h"
#define DIRECTION_PIN 12    // Servo für die Lenkung
#define SPEED_PIN 13        // Motor Treiber Pin ENB
#define MOTOR_1_PIN_1 32    // Motor Treiber Pin IN3
#define MOTOR_1_PIN_2 33    // Motor Treiber Pin IN4

Servo lenkungsServo;

/**
 * Funktion setzt die Parameter für Vorwärts / Rückwärts und Lenkung
 * 
 **/
void drive(int8_t x, int8_t y) {
  
  // === VORWÄRTS / RÜCKWÄRTS ===

  // y - Also "hoch" und "runter" im Joystick get von -100 bis +100 
  // > 0 vorwärts
  // < 0 rückwärts
  // 0 stop
  if (y > 0) {
    // Vorwärts
    digitalWrite(MOTOR_1_PIN_1, HIGH);
    digitalWrite(MOTOR_1_PIN_2, LOW);
  } else if (y < 0) {
    // Rückwärts
    digitalWrite(MOTOR_1_PIN_1, LOW);
    digitalWrite(MOTOR_1_PIN_2, HIGH);    
  } else {
    // Stop
    digitalWrite(MOTOR_1_PIN_1, LOW);
    digitalWrite(MOTOR_1_PIN_2, LOW);
    analogWrite(SPEED_PIN, 0); 
  }

  // === GESCHWINDIGKEIT ===

  // Der Motor kann eine Geschwindigkeit von 0 bis 255
  // Bis zu einem Wert von ca 50 pfeift der Motor nur und hat nicht genügend Kraft das Auto anzutreiben
  // Der Joystick liefert Werte von 0 bis 100 oder von 0 bis -100
  // Deswegen müssen wir die Geschwindigkeit umrechnen

  if (y != 0) {
    // + und - behandeln wir gleich, wir wollen nur einen Wert von 0 -100 - https://www.arduino.cc/reference/de/language/functions/math/abs/
    int geschwindigkeit_positiv = abs(y);
    int geschwindigkeit = map(geschwindigkeit_positiv, 0, 100, 100, 255);
    // - Wir 'mappen' den Wertebreeich 0 bis 100 auf einen Wertebereich von 50 bis 255 - https://www.arduino.cc/reference/de/language/functions/math/map/
    // - mit analogWrite schreiben geben wir den Wert dann am Pin aus
    analogWrite(SPEED_PIN, geschwindigkeit); 
  }
  // === LENKUNG ===

  // x - also Joystick links oder rechts müssen wir wieder mappen

  // wir mappen auf 10° bis 170° weil der Servo sonst am Gehäuse anschlägt
  int lenkung = map(x, -100, 100, 10, 170); 
  lenkungsServo.write(lenkung);
}

void setup() 
{
  RemoteXY_Init (); 
  Serial.begin(9600);

  // Pins am ESP32 Modul auf OUTPUT = Ausgang setzen
  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(SPEED_PIN, OUTPUT);

  // Ansteuerung des Servos für die Lenkung ist etwas komplizierter
  ESP32PWM::allocateTimer(0);
  lenkungsServo.setPeriodHertz(50);
  lenkungsServo.attach(DIRECTION_PIN, 500, 2400);

  // Lenkung auf Mittelstellung
  lenkungsServo.write(0);
  // Motor aus
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, LOW);
}

void loop() 
{ 
  RemoteXY_Handler ();

  drive(RemoteXY.joystick_1_x, RemoteXY.joystick_1_y);
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 


}