// Patch Muli-Feeder firmware for V3 physical design.
// Design aim was/is to make an automatic multi feeder for Patch cat.

#include <Wire.h>
#include <Servo.h>
#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

// Components used:
// Arduino:
//    EMSL Diavolino with Adafruit bluefruit EZ-Link
// ChronoDot RTC
// SEEED Studio TWIG OLED 128x64 monochrome - with u8glib
// Parallax continuous rotation servo
// Adafruit rotary encoder 24 clicks
//    coupled to servo shaft with Servocity chain and sprocket
// Piezo buzzer

#define encoder0PinA  2
#define encoder0PinB  7
#define encoder0Push  4

#define contiServoPin 3

#define led           13

#define rtc_i2c_addr 0x68

#define buzzer_pin 10

Servo myServo;

// 1=Sunday, 2=Monday, ... 7=Saturday
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
bool update_time = 0;

String stime;
char cbuf[21] = {0};

void march(unsigned speakerPin);

// compile time testing mode
//#define TESTING

/////////////////////////////////////////////////////////
// Time and portion control
/////////////////////////////////////////////////////////

//////// How to set the clicks value ////////
//24 detents per rotation
// 4 transitions per detent
//16 transitions/clicks for 1/6 rotation


#ifdef TESTING
const byte feed_entries   = 3;
const byte feed_hours[3]  = {17, 18, 18};
const byte feed_minute[3] = {59, 02, 04};
const byte feed_clicks[3] = {64, 64, 64};
#else
//@home:
const byte feed_entries   = 3;
const byte feed_hours[3]  = { 5,  6, 17};
const byte feed_minute[3] = {30, 30, 30};
const byte feed_clicks[3] = {32, 32, 32};
//Pet Sitter:
//const byte feed_entries   = 2;
//const byte feed_hours[2]  = { 5, 21};
//const byte feed_minute[2] = {30,  0};
//const byte feed_clicks[2] = {32, 32};
#endif
// value tuned as per the duration of dispense() method
const byte feed_second    = 4;



/////////////////////////////////////////////////////////
// setup and loop
/////////////////////////////////////////////////////////

void setup()
{
  pinMode(led, OUTPUT);

  delay(3000);
  Serial.begin (115200);
  delay(500);
  Serial.print(millis(), DEC);
  Serial.println (" : setup() : pin setup");

  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(encoder0Push, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  digitalWrite(encoder0Push, HIGH);       // turn on pullup resistor

  Wire.begin();
  Wire.beginTransmission(rtc_i2c_addr); // address DS3231
  Wire.write(0x0E); // select register
  Wire.write(0b00011100); // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();

  fetch_time();

  if (update_time) {
    Serial.println ("BEFORE");
    Serial.print ("hour:");
    Serial.println (hour, DEC);
    Serial.print ("minute:");
    Serial.println (minute, DEC);

    ////// Use byte type casting and if you want to hardcode values
    second = (byte) (0);
    minute = (byte) (0);
    hour  = (byte) (1);
    dayOfWeek = (byte) (2); // 1=Sunday, 2=Monday, ... 7=Saturday
    dayOfMonth = (byte) (11);
    month = (byte) (5);
    year = (byte) (15);
    set_time();

    Serial.println ("AFTER");
    Serial.print ("hour:");
    Serial.println (hour, DEC);
    Serial.print ("minute:");
    Serial.println (minute, DEC);
  }

# ifdef TESTING
  Serial.print(millis(), DEC);
  Serial.println (" : setup() : in TESTING mode");
# endif
}

void loop()
{
  //if nothing was pressed delay
  delay(100);
  fetch_time();

  static byte toggle = 0;
  digitalWrite(led, toggle);
  toggle = !toggle;

  // OLED draw loop
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );

# ifdef TESTING
  if (Serial.available() != 0) {
    if (Serial.read() == '1') {
      Serial.print(millis(), DEC);
      Serial.println (" : loop() : TEST triggered");
      delay(2000);
      dispense(16);
    }
  }
# endif

  // real dispense check
  for (byte i = 0; i < feed_entries; i++)
  {
    if (hour == feed_hours[i] &&
        minute == feed_minute[i] &&
        second < feed_second )
    {
      dispense(feed_clicks[i]);
      break;
    }
  }

}

