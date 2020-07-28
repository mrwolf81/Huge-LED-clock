


/* 
Red = Ard Pin 10
White = Ard Pin 11
Green = Ard Pin 12
DHT11() is disabled
 Version 1.2 with DHT11 RH, DS18B20 & Temp & secret voltmeter
 Version 1.3 with 7-segment testing, blink 5 times
 
 Original sketch from Paul Electronics 7-segment shift register posting
 RTC libraries from http://jeelabs.net/projects/cafe/wiki/RTClib
 Setting realtime clock on-compile was from Ladyada.net
 
 Pin assignment on the 74595 to 7-segment common cathode as follows :-
 
 QA - a
 QB - b
 QC - c
 QD - d
 QE - e
 QF - f
 QG - g
 
 pin 3 & 8 are common cathode
 resistors used is 220R
  
 Stanley Seow
 stanleyseow@gmail.com
 
 */

#include <Wire.h>                // i2c wire libraries for RTC
#include "RTClib.h"              // RTC libraries
#define DHT11_PIN 0             // ADC0
#define TIME_24_HOUR false
                                 // define 74595 pins
const int  g_pinData = 5;       // SI  (Pin 14 on 74595) Red wire
const int  g_pinCommLatch = 7;  // RCK (Pin 12 on 74595) White wire
const int  g_pinClock = 6;      // SCK (Pin 11 on 74595) Green wire
const int  ledPin = 13; // for blinking every 1/2 seconds
const int  ledpinA = 12;
const int  button1 = 2;
int buttonState = 0;

int temp = 0;
int humid = 0;
int volt = 0;

RTC_DS1307 RTC;                  // define RTC variables

byte g_digits [10];             // Definitions of the 7-bit values for displaying digits

int g_numberToDisplay = 0;      // default number being displayed, 0 

const int g_registers = 4;      // Number of shift registers in use, 4

byte g_registerArray [g_registers]; // Array of numbers to pass to shift registers

// Begin setup() functions //

void setup()
{
  
   // DHT11 setup
   DDRC |= _BV(DHT11_PIN);
   PORTC |= _BV(DHT11_PIN);
  
   // I2C RTC Setup
   Wire.begin();
   RTC.begin();
   
  /*  Only set the time on compile if the RTC is not running...
  This is used to set the current time from the computer clock
  */
  if ( !RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  

  pinMode (g_pinCommLatch, OUTPUT);       // define 74595 pins as output
  pinMode (g_pinClock, OUTPUT);           // define 74595 pins as output
  pinMode (g_pinData, OUTPUT);            // define 74595 pins as output
  pinMode (ledPin, OUTPUT);               // define LED pins as output
  pinMode (ledpinA, OUTPUT);
  pinMode (button1, INPUT);
  
  Serial.begin (9600);                    // optional, turn on serial monitoring for debugging

  // Setup 7 segment display for number 0 to 9 and other characters
  
  // a - top bar
  // b - top right
  // c - bottom right
  // d - bottom bar
  // e - bottom left
  // f - top right
  // g - middle bar
  
  int a = 1, b = 2, c = 4, d = 8, e = 16, f = 32, g = 64;

  g_digits [0] = 8 + 4 + 2 + 64 + 32 + 1;
  g_digits [1] = 4 + 2;
  g_digits [2] = 8 + 4 + 16 + 32 + 64;
  g_digits [3] = 8 + 4 + 16 + 2 + 64;
  g_digits [4] = 1 + 16 + 4 + 2;
  g_digits [5] = 8 + 1 + 16 + 2 + 64;
  g_digits [6] = 8 + 1 + 16 + 2 + 64 + 32;
  g_digits [7] = 8 + 4 + 2;
  g_digits [8] = 8 + 4 + 2 + 64 + 32 + 1 + 16;
  g_digits [9] = 8 + 4 + 2 + 1 + 16 + 64;
  g_digits [90] = 1 + 16 + 4 + 8;           // Degree dot
  g_digits [91] = 1 + 32 + 64 + 8;           // Capital C
  g_digits [92] = 32 + 16;                   //  r, 80
  g_digits [93] = 32 + 16 + 1 + 2;           //  h, 116
  
  g_digits [99] = 0;
  
  // 7-segment testing... blink 4 times
  for (int i=0;i<5;i++) {
  
    g_registerArray [0] = g_digits [8];
    g_registerArray [1] = g_digits [8];
    g_registerArray [2] = g_digits [8];
    g_registerArray [3] = g_digits [8];
   
    sendSerialData (g_registers, g_registerArray);
    delay(200);

    g_registerArray [0] = g_digits [99];
    g_registerArray [1] = g_digits [99];
    g_registerArray [2] = g_digits [99];
    g_registerArray [3] = g_digits [99];
   
    sendSerialData (g_registers, g_registerArray);
    delay(200);
     
  }
  
    
  
} // End of setup() //

// Simple function to send serial data to one or more shift registers by iterating backwards through an array.
// Although g_registers exists, they may not all be being used, hence the input parameter.

void sendSerialData (byte registerCount, byte *pValueArray) {
  // Signal to the 595s to listen for data
  digitalWrite (g_pinCommLatch, LOW);

  for (byte reg = registerCount; reg > 0; reg--)
  {
    byte value = pValueArray [reg - 1];

    for (byte bitMask = 128; bitMask > 0; bitMask >>= 1)
    {
      digitalWrite (g_pinClock, LOW);
      digitalWrite (g_pinData, value & bitMask ? HIGH : LOW);
      digitalWrite (g_pinClock, HIGH);
    }
  }
  // Signal to the 595s that I'm done sending
  digitalWrite (g_pinCommLatch, HIGH);
}  // sendSerialData


// Fucntion to read Arduino hidden voltmeter
long readVcc() {                 
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);                     // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}


// codes for DHT11 Temp & humidity sensors
byte read_dht11_dat() {   
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++)
  {
    while(!(PINC & _BV(DHT11_PIN)));  // wait for 50us
    delayMicroseconds(30);
    if(PINC & _BV(DHT11_PIN)) 
      result |=(1<<(7-i));
    while((PINC & _BV(DHT11_PIN)));  // wait '1' finish
    }
    return result;
}

