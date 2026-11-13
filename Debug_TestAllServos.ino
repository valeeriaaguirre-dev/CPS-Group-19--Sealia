#include <Servo.h>

Servo servo2;
Servo servo3;
Servo servo4;
Servo servo9;
Servo servo10;
Servo servo11;

const int STEP_DELAY = 50;   // Increase for slower movement

void setup() {

  Serial.begin(9600);

  servo2.attach(2);
  servo3.attach(3);
  servo4.attach(4);

  servo9.attach(9);
  servo10.attach(10);
  servo11.attach(11);

  // Starting positions
  servo2.write(100);
  servo3.write(100);
  servo4.write(100);

  servo9.write(90);

  servo10.write(0);
  servo11.write(0);

  delay(2000);
}

void moveAllForward() {

  // progress goes from 0% to 100%
  for (int progress = 0; progress <= 100; progress++) {

    // Pins 2, 3, 4: 100 -> 0
    int angle234 = map(progress, 0, 100, 100, 0);

    // Pin 9: 90 -> 170
    int angle9 = map(progress, 0, 100, 90, 170);

    // Pins 10, 11: 0 -> 130
    int angle1011 = map(progress, 0, 100, 0, 130);

    servo2.write(angle234);
    servo3.write(angle234);
    servo4.write(angle234);

    servo9.write(angle9);

    servo10.write(angle1011);
    servo11.write(angle1011);

    delay(STEP_DELAY);
  }
}

void moveAllBack() {

  // progress goes from 0% to 100%
  for (int progress = 0; progress <= 100; progress++) {

    // Pins 2, 3, 4: 0 -> 100
    int angle234 = map(progress, 0, 100, 0, 100);

    // Pin 9: 170 -> 90
    int angle9 = map(progress, 0, 100, 170, 90);

    // Pins 10, 11: 130 -> 0
    int angle1011 = map(progress, 0, 100, 130, 0);

    servo2.write(angle234);
    servo3.write(angle234);
    servo4.write(angle234);

    servo9.write(angle9);

    servo10.write(angle1011);
    servo11.write(angle1011);

    delay(STEP_DELAY);
  }
}

void loop() {

  Serial.println("Moving all servos forward");

  moveAllForward();

  delay(500);

  Serial.println("Moving all servos back");

  moveAllBack();

  delay(500);
}