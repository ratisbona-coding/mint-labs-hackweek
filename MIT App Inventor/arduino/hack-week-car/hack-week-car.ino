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

// Wenn der Motor zu langsam ist, dann pfeift er nur
#define MIN_SPEED 20
#define MAX_SPEED 254

// Der Servo stößt sonst am Gehäuse an
#define MIN_STEER 10
#define MAX_STEER 170

// Define necessary global variables
Servo directionServo;
BluetoothSerial bluetooth;

boolean newData = false;
byte numChars = 5;
char receivedChars[5];
int speed = 0;
int steering = 0;

// Control direction of main motor
enum Direction { Forward, Reverse, Stop };

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
    case Stop:
      digitalWrite(MOTOR_PIN_1, LOW);
      digitalWrite(MOTOR_PIN_2, LOW);
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
      if (ndx < numChars - 1) {
        // Serial.println(rc);
        receivedChars[ndx] = rc;
        ndx++;        
      } else {
        // Serial.println("Too long");
        // String ist zu lang, wir ignorieren den Rest
      }    
    } else {
      // Serial.println("End Transmission");
      receivedChars[ndx++] = '\0';
      ndx = 0;
      newData = true;
    }
  }

//  if (newData) {
//    Serial.print("Got message: ");
//    Serial.println(receivedChars);
//  }
}

// Handle speed inputs
// TODO: Consider refactoring this and moving it to another place
//       in the code
void handleSpeed(int value) {
  if (value < 200) {
    setMainDirection(Forward);
    speed = map(value, 200, 100, MIN_SPEED, MAX_SPEED);
    Serial.print("Speed forward: ");
  } else if (value > 200) {
    setMainDirection(Reverse);
    speed = map(value, 200, 300, MIN_SPEED, MAX_SPEED);
    Serial.print("Speed reverse: ");
  } else {
    setMainDirection(Stop);
    Serial.println("Stop!");
    speed = 0;
  }
  Serial.println(speed);
  setSpeed(speed);
}

// Handle steering inputs
void handleSteering(int value) {
  steering = map(value, 100, 300, MIN_STEER, MAX_STEER);
  Serial.print("Steering: ");
  Serial.println(steering);
  directionServo.write(steering);
}

// Configure bluetooth and both motors
void setup() {
  bluetooth.begin(BLUETOOTH_NAME);
  Serial.begin(115200);
  Serial.println("Hack week car started");

  
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
    newData = false;
    
    String received = receivedChars;
    // Wir brauchen 3 stellige Nummern, damit wir hier genau 3 Stellen lesen können
    int value = received.substring(1,4).toInt();

    // Serial.println(received);
    // Serial.println(value);

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
  }
}
