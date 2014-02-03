// Patch Cat Feeder Software
// Vikram Dhar vikram.dhar@gmail.com
// 26th August 2012
//
// Uses:
// TWIG-RTC from Seeeduino
// Freeduino-V1.22
// TowerPro-MicroServo-9g-SG90 from EvilMadScience
// NewHaven i2c 16x2 LCD display from Mouser
//
// PIN CONNECTIONS:
//
// SERVO  - Arduino
// brown  - gnd
// red    - 5v
// orange - digital pin 9
//
// Using the TWIG RTC v0.9b
// RTC - Arduino
// VCC - 5v - digital pin 12 ??
// GND - gnd
// SCL - analog pin 5
// SDA - analog pin 4
//
// NEW HAVEN 16x2 SERIAL LCD DISPLAY
// LCD - Arduino
// SCL - analog pin 5
// SDA - analog pin 4
//

#include <inttypes.h>
#include <Servo.h> 
#include <Wire.h>
#include <LCDi2cNHD.h>                    

#define DS1307_I2C_ADDRESS 0x68  // This is the RTC I2C address
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

const int debug = 1;
const byte INCREM_BY_1HOUR = 0;
const byte INCREM_BY_5MINUTES = 1;

int led = 0; //used to toggle led state

const int use_lcd = 1; //1 = enable, 0 = disable
LCDi2cNHD lcd = LCDi2cNHD(2,16,0x50>>1,0);
int backlight = 0; //used to toggle led state

Servo myservo;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 
int pos = 0;    // variable to store the servo position 
const int init_pos = 10;
const int max_pos = 175;

int analogPin = 0; //pin for 5 way switch/joystick

int dispense_count = 0;

const int morn_hour = 5;
const int nite_hour = 19;

int feed_hour = morn_hour;

void setup() { 
  Serial.begin(57600); 
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     
  Wire.begin();
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  init_servo();
  if(use_lcd) {
    lcd.init(); // Init the display, clears the display
    lcd.setContrast(20);
    lcd.setCursor(0,0);
    lcd.print("Patch Feeder 1.7");
    lcd.setCursor(1,0);
    lcd.print("Meow certified");
    lcd.setBacklight(1); //turn backlight to minimum
  }
  delay(500);
  lcd.print(".");
  Serial.println("3");
  delay(500);
  lcd.print(".");
  Serial.println("2");
  delay(500);
  lcd.print(".");
  Serial.println("1");
  delay(500);
  lcd.print(".");
  Serial.println("** WELCOME to Patch Feeder Software **");
  if(use_lcd) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Pay attention!");
    lcd.setCursor(1,0);
    lcd.print("use 5 way stick");
    delay(4000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("N S set clock");
    lcd.setCursor(1,0);
    lcd.print("E W feed time");
    delay(6000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("And push button in");
    lcd.setCursor(1,0);
    lcd.print("to give food now.");
    delay(4000);
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("Feed @ 5 means");
    lcd.setCursor(1,0);
    lcd.print("feed at 5:00 am");
    delay(4000);
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print("got that?");
    delay(2000);
  }
  fetch_time();
  if (hour >= morn_hour && hour < nite_hour) {
    feed_hour = nite_hour;
  } else {
    feed_hour = morn_hour;
  }
}

void loop()
{
  lcd.setBacklight(1); //turn backlight to minimum
  //This code is run once every ~10 seconds
  fetch_time();
  print_time_serial();
  show_time();
  int k;
  for (int s=1; s<2001; s++) {
    delay(5);
    k = read_switch();
    fetch_time();
    if (k) { //some key was pressed
      lcd.setBacklight(8); //max backlight
      if (k == 1) {
        set_time(INCREM_BY_1HOUR);
      } 
      else if (k == 2) {
        feed_hour_decrement();
      } 
      else if (k == 3) {
        set_time(INCREM_BY_5MINUTES);
      } 
      else if (k == 4) {
        feed_hour_increment();
      } 
      else if (k == 5) {
        dispense_food();
        break;
      } 
      else {
      }
      /*
      if (use_lcd) {
        String s = String(String("Next feed @ ") + String(feed_hour,DEC) + String("    "));
        char buf[17] = {
          0        };
        s.toCharArray(buf, 17);
        lcd.setCursor(1,0);
        lcd.print (buf);
        delay(10);
      }*/
      show_time();
    }
    if (!(s % 500)) { // flash the led
      lcd.setBacklight(1); //min backlight
      if (led) {
        digitalWrite(13, HIGH); // set the LED on
      } 
      else {
        digitalWrite(13, LOW); // set the LED off
      }
      led = !led;
    }
    if ((hour == feed_hour) && (minute == 0) ) {
      Serial.println("Its time to say goodbye (to the food)");
      dispense_food();
      char buf[17] = { 0 };
      String s = String("60 sec sleep now ");
      s.toCharArray(buf, 17);
      lcd.setCursor(1,0);
      lcd.print (buf);
      Serial.println("Taking 60 secs to catch my breath");
      for (int i=0;i<60;i++) delay(1000);
      Serial.println("Back to normal programming");
      fetch_time();
      if (feed_hour >= morn_hour && feed_hour < nite_hour) {
          feed_hour = nite_hour;
      } else {
          feed_hour = morn_hour;
      }
      print_time_serial();
      show_time();
    }
  }
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}


// Gets the time and stores it in the global variables
void fetch_time()
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  second     = bcdToDec(Wire.read() & 0x7f);
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());

}

