#include <Wire.h>
#include <SeeedOLED.h>
#include <Servo.h>

#define encoder0PinA  2
#define encoder0PinB  7
#define encoder0Push  4

#define contiServoPin 3

#define led           13

#define sensorPin     A0 //Sharp IR rangefinder

#define rtc_i2c_addr 0x68

volatile int clicks = 0;
volatile int clacks = 0;
volatile int push = 1;

// 1=Sunday, 2=Monday, ... 7=Saturday
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

String stime;
char cbuf[21] ={0};

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
    attachInterrupt((encoder0PinA-2), handleRotation, CHANGE);
    attachInterrupt((encoder0Push-2), handlePush, CHANGE);

    Serial.begin (9600);

    Wire.begin();

    SeeedOled.init();  //initialze SEEED OLED display
    //DDRB|=0x21;        
    //PORTB |= 0x21;
    SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
    SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
    SeeedOled.setPageMode();           //Set addressing mode to Page Mode
    SeeedOled.setTextXY(0,0);          //Set the cursor to Xth Page, Yth Column  
    //SeeedOled.setHorizontalMode();      //Set addressing mode to Horizontal Mode
    clearLine(0);
    SeeedOled.putString("Patch Feeder v2");
    clearLine(1);
    SeeedOled.putString("CW=up CCW=down");

    Wire.beginTransmission(rtc_i2c_addr); // address DS3231
    Wire.write(0x0E); // select register
    Wire.write(0b00011100); // write register bitmap, bit 7 is /EOSC
    Wire.endTransmission();

    myServo.attach(contiServoPin);
    myServo.writeMicroseconds(1500);  // Stop rotating

    delay(1000);
    Serial.println("READY!");

    fetch_time();

    ////// Use byte type casting and if you want to hardcode values
    //second = (byte) (0);
    //minute = (byte) (12);
    //hour  = (byte) (0);
    //dayOfWeek = (byte) (7);
    //dayOfMonth = (byte) (31);
    //month = (byte) (8);
    //year = (byte) (13);

    //set_time();

    clearLine(4);
    SeeedOled.putString("servo stop");
} 



void test_loop()
{
    digitalWrite(led, 1);
    delay(500);
    SeeedOled.putString("x");
    digitalWrite(led, 0);
    delay(500);
    SeeedOled.putString("+");
}



void loop() 
{
    // First print current status of inputs
    clearLine(2);
    SeeedOled.putString("push:");
    SeeedOled.putNumber(push);
    clearLine(3);
    SeeedOled.putString("cli:");
    SeeedOled.putNumber(clicks);
    SeeedOled.putString(" cla:");
    SeeedOled.putNumber(clacks);

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

    stime = String( 
            String(hour,DEC) + String(":") + 
            String(minute,DEC) + String(":") + 
            String(second,DEC) + String(":")
            );
    stime.toCharArray(cbuf, 21);
    clearLine(6);
    SeeedOled.putString(cbuf);

    stime = String(
            String(dayOfWeek,DEC) + String(":") + 
            String(dayOfMonth,DEC) + String(":") + 
            String(month,DEC) + String(":") + 
            String(year,DEC) + String(":")
            );
    stime.toCharArray(cbuf, 21);
    clearLine(7);
    SeeedOled.putString(cbuf);

    if (!push || clicks || clacks)
    {
        // rotary input needs to be handled
        // Handle platform down (clack) first.
        // Only handle one direction per loop() invocation so that 
        // clack always gets priority over click.
        if (consume_clack())
        {
            digitalWrite(led, 1);
            Serial.println("clack down");

            clearLine(4);
            SeeedOled.putString("clack down");

            myServo.writeMicroseconds(1700);  // Clockwise
            delay(1800);
            myServo.writeMicroseconds(1500);  // Stop

            clearLine(4);
            SeeedOled.putString("servo stop");
            Serial.println("servo stop");
            //// check range
            //int sensorValue = analogRead(sensorPin);    
            //clearLine(5);
            //SeeedOled.putString("Range:");
            //SeeedOled.putNumber(sensorValue);
            //Serial.print ("Range:");
            //Serial.println (sensorValue, DEC);    
        } 
        else if (consume_click())
        {
            digitalWrite(led, 0);
            Serial.println("click up");
            clearLine(4);
            SeeedOled.putString("click up");
            myServo.writeMicroseconds(1300);  // Anti-Clockwise
            delay(1800);
            myServo.writeMicroseconds(1500);  // Stop
            clearLine(4);
            SeeedOled.putString("servo stop");
            Serial.println("servo stop");
            //// check range
            //int sensorValue = analogRead(sensorPin);    
            //clearLine(5);
            //SeeedOled.putString("Range:");
            //SeeedOled.putNumber(sensorValue);
            //Serial.print ("Range:");
            //Serial.println (sensorValue, DEC);    
        }
    }
    else {
        //if nothing was pressed delay
        delay(500);
        fetch_time();
    }
}



// prints 16 spaces to line 'num'
// num: 0 to 7
void clearLine(int num)
{
    SeeedOled.setTextXY(num,0);          //Set the cursor to Xth Page, Yth Column  
    SeeedOled.putString("                ");
}



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