int dht11() {
  byte dht11_dat[5];
  byte dht11_in;
  byte i;// start condition
   // 1. pull-down i/o pin from 18ms
  PORTC &= ~_BV(DHT11_PIN);
  delay(18);
  PORTC |= _BV(DHT11_PIN);
  delayMicroseconds(40);
  DDRC &= ~_BV(DHT11_PIN);
  delayMicroseconds(40);
  
  dht11_in = PINC & _BV(DHT11_PIN);
  if(dht11_in)
  {
    Serial.println("dht11 start condition 1 not met");
    return 0;
  }
  delayMicroseconds(80);
  dht11_in = PINC & _BV(DHT11_PIN);
  if(!dht11_in)
  {
    Serial.println("dht11 start condition 2 not met");
    return 0;
  }
  
  delayMicroseconds(80);// now ready for data reception
  for (i=0; i<5; i++)
    dht11_dat[i] = read_dht11_dat();
  DDRC |= _BV(DHT11_PIN);
  PORTC |= _BV(DHT11_PIN);
  byte dht11_check_sum = dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3];// check check_sum
  if(dht11_dat[4]!= dht11_check_sum)
  {
    Serial.println("DHT11 checksum error");
  }
  
  humid = dht11_dat[0];
  temp = dht11_dat[2];

  return temp,humid;           // return 2 values when this functions is called
}  

// ======================  Main loop() ======================= 

void loop()
{
  int hour,minute,sec,disp = 0;
  buttonState = digitalRead(button1);

   DateTime now = RTC.now();  // Get current time & date
   hour = now.hour();         // break down time to hour
   minute = now.minute();     // break down time to minute
   sec = now.second();        // break down time to second
   if (buttonState == HIGH){
    if (hour > 12){
      hour -=12;
   }else(hour == 0);
        hour +=12;
   }
   else if(buttonState == LOW){
    if (hour < 12){
      hour +=12;
    }else(hour == 0);
      hour -=12;
    }

   
  /* Serial output debugging for the date & time 
  
   Serial.print(now.year(), DEC);
   Serial.print('/');
   Serial.print(now.month(), DEC);
   Serial.print('/');
   Serial.print(now.day(), DEC);
   Serial.print(' ');
 */
 
   Serial.print(hour);
   Serial.print(':');
   Serial.print(minute);
   Serial.print(':');    
   Serial.print(sec);
   Serial.println();
   
  
  
  // Push the hour 2 digits to the left by multiplying 100
  
  disp = (hour * 100) + minute;
  g_numberToDisplay = disp;

   
  // Push the numbers to the four digits
  
  if (g_numberToDisplay < 10)
  {
    g_registerArray [0] = g_digits [0];
    g_registerArray [1] = g_digits [0];
    g_registerArray [2] = g_digits [0];
    g_registerArray [3] = g_digits [g_numberToDisplay];
  }
  else if (g_numberToDisplay < 100)
  {
    g_registerArray [0] = g_digits [0];
    g_registerArray [1] = g_digits [0];
    g_registerArray [2] = g_digits [g_numberToDisplay / 10];
    g_registerArray [3] = g_digits [g_numberToDisplay % 10];
  }
  else if (g_numberToDisplay < 1000)
  {
    g_registerArray [0] = g_digits [0];
    g_registerArray [1] = g_digits [g_numberToDisplay / 100];
    g_registerArray [2] = g_digits [(g_numberToDisplay % 100) / 10];
    g_registerArray [3] = g_digits [g_numberToDisplay % 10];
  }
  else
  {
    g_registerArray [0] = g_digits [g_numberToDisplay / 1000];
    g_registerArray [1] = g_digits [(g_numberToDisplay % 1000) / 100];
    g_registerArray [2] = g_digits [(g_numberToDisplay % 100) / 10];
    g_registerArray [3] = g_digits [g_numberToDisplay % 10];
  }
  
  // Blink the LED on pin 13 every seconds
  digitalWrite(ledPin,HIGH);
  digitalWrite(ledpinA, HIGH);
  delay(500);
  digitalWrite(ledPin,LOW);
  digitalWrite(ledpinA, LOW);
  delay(500);

} // end of loop