void show_time()
{
  if (use_lcd) {
    lcd.clear();
    lcd.setCursor(0,0);
    String s = String(String("T=") + String(hour,DEC) + String(":") + String(minute,DEC) + String(" feeds=") + String(dispense_count,DEC) + String("  "));
    char buf[17] = {
      0    };
    s.toCharArray(buf, 16);
    lcd.print(buf);
    delay(50);
  }
  if (use_lcd) {
    String s = String(String("Next feed @ ") + String(feed_hour,DEC) + String("    "));
    char buf[17] = {
      0    };
    s.toCharArray(buf, 17);
    lcd.setCursor(1,0);
    lcd.print (buf);
  }
}

void set_time(byte change)
{
  if (use_lcd) {
    String s = String(String("setting time    ") );
    char buf[17] = {
      0    };
    s.toCharArray(buf, 17);
    lcd.setCursor(1,0);
    lcd.print (buf);
    delay(200);
  }
  second = (byte) (0);
  // Use of (byte) type casting and ascii math to achieve result.
  if (change == INCREM_BY_5MINUTES) {
    minute = (byte) (minute>=55?0:(minute+5));
    hour  = (byte) (hour);
  } 
  if (change == INCREM_BY_1HOUR) {
    minute = (byte) (minute);
    hour  = (byte) (hour>=23?0:(hour+1));
  }
  dayOfWeek = (byte) (0);
  dayOfMonth = (byte) (26);
  month = (byte) (8);
  year= (byte) (12);
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0x00);
  Wire.write(0x00);    // 0 to bit 7 starts the clock
  //Wire.write(decToBcd(second));    // 0 to bit 7 starts the clock
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));      // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();

  delay(50);
}

void print_time_serial()
{
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(second, DEC);
  Serial.print("  ");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" dispense_count:");
  Serial.print(dispense_count, DEC);
  Serial.println("  ");
}

void init_servo()
{
  //move to position ZEROish
  pos = init_pos;
  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
}

void dispense_food()
{
  //feed_hour = 5; // reset to default value
  dispense_count ++;
  if (use_lcd) {
    String s = String(String("dispense ") + String(dispense_count,DEC) + String(" start "));
    char buf[17] = {
      0                                                };
    s.toCharArray(buf, 17);
    lcd.setCursor(1,0);
    lcd.print (buf);
  }
  for(pos = init_pos; pos <= max_pos; pos += 10)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(50);                       // waits 15ms for the servo to reach the position 
  } 
  delay(6000);                       // waits 15ms for the servo to reach the position       
  for(pos = max_pos; pos >= init_pos; pos -= 10)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(50);                       // waits 15ms for the servo to reach the position 
  }   
  init_servo();
  if (use_lcd) {
    String s = String(String("dispense ") + String(dispense_count,DEC) + String(" done  "));
    char buf[17] = { 0 };
    s.toCharArray(buf, 17);
    lcd.setCursor(1,0);
    lcd.print (buf);
    delay(2000);
    //s = String("60 sec sleep now ");
    //s.toCharArray(buf, 17);
    //lcd.setCursor(1,0);
    //lcd.print (buf);
    //delay(2000);
  }
}

int analog2key(int val) {
  int key = 0;
  if(val <= 100) {
    key = 1;
  } 
  else if (val <= 300) {
    key = 2;
  } 
  else if (val <= 500) {
    key = 3;
  } 
  else if (val <= 700) {
    key = 4;
  } 
  else if (val <= 900) {
    key = 5;
  } 
  else {
    key = 0;
  }  
  return key;
}

int read_switch() {
  //Range: 0 - 1024
  //states: 6 = open, N, S, E, W, down
  //per state: 204 or approx 200
  int val = 0, key1 = 0, key2 = 0;
  val = analogRead(0); // read the input pin
  key1 = analog2key(val);
  //assuming that debounce delay inbuilt into analog2key 
  val = analogRead(0); // read the input pin
  key2 = analog2key(val);
  if (key1 != key2) {
    return 0;
  }
  if (debug && key2) {
    Serial.print("read_switch:"); // debug value
    Serial.println(key2); // debug value
  }
  return key2;
}

void feed_hour_decrement() {
  feed_hour --;
  if (feed_hour < 0) feed_hour = 23;
}

void feed_hour_increment() {
  feed_hour ++;
  if (feed_hour >= 24) feed_hour = 0;
}

