// Patch Feeder firmware for V3 physical design.
// Design aim is to make an automatic multi feeder for Patch cat.
#include <Wire.h>
#include <Servo.h>
#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

// Components used:
// Freeduino-V1.22 ATmega328 with FTDI on board
// ChronoDot
// SEEED Studio TWIG OLED 128x64 monochrome - with u8glib
// Parallav continuous rotation servo
// Adafruit rotary encoder 24 clicks

#define encoder0PinA  2
#define encoder0PinB  7
#define encoder0Push  4

#define contiServoPin 3

#define led           13

#define sensorPin     A0 //Sharp IR rangefinder

#define rtc_i2c_addr 0x68

volatile int clicks = 0;
volatile int clacks = 0;
volatile int push = 0;

// 1=Sunday, 2=Monday, ... 7=Saturday
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

const byte feed_minute = 30;
const byte feed_hours[2] = {5, 17};

String stime;
char cbuf[21] = {0};

bool update_time = 0;
bool testing = false;

Servo myServo;

void setup()
{
  pinMode(led, OUTPUT);

  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(encoder0Push, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  digitalWrite(encoder0Push, HIGH);       // turn on pullup resistor
  attachInterrupt((encoder0PinA - 2), handleRotation, CHANGE);
  attachInterrupt((encoder0PinB - 2), handleRotation, CHANGE);
  attachInterrupt((encoder0Push - 2), handlePush, CHANGE);

  Serial.begin (115200);

  Wire.begin();
  Wire.beginTransmission(rtc_i2c_addr); // address DS3231
  Wire.write(0x0E); // select register
  Wire.write(0b00011100); // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();

  // Completely disable the servo till we decide to move it to avoid 
  // creeping in continuous rotation servo motors.
  //myServo.attach(contiServoPin);
  //myServo.writeMicroseconds(1500);  // Stop rotating

  delay(2000);
  Serial.println("READY!");

  fetch_time();

  Serial.println ("BEFORE");
  Serial.print ("hour:");
  Serial.println (hour, DEC);
  Serial.print ("minute:");
  Serial.println (minute, DEC);

  if (update_time) {
    ////// Use byte type casting and if you want to hardcode values
    second = (byte) (0);
    minute = (byte) (0);
    hour  = (byte) (1);
    dayOfWeek = (byte) (2); // 1=Sunday, 2=Monday, ... 7=Saturday
    dayOfMonth = (byte) (11);
    month = (byte) (5);
    year = (byte) (15);
    set_time();
  }

  Serial.println ("AFTER");
  Serial.print ("hour:");
  Serial.println (hour, DEC);
  Serial.print ("minute:");
  Serial.println (minute, DEC);

  Serial.print ("debug feed_hours size:");
  Serial.println (sizeof(feed_hours), DEC);
}


void loop()
{
  Serial.print ("pu:");
  Serial.print (push, DEC);
  Serial.print (" cli:");
  Serial.print (clicks, DEC);
  Serial.print (" cla:");
  Serial.println (clacks, DEC);

  //int sensorValue = analogRead(sensorPin);
  //clearLine(5);
  //SeeedOled.putString("Range:");
  //SeeedOled.putNumber(sensorValue);
  //Serial.print ("Range:");
  //Serial.println (sensorValue, DEC);

  // OLED draw loop
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );

  // when in testing mode dispense every minute at second==20
  if (testing && !(second % 20)) {
    dispense();
  }

  // real dispense check
  if (minute == feed_minute && second < 4) {
    for (byte i = 0; i < sizeof(feed_hours); i++)
    {
      if (hour == feed_hours[i]) {
        dispense();
        break;
      }
    }
  }

  //if nothing was pressed delay
  delay(100);
  fetch_time();

  static byte toggle = 0;
  digitalWrite(led, toggle);
  toggle = !toggle;
}

void dispense()
{
  myServo.attach(contiServoPin);
  myServo.writeMicroseconds(1500);  // Stop rotating

  Serial.println("Feed!");
  delay(500);
  myServo.writeMicroseconds(1525);  // Clockwise
  Serial.println("Done!");
  delay(8000);
  myServo.writeMicroseconds(1500);  // Stop rotating
  Serial.println("Resume!");
  myServo.detach();
  delay(2000);
}

