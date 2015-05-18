#include <Servo.h>

#define encoder0PinA  2
#define encoder0PinB  7
#define encoder0Push  4

#define contiServoPin 3

#define led           13

Servo myServo;

void setup()
{
  Serial.begin (115200);

  myServo.attach(contiServoPin);
  myServo.writeMicroseconds(1500);  // Stop rotating

  Serial.println("READY!");
}

void loop() 
{
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
    // look for the next valid integer in the incoming serial stream:
    int speed = Serial.parseInt(); 
    Serial.println("----------------------------------");
    Serial.print("got: ");
    Serial.println(speed);
    if (speed < 1400 || speed > 1600) 
      speed = 1500;    
    Serial.print("adjusted: ");
    Serial.println(speed);
    myServo.writeMicroseconds(1500);  // Stop
    myServo.writeMicroseconds(speed);
  }
  /*
  myServo.writeMicroseconds(1500);  // Stop
  myServo.writeMicroseconds(1300);  // Anti-Clockwise
  myServo.writeMicroseconds(1700);  // Clockwise
  delay(200);
  */
}


