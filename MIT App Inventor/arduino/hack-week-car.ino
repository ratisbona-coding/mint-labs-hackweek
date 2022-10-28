// Include necessary libraries
#include "BluetoothSerial.h"
#include "ESP32Servo.h"

// Set bluetooth name
#define BLUETOOTH_NAME "HACK WEEK CAR"

// Define necessary pins
#define DIRECTION_PIN 12
#define SPEED_PIN 13
#define MOTOR_PIN_1 32
#define MOTOR_PIN_2 33

// Define necessary global variables
Servo directionServo;
BluetoothSerial bluetooth;

boolean newData = false;
byte numChars = 5;
char receivedChars[5];

// Control direction of main motor
enum Direction { Forward, Reverse };

void setMainDirection(Direction direction) {

  switch (direction) {
    case Forward:
      digitalWrite(MOTOR_PIN_1, HIGH);
      digitalWrite(MOTOR_PIN_2, LOW);
      break;
    case Reverse:
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, HIGH);
      break;
    default:
      break; 
  }
}

// Control speed of main motor
void setSpeed(int speed) {
  analogWrite(SPEED_PIN, speed);
}

void getBluetoothData() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (bluetooth.available() && !newData) {
    rc = bluetooth.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
    
      if (ndx >= numChars) {
        ndx = numChars -1;
      }
    } else {
      receivedChars[ndx] = '\0';
      ndx = 0;
      newData = true;
    }
  }
}

// Handle speed inputs
// TODO: Consider refactoring this and moving it to another place
//       in the code
void handleSpeed(int value) {
  if (value > 128) {
    setMainDirection(Forward);
    setSpeed(map(value, 128, 255, 0, 255));
  }

  if (value <= 128) {
    setMainDirection(Reverse);
    setSpeed(map(value, 0, 128, 0, 255));
  }
}

// Handle steering inputs
void handleSteering(int value) {
  directionServo.write(value);
}

// Configure bluetooth and both motors
void setup() {
  bluetooth.begin(BLUETOOTH_NAME);
  Serial.begin(9600);
  
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(SPEED_PIN, OUTPUT);

  ESP32PWM::allocateTimer(0);
  directionServo.setPeriodHertz(50);
  directionServo.attach(DIRECTION_PIN, 500, 2400);
}

// This code runs in an infinite loop
void loop() {
  getBluetoothData();
  
  if (newData) {
    
    String received = receivedChars;
    int value = received.substring(1,3).toInt();

    switch (receivedChars[0]) {
      case 'V':
        handleSpeed(value);
        break;
      case 'S':
        handleSteering(value);
        break;
      default:
        break;
    }
    
    newData = false;
  }
}