/////////////////////////////////////////////////////////
// food dispense control
/////////////////////////////////////////////////////////

void stop_servo()
{
  myServo.attach(contiServoPin);
  myServo.writeMicroseconds(1500);  // Stop rotating
  myServo.detach();
  Serial.print(millis(), DEC);
  Serial.println(" : stop_servo : stopped");
}

void start_servo(int power)
{
  myServo.attach(contiServoPin);
  myServo.writeMicroseconds(power);  // Clockwise
  Serial.print(millis(), DEC);
  Serial.println(" : start_servo : started");
}

void dispense(byte clicks)
{
  String dtime = String( 
                  String(millis(), DEC) +
                  String(" : dispense@ ") +
                  String(hour, DEC) + String(":") +
                  String(minute, DEC) + String(":") +
                  String(second, DEC)
                );
  Serial.println(dtime);

  // sound the dinner bell
# ifndef TESTING
  march(buzzer_pin);
# else
  tone(buzzer_pin, 2000, 500);
  delay(600);
# endif

  int a = 0, b = 0, x = 0, y = 0, count = 0;
  unsigned long tstamp = 0;
  unsigned long tstamp_toggle = 0;

  tstamp = millis();
  tstamp_toggle = tstamp;
  a = digitalRead(encoder0PinA);
  b = digitalRead(encoder0PinB);
  Serial.print(tstamp, DEC);
  Serial.print(" ");
  Serial.print(a);
  Serial.print(b);
  Serial.println("");

  start_servo(1475);

  while (1) {
    bool print = false;

    // suspect HW debouncing, No SW debouncing so help us god!
    x = digitalRead(encoder0PinA);
    y = digitalRead(encoder0PinB);

    if (x != a) {
      print = true;
      tstamp_toggle=millis();
      a = x;
      count ++;
    }
    if (y != b) {
      print = true;
      tstamp_toggle=millis();
      b = y;
      count ++;
    }
    if (print) {
      dtime = String( 
                  String(millis(), DEC) +
                  String(" : ") +
                  String(a, DEC) + String(b, DEC)
                );
      Serial.println(dtime);
      /*
      Serial.print(millis(), DEC);
      Serial.print(" ");
      Serial.print(a);
      Serial.print(b);
      Serial.println("");
      */
    }
    //24 detents per rotation
    // 4 transitions per detent
    //16 transition for 1/6 rotation
    if (count >= clicks) {
      stop_servo(); // stop immediately
      Serial.print(millis(), DEC);
      Serial.print(" : dispense() : ");
      Serial.print(count);
      Serial.print(" >= ");
      Serial.print(clicks);
      Serial.println("");
      break;
    }
    // watchdog
    if (millis()-tstamp_toggle > 15000ul) {
      // if it has been 15+ seconds since the last transition/toggle
      //  then stop and abort
      stop_servo();
      Serial.print(millis(), DEC);
      Serial.println(" : dispense() : ABORT");
      break;
    } else if (millis()-tstamp_toggle > 10000ul) {
      // if it has been 10+ seconds since the last transition/toggle
      //  then increase motor power
      Serial.print(millis(), DEC);
      Serial.println(" : dispense() : 10+ power up to 1535");
      start_servo(1425);
    } else if (millis()-tstamp_toggle > 5000ul) {
      // if it has been 5+ seconds since the last transition/toggle
      //  then increase motor power
      start_servo(1445);
      Serial.print(millis(), DEC);
      Serial.println(" : dispense() : 5+ power up to 1530");
    }
  };

  delay(2000);
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
             String (feed_minute[i]) + String(" ");
  }
  stime.toCharArray(cbuf, 21);
  u8g.drawStr( 0,  44, cbuf);

  u8g.drawStr( 0, 60, " PatchMultiFeeder");
}

/////////////////////////////////////////////////////////
// Imperial March on Buzzer
/////////////////////////////////////////////////////////

