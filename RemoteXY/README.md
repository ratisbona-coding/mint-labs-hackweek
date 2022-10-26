# Bluetooth BLE Fernsteuerung mit RemoteXY

## Dokumentation Todo:

- [] Deutsch
- [] Bilder
- [] Hardware setup

## Hardware Setup

## Installation

### Arduino Studio 1.8.19

Hier herunterladen https://downloads.arduino.cc/arduino-1.8.19-windows.exe und installieren.

### ESP32 Board
Dann müssen wir die board library für das ESP32 board installieren:

* `File->Preferences->Additional Board Manager Urls`: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json eintragen
* `Tools->Board->Board Manager` suchen nach ESP32 und installieren

### RemoteXY Bibliothek
Dann müssen wir die RemoteXY Bibliothek installieren (https://remotexy.com/en/library/)

* `Sketch->Include Library->Manage Libraries` suche nach `RemoteXY` und dann installieren.

### ESPServo Bibliothek

* `Sketch->Include Library->Manage Libraries` suche nach `esp32Servo` und dann installieren (nicht esp32servo360).
![ESP Servo](doc/esp32servo.png "ESP Servo")

### RemoteXY App installieren

Die App gibt es hier https://remotexy.com/en/download/

Bis fünf GUI Elemente ist die APP kostenlos. Mehr als 5 kosten dann ca 8€.

Okay, cool. Soweit sind wir bereit, jetzt können wir starten.

## Steuerungsprojekt erstellen

### GUI mit RemoteXY erstellen

Hier ist der Editor: https://remotexy.com/en/editor/

Configuration: 
* Connection: Bluetooth
* Board: ESP32based board
* Module: Bluetooth on Board
* IDE: Arduino IDE

Dann ziehen wir einen Joystick in die GUI. Bitte den Haken bei `automatically center` anklicken. Er heißt dann `joystick_1`

Vergebt rechts noch einen Namen für euer Auto unter `Bluetooth name`

Jetzt können wir mit `Get Code` den Quellcode holen. Er wird unten dargestellt. Wir können im offenen Arduino Studio den Text einfach in eine neue Datei kopieren.

### Steuercode für unser Auto

Zuerst müssen wir ein paar Sachen definieren die wir später verwenden wollen:

Vorher
```C++
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////
```

Nachher
```C++
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
```

Danach müssen wir unsere die Methode `setup` anpassen, damit wir einen definierten Zustand beim Start des ESP32 haben und die Pins richtig konfiguriert sind.

Vorher
```C++
void setup() 
{
  RemoteXY_Init (); 
}
```

Nachher
```C++
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
```

In der Methode `loop` rufen wir eine Methode `drive` auf. Diese bekommt den aktuellen Wert vom Joystick und steuert dann den Motor und den Servo an.

Dafür definieren wir zuerst die Methode `drive` und fügen diese **vor** der Methode `setup` in unseren Code ein.

```C++
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

  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(geschwindigkeit);
  Serial.print(",");
  Serial.print(lenkung);
}
```

Der Aufruf für unsere `drive` Methode muss dann noch in die Methode `loop`

Vorher:
```C++
void loop() 
{ 
  RemoteXY_Handler ();  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 
}
```

Nachher:
```C++
void loop() 
{ 
  RemoteXY_Handler ();  
  drive(RemoteXY.joystick_1_x, RemoteXY.joystick_1_y);
}
```

Jetzt bitte kompilieren und auf den ESP übertragen.
Klick auf Button `Upload` und wenn eine Meldung `Connecting....` kommt, den Knopf rechts vom USB Anschluss auf dem ESP drücken, bis die Übertragung beginnt.

## Fahren!!!!

Auf dem Handy die App `RemoteXY` öffnen. Oben auf `+` und dann `Bluetooth BLE` aussuchen. Dann in der Liste das Auto nach dem Namen suchen den ihr vorher festgelegt habt.

Die Steuerung öffnet sich dann und es heißt. `Gentlegirls, please start your engines`

## Fehlersuche

* Auch ohne angeschlossenen Motor kann man den Serial Plotter `Tools->Serial Plotter` nutzen. Er zeigt X/Y, Geschwindigkeit und Lenkung grafisch dar.
* Sollte die Bluetooth Verbindung abbrechen und nicht mehr hergestellt werden können hilft das Bluetooth kurz abschalten und wieder einschalten. 