/////////////////////////////////////////////////////////
// Rotary encoder
/////////////////////////////////////////////////////////


// rotary encoder rotation detection
void handleRotation() {
  // assuming hardware debouncing using capacitors
  if (digitalRead(encoder0PinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder0PinB) == LOW) {  // check channel B to see which way
      clacks ++;
    }
  }
  else                                        // found a high-to-low on channel A
  {
    if (digitalRead(encoder0PinB) == LOW) {   // check channel B to see which way
      clicks ++;
    }
  }
}

// rotary encoder push detection
void handlePush() {
  // assuming NO-hardware debouncing using capacitors
  push = digitalRead(encoder0Push);
  delay(5); // debounce
  if (push != digitalRead(encoder0Push)) {
    push = 1;
  }
}

int consume_click()
{
  return clicks ? clicks-- : 0 ;
}

int consume_clack()
{
  return clacks ? clacks-- : 0 ;
}


/////////////////////////////////////////////////////////
// RTC
/////////////////////////////////////////////////////////


// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}


// Gets the time and stores it in the global variables
void fetch_time()
{
  // Reset the register pointer
  Wire.beginTransmission(rtc_i2c_addr);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(rtc_i2c_addr, 7);

  // A few of these need masks because certain bits are control bits
  second     = bcdToDec(Wire.read() & 0x7f);
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());

}

void set_time()
{
  //// Use byte type casting and if you want to hardcode values
  //second = (byte) (second);
  //minute = (byte) (minute);
  //hour  = (byte) (hour);
  //dayOfWeek = (byte) (dayOfWeek);
  //dayOfMonth = (byte) (dayOfMonth);
  //month = (byte) (month);
  //year= (byte) (year);

  Wire.beginTransmission(rtc_i2c_addr);
  Wire.write(0x00);
  Wire.write(decToBcd(second));    // 0 to bit 7 starts the clock
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));      // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}


/////////////////////////////////////////////////////////
// SEEED STUDIO 128x64 monochrome OLED display
/////////////////////////////////////////////////////////


void draw(void) {
  // graphic commands to redraw the complete screen should be placed here

  // switch background and foreground colors every minute to prevent display burn in
  if ( minute % 2 ) { // if odd minute
    u8g.setColorIndex(1);
    u8g.drawBox(0, 0, 127, 63);     // draw cursor bar
    u8g.setColorIndex(0);
  } else {
    u8g.setColorIndex(1);
  }

/*
    u8g.setFont(u8g_font_unifont);
    u8g.setFont(u8g_font_osb21);
    u8g.setFont(u8g_font_fur30);
    u8g.setFont(u8g_font_10x20);
    u8g.setFont(u8g_font_ncenR12);
    u8g.setFont(u8g_font_fur11);
    u8g.setFont(u8g_font_6x13); 
    u8g.setFont(u8g_font_helvR12);
    u8g.setFont(u8g_font_helvR08);
*/
    u8g.setFont(u8g_font_7x13); 
  
  stime = String( String(" Time: ") +
            String(hour, DEC) + String(":") +
            String(minute, DEC) + String(":") +
            String(second, DEC)
          );
  stime.toCharArray(cbuf, 21);
  u8g.drawStr( 0,  12, cbuf);

  stime = String(" ") + String (sizeof(feed_hours)) + String(" feeds, at:");
  stime.toCharArray(cbuf, 21);
  u8g.drawStr( 0,  28, cbuf);
  
  stime = String(" ");
  for (byte i = 0; i < sizeof(feed_hours); i++)
  {
    stime += String(feed_hours[i], DEC) + String(":") +
             String (feed_minute) + String(" ");
  }
  stime.toCharArray(cbuf, 21);
  u8g.drawStr( 0,  44, cbuf);
 
  u8g.drawStr( 0, 60, " PatchFeeder V3.x");
}