#define c 261
#define d 294
#define e 329
#define f 349
#define g 391
#define gS 415
#define a 440
#define aS 455
#define b 466
#define cH 523
#define cSH 554
#define dH 587
#define dSH 622
#define eH 659
#define fH 698
#define fSH 740
#define gH 784
#define gSH 830
#define aH 880
//frequencies for the tones we're going to use
//used http://home.mit.bme.hu/~bako/tonecalc/tonecalc.htm to get these

void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)
{
  //digitalWrite(ledPin, HIGH);
  //use led to visualize the notes being played

  int x;
  long delayAmount = (long)(1000000 / frequencyInHertz);
  long loopTime = (long)((timeInMilliseconds * 1000) / (delayAmount * 2));
  for (x = 0; x < loopTime; x++)
  {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(delayAmount);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(delayAmount);
  }

  //digitalWrite(ledPin, LOW);
  //set led back to low

  delay(20);
  //a little delay to make all notes sound separate
}

void march(unsigned speakerPin)
{
  //sets the speakerPin to be an output
  pinMode(speakerPin, OUTPUT);

  //for the sheet music see:
  //http://www.musicnotes.com/sheetmusic/mtd.asp?ppn=MN0016254
  //this is just a translation of said sheet music to frequencies / time in ms
  //used 500 ms for a quart note

  beep(speakerPin, a, 500);
  beep(speakerPin, a, 500);
  beep(speakerPin, a, 500);
  beep(speakerPin, f, 350);
  beep(speakerPin, cH, 150);

  beep(speakerPin, a, 500);
  beep(speakerPin, f, 350);
  beep(speakerPin, cH, 150);
  beep(speakerPin, a, 1000);
  //first bit

  beep(speakerPin, eH, 500);
  beep(speakerPin, eH, 500);
  beep(speakerPin, eH, 500);
  beep(speakerPin, fH, 350);
  beep(speakerPin, cH, 150);

  beep(speakerPin, gS, 500);
  beep(speakerPin, f, 350);
  beep(speakerPin, cH, 150);
  beep(speakerPin, a, 1000);
  //second bit...

  beep(speakerPin, aH, 500);
  beep(speakerPin, a, 350);
  beep(speakerPin, a, 150);
  beep(speakerPin, aH, 500);
  beep(speakerPin, gSH, 250);
  beep(speakerPin, gH, 250);

  beep(speakerPin, fSH, 125);
  beep(speakerPin, fH, 125);
  beep(speakerPin, fSH, 250);
  delay(250);
  beep(speakerPin, aS, 250);
  beep(speakerPin, dSH, 500);
  beep(speakerPin, dH, 250);
  beep(speakerPin, cSH, 250);
  //start of the interesting bit

  beep(speakerPin, cH, 125);
  beep(speakerPin, b, 125);
  beep(speakerPin, cH, 250);
  delay(250);
  beep(speakerPin, f, 125);
  beep(speakerPin, gS, 500);
  beep(speakerPin, f, 375);
  beep(speakerPin, a, 125);

  beep(speakerPin, cH, 500);
  beep(speakerPin, a, 375);
  beep(speakerPin, cH, 125);
  beep(speakerPin, eH, 1000);
  //more interesting stuff (this doesn't quite get it right somehow)

  beep(speakerPin, aH, 500);
  beep(speakerPin, a, 350);
  beep(speakerPin, a, 150);
  beep(speakerPin, aH, 500);
  beep(speakerPin, gSH, 250);
  beep(speakerPin, gH, 250);

  beep(speakerPin, fSH, 125);
  beep(speakerPin, fH, 125);
  beep(speakerPin, fSH, 250);
  delay(250);
  beep(speakerPin, aS, 250);
  beep(speakerPin, dSH, 500);
  beep(speakerPin, dH, 250);
  beep(speakerPin, cSH, 250);
  //repeat... repeat

  beep(speakerPin, cH, 125);
  beep(speakerPin, b, 125);
  beep(speakerPin, cH, 250);
  delay(250);
  beep(speakerPin, f, 250);
  beep(speakerPin, gS, 500);
  beep(speakerPin, f, 375);
  beep(speakerPin, cH, 125);

  beep(speakerPin, a, 500);
  beep(speakerPin, f, 375);
  beep(speakerPin, c, 125);
  beep(speakerPin, a, 1000);
  //and we're done \ó/
}

