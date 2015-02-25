// User --HTTP--> Agent -imp-cloud-> ImpHW --UART--> Teensy --USB-kbd--> PC
#include <TimerOne.h>

const int led = LED_BUILTIN;  // the pin with a LED

// The interrupt will blink the LED, and keep
// track of how many times it has blinked.
int ledState = LOW;
// use volatile for shared variables
volatile unsigned long blinkCount = 0; 


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(led, OUTPUT);
  // timer triggers every 1.5 seconds
  Timer1.initialize(1500000);
  Timer1.attachInterrupt(blinkLED);
  // initialize control over the keyboard:
  Keyboard.begin();  
  Serial.println("setup finished.");
}

int wrap_count = 0;

void loop() {
  //wait for command from Serial1
  if (Serial1.available() > 0) {
    int inByte = Serial1.read();
    Serial.println("");
    Serial.print("UART rx: ");
    Serial.println((char)inByte);
    Serial.print("Sending to keyboard... ");
    //delay(1000);
    Keyboard.println("password");
    Serial.println("Done");
  }
  Serial.print(".");
  delay(1000);
  wrap_count++;
  if (wrap_count == 20) {
    Serial.println("");
    wrap_count = 0;
    Serial1.print('$');
  }
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    Serial.println("");
    Serial.print("UART rx: ");
    Serial.println((char)inByte);
    Serial.print("Sending to Serial1... ");
    delay(200);
    Serial1.print(inByte);
    Serial.println("Done");
  }
}

void blinkLED(void)
{
  digitalWrite(led, LOW);
  blinkCount ++;
  if (blinkCount == 10) {
    /*
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    */
    blinkCount = 0;
    digitalWrite(led, HIGH);
  }
}

/*
120120
*/
