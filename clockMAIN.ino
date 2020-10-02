//*warehouse clock menu test*//
#include "DHT.h"
#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>
#include "pitches.h"
#include <anyrtttl.h>
#include <binrtttl.h>
#define DHT11_PIN A0
#define DHTTYPE DHT11
#define buzzer A2
//#define TIME_24_HOUR false

DHT dht(DHT11_PIN, DHTTYPE);

const int g_pinData=4; //slave input 74595
const int g_pinLatch=3; //latch 74595
const int g_pinClk=2; //clock 74595
const int ledPinA=10; //semi column blink
const int ledPinB=9;
const int ledPinC=A1; //am pm light
const int button1=6; //set button
const int button2=5; //plus button
const int button3=7; //minus button
const int button4=8; //extra display button

const char * mario = "smb:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";
const char * wonder = "Wonderboy:d=4,o=5,b=225:f6,d6,f6,8d6,f6,32p,8f6,d6,f6,d6,e6,c6,e6,8c6,e6,32p,8e6,c6,e6,c6";
const char * starwars = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
const char * simpsons = "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
const char * indiana = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
const char * bubblebobble = "BubbleBo:d=4,o=6,b=265:e,f#,g#,a,g#,f#.,8e,g#,f#,e.,8d,f#,e,8d,c#,2e,p,8c#,8b5,a5,b5,c#,d,8b5,c#,d.,8p,8e,e,f#,8g#,f#,e,8p,e,f#,g#,8a.,16a5,g#,f#.,8e,8g#.,16a5,f#,e.,8d,8f#.,16a5,e,8d,c#,2e,p,8c#,8b5,8a5.,16e5,b5,c#,d,8b5,c#,e.,8p,8e,e,f#,8g#,f#,e";
const char * imperial = "Imperial:d=4,o=5,b=112:8g,16p,8g,16p,8g,16p,16d#.,32p,32a#.,8g,16p,16d#.,32p,32a#.,g,8p,32p,8d6,16p,8d6,16p,8d6,16p,16d#.6,32p,32a#.,8f#,16p,16d#.,32p,32a#.,g,8p,32p,8g6,16p,16g.,32p,32g.,8g6,16p,16f#.6,32p,32f.6,32e.6,32d#.6,16e6,8p,16g#,32p,8c#6,16p,16c.6,32p,32b.,32a#.,32a.,16a#,8p,16d#,32p,8f#,16p,16d#.,32p,32g.,8a#,16p,16g.,32p,32a#.,d6,8p,32p,8g6,16p,16g.,32p,32g.,8g6,16p,16f#.6,32p,32f.6,32e.6,32d#.6,16e6,8p,16g#,32p,8c#6,16p,16c.6,32p,32b.,32a#.,32a.,16a#,8p,16d#,32p,8f#,16p,16d#.,32p,32g.,8g,16p,16d#.,32p,32a#.";
const char * sonic = "SonicThe:d=4,o=5,b=125:16c7,32p,8a6,32p,16c7,32p,8b6,32p,16c7,32p,8b6,32p,g6,p,16a6,32p,16e7,32p,8d7,32p,16c7,32p,8b6,32p,16c7,32p,8b6,32p,g6,p,16c7,32p,8a6,32p,16c7,32p,8b6,32p,16c7,32p,8b6,32p,g6,p,16a6,32p,8f6,32p,16a6,32p,8g6,32p,16a6,32p,8g6,32p,c6";
const char * teenagemutant = "TeenageM:d=4,o=6,b=200:8g,8c,8a,8c,8g,8c,8a,8c,8g,8c,8a,g,8c,8a,8c,8a#,8d#,8c7,8d#,8a#,8d#,8c7,8d#,8a#,8d#,8c7,a#,8d#,8c7,8d#,8c7,8f,8f7,8f,8d#7,8f,8f7,8f,8d#7,8g#,8f7,d#7,8g#,8f7,8g#,16c7,16c,16c7,16c,16c7,16c,16c7,16c,16a#,16a#5,8p,16c7,16c,p,16c7,16c,16c7,16c,16c7,16c";
const char * pinky = "PinkyAnd:d=16,o=5,b=45:32p,32c,f,32e,f,32g#,8e.,32c,f,32e,f,32g#,8e.,32c,f.,f,32g#,32b,8b,32b,c6,32a#,g#.,8g.,a#.,a#,32c#6,8a.,32f,a#,32f,a#,32c#6,8a.,32c,c,c.,32c,c,32c,d,32e,f.,d#.,c#.,c.,f.,d#.,c#.,c.,4f";

RTC_DS1307 RTC; //define RTC variables

byte g_digits[10]; //definitions of the 7-bit values for displaying g_digits
byte g_digit2[10];
byte g_digit3[10];

int g_numberToDisplay=0; //default number to display

const int g_registers=4; //number of shift registers in use

byte g_registerArray[g_registers]; //array of numbers to pass to shift registers

int hour,minute,sec,day,month,year,disp=0;

unsigned long previousMillis = 0;
const long interval = 1000;

//*******time date set variables**********//

int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int Ahourupg;
int Aminuteupg;
int Astateupg;
int Astate;
int alarm_hr;
int alarm_min;
int menu =0;
int alarm_state;
int toneSelect;

//****************************************//

void setup()
{
    //I2C RTC setup
    Wire.begin();
    RTC.begin();
    dht.begin();
    alarm_hr = EEPROM.read(0);
    alarm_min = EEPROM.read(1);
    alarm_state = EEPROM.read(2);
    

    /*  Only set the time on compile if the RTC is not running...
    This is used to set the current time from the computer clock
    */
   if ( !RTC.isrunning()) {
       Serial.println("RTC is NOT running!");
       // following line sets the RTC to the date & time this sketch was compiled
       RTC.adjust(DateTime(__DATE__, __TIME__));
       //RTC.adjust(DateTime(2014, 1, 21, 2, 0, 0));
       }
       pinMode(g_pinLatch,OUTPUT);   //define 74595 pin OUTPUT
       pinMode(g_pinClk,OUTPUT);     //define 74595 pin OUTPUT
       pinMode(g_pinData,OUTPUT);    //define 74595 pin OUTPUT
       pinMode(ledPinA,OUTPUT);      //define LED pin OUTPUT
       pinMode(ledPinB,OUTPUT);      //define LED pin OUTPUT
       pinMode(ledPinC,OUTPUT);
       pinMode(button1,INPUT_PULLUP);    //define button pin input
       pinMode(button2,INPUT_PULLUP);
       pinMode(button3,INPUT_PULLUP);
       pinMode(button4,INPUT_PULLUP);
       pinMode(buzzer, OUTPUT);
       
       Serial.begin(57600);           //optional, serial monitoring for debugging
       
       //setup display from 0 to 9 on 7-segment g_digits
       // a - top bar
       // b - top right
       // c - bottom right
       // d - bottom bar
       // e - bottom left
       // f - top right
       // g - middle bar
       
       int a=1,b=2,c=4,d=8,e=16,f=32,g=64;
       
       g_digits[0]=8+4+2+64+32+1;
       g_digits[1]=4+2;
       g_digits[2]=8+4+16+32+64;
       g_digits[3]=8+4+16+2+64;
       g_digits[4]=1+16+4+2;
       g_digits[5]=8+1+16+2+64;
       g_digits[6]=8+1+16+2+64+32;
       g_digits[7]=8+4+2;
       g_digits[8]=8+4+2+64+32+1+16;
       g_digits[9]=8+4+2+1+16+64;
       g_digits[90]=1+16+4+8;       // Degree dot
       g_digits[91]=1+32+64+8;      // Capital C
       g_digits[92]=16+64;          //  r, 80
       g_digits[93]=32+16+1+2;      //  h, 116
       g_digits[99]=0;
       g_digit2[0]=32+16+2; //n
       g_digit2[1]=32+16+1+2; //h
       g_digit2[2]=4+2+64+32+16; //d
       g_digit2[3]=32+1+8+4+2; //N
       g_digit2[4]=1+16+4+2+64; //y
       g_digit2[5]=32+1+8+4+2+16; //A
       g_digit2[6]=16+32; //r
       g_digit2[7]=16+2; //backward r
       g_digit2[8]=8+4+2; //backward large r
       g_digit2[9]=32+1+8+16; //F
       g_digit2[99]=0;
       g_digit3[0]=1+16+32+64;
       g_digit3[1]=8+1+16+2+64;
       g_digit3[2]=0;
       g_digit3[3]=0;
       g_digit3[4]=0;
       g_digit3[5]=0;
       g_digit3[6]=0;
       g_digit3[7]=0;
       g_digit3[8]=0;
       g_digit3[9]=0;
       g_digit3[99]=0;
       
       
       //blink test
       for (int i=0;i<5;i++){
           g_registerArray[0]=g_digits[8];
           g_registerArray[1]=g_digits[8];
           g_registerArray[2]=g_digits[8];
           g_registerArray[3]=g_digits[8];
           sendSerialData(g_registers,g_registerArray);
           delay(100);
           g_registerArray[0]=g_digits[99];
           g_registerArray[1]=g_digits[99];
           g_registerArray[2]=g_digits[99];
           g_registerArray[3]=g_digits[99];
           sendSerialData(g_registers,g_registerArray);
           delay(100);
       }
} // End of Setup() //

// Simple function to send serial data to one or more shift registers by iterating backwards through an array.
// Although g_registers exists, they may not all be being used, hence the input parameter.

void sendSerialData (byte registerCount, byte *pValueArray) {
  // Signal to the 595s to listen for data
  digitalWrite (g_pinLatch, LOW);

  for (byte reg = registerCount; reg > 0; reg--)
  {
    byte value = pValueArray [reg - 1];

    for (byte bitMask = 128; bitMask > 0; bitMask >>= 1)
    {
      digitalWrite (g_pinClk, LOW);
      digitalWrite (g_pinData, value & bitMask ? HIGH : LOW);
      digitalWrite (g_pinClk, HIGH);
    }
  }
  // Signal to the 595s that I'm done sending
  digitalWrite (g_pinLatch, HIGH);
}  // sendSerialData

void loop()
{
    int hour,minute,sec,day,month,year,disp,alarm_hr,alarm_min,alarm_state = 0;

    DateTime now = RTC.now();   //get current time and date from RTC
    hour = now.hour();          // break down time to hour
    minute = now.minute();      // break down time to minute
    sec = now.second();         // break down time to seconds
    day = now.day();            // break down date to day
    month = now.month();        // break down date to month
    year = now.year();          // break down date to year
    alarm_hr = EEPROM.read(0);
    alarm_min = EEPROM.read(1);
    alarm_state = EEPROM.read(2);
    
    if(now.hour() >= 12 && now.hour() <=23){
      digitalWrite(ledPinC, LOW);
    }else
    if(now.hour() >= 0 && now.hour() <=11){
      digitalWrite(ledPinC, HIGH);
    }
    if(now.hour() == alarm_hr && now.minute() == alarm_min && alarm_state == 1){
      AlarmActive();
      }
      else{
          noTone(buzzer);
          alarm_state=0; 
        }
      

/*****new menu****/
if(digitalRead(button4))
{
  DisplayDate();
  delay(500);
  DisplayTemp();
  delay(500);
  DisplayAlarmTime();
  delay(500);
}
else if(digitalRead(button1))
{
    menu=menu+1;
}
if(menu==0)
{
    DisplayTime();
}
if(menu==1)
{
    DisplaySetHour();
}
if(menu==2)
{
    DisplaySetMinute();
}
if(menu==3)
{
    DisplaySetDay();
}
if(menu==4)
{
    DisplaySetMonth();
}
if(menu==5)
{
    DisplaySetYear();
}
if(menu==6)
{
  DisplaySetAlarmHr();
}
if(menu==7)
{
  DisplaySetAlarmMin();
}
if(menu==8)
{
  DisplayAlarmState();
}
if(menu==9)
{
  DisplayMelodySelect();
}
if(menu==10)
{
    StoreAgg();
    delay(500);
    menu=0;
}
delay(300);
}

void DisplayDate()
{
    //push the day and month to four digits
    
    g_registerArray [0] = g_digits [day / 10];
    g_registerArray [1] = g_digits [day % 10];
    g_registerArray [2] = g_digits [(month % 100)/10];
    g_registerArray [3] = g_digits [month % 10];

    sendSerialData (g_registers, g_registerArray);

    delay(1000);

    // push year to the four digits

    g_registerArray [0] = g_digits [year / 1000];
    g_registerArray [1] = g_digits [(year % 1000)/100];
    g_registerArray [2] = g_digits [(year % 100)/10];
    g_registerArray [3] = g_digits [year % 10];

    sendSerialData (g_registers, g_registerArray);

    delay(1000);
}

void DisplayTemp()
{
  delay(200);
  int temp = dht.readTemperature();
  int humid = dht.readHumidity();
  
    g_registerArray [0] = g_digits [temp / 10];
    g_registerArray [1] = g_digits [temp % 10];
    g_registerArray [2] = g_digits [90];
    g_registerArray [3] = g_digits [91];

    sendSerialData (g_registers, g_registerArray);

    delay(1000);

    g_registerArray[0]=g_digits[humid/10];
    g_registerArray[1]=g_digits[humid%10];
    g_registerArray[2]=g_digit2[1];
    g_registerArray[3]=g_digit2[6];
    sendSerialData(g_registers,g_registerArray);
    delay (1000);

}
void DisplayTime()
{
    //Time display
    // push the hour 2 digits to the left by multiplying by 100
    DateTime now = RTC.now();   //get current time and date from RTC
    hour = now.hour();          // break down time to hour
    minute = now.minute();      // break down time to minute
    sec = now.second();         // break down time to seconds
    day = now.day();            // break down date to day
    month = now.month();        // break down date to month
    year = now.year();          // break down date to year
    hourupg=hour;
    minupg=minute;
    dayupg=day;
    monthupg=month;
    yearupg=year;
    
    if(now.hour() > 12){
      hour -= 12;
    }
    else
    if(now.hour() == 0){
      hour = 12;
    }
    


    
    disp=(hour*100)+minute;
    g_numberToDisplay=disp;

    // push the number to the four digits
    if (g_numberToDisplay<10)
    {
        g_registerArray[0]=g_digits[0];
        g_registerArray[1]=g_digits[0];
        g_registerArray[2]=g_digits[0];
        g_registerArray[3]=g_digits[g_numberToDisplay];
        }
        else if(g_numberToDisplay<100)
        {
            g_registerArray[0]=g_digits[0];
            g_registerArray[1]=g_digits[0];
            g_registerArray[2]=g_digits[g_numberToDisplay/10];
            g_registerArray[3]=g_digits[g_numberToDisplay%10];
            }
            else if(g_numberToDisplay<1000)
            {
                g_registerArray[0]=g_digits[0];
                g_registerArray[1]=g_digits[g_numberToDisplay/100];
                g_registerArray[2]=g_digits[(g_numberToDisplay%100)/10];
                g_registerArray[3]=g_digits[g_numberToDisplay%10];
                }
                else
                {
                    g_registerArray[0]=g_digits[g_numberToDisplay/1000];
                    g_registerArray[1]=g_digits[(g_numberToDisplay%1000)/100];
                    g_registerArray[2]=g_digits[(g_numberToDisplay%100)/10];
                    g_registerArray[3]=g_digits[g_numberToDisplay%10];
                    }
                    sendSerialData(g_registers,g_registerArray);
                    delay(100);
                      if(now.hour()>=12&&now.hour()<=23){
                        digitalWrite(ledPinA,HIGH);
                        digitalWrite(ledPinB,HIGH);
                        delay(300);
                        digitalWrite(ledPinA,LOW);
                        digitalWrite(ledPinB,LOW);
                        delay(300);
                        }else
                        if(now.hour()>=0&&now.hour()<=11){
                          digitalWrite(ledPinA,LOW);
                          digitalWrite(ledPinB,HIGH);
                          delay(300);
                          digitalWrite(ledPinA,LOW);
                          digitalWrite(ledPinB,LOW);
                          delay(300);
                        }
}

void DisplaySetHour()
{
    if(digitalRead(button2)==HIGH)
    {
        if(hourupg==23)
        {
            hourupg=0;
        }
        else
        {
            hourupg=hourupg+1;
        }
    }
    if(digitalRead(button3)==HIGH)
    {
        if(hourupg==0)
        {
            hourupg=23;
        }
        else
        {
            hourupg=hourupg-1;
        }
    }
    g_registerArray[0]=g_digits[hourupg/10];
    g_registerArray[1]=g_digits[hourupg%10];
    g_registerArray[2]=g_digit2[1];
    g_registerArray[3]=g_digit2[6];
    sendSerialData(g_registers,g_registerArray);
    delay(200);
}
void DisplaySetMinute()
{
    if(digitalRead(button2)==HIGH)
    {
        if(minupg==59)
        {
            minupg=0;
        }
        else
        {
            minupg=minupg+1;
        }
    }
    if(digitalRead(button3)==HIGH)
    {
        if(minupg==0)
        {
            minupg=59;
        }
        else
        {
            minupg=minupg-1;
        }
    }
    g_registerArray[0]=g_digit2[0];
    g_registerArray[1]=g_digit2[7];
    g_registerArray[2]=g_digits[minupg/10];
    g_registerArray[3]=g_digits[minupg%10];
    sendSerialData(g_registers,g_registerArray);
    delay(200);
}
void DisplaySetDay(){
  if(digitalRead(button2)==HIGH)
  {
    if(dayupg==31){
      dayupg=1;
      }else{
        dayupg=dayupg+1;
        }
        }
        if(digitalRead(button3)==HIGH)
        {
          if(dayupg==1){
            dayupg=31;
            }else{
              dayupg=dayupg-1;
              }
              }
              g_registerArray[0]=g_digits[dayupg/10];
              g_registerArray[1]=g_digits[dayupg%10];
              g_registerArray[2]=g_digit2[2];
              g_registerArray[3]=g_digit2[4];
              sendSerialData(g_registers,g_registerArray);
              delay(200);
              }
void DisplaySetMonth()
{
  if(digitalRead(button2)==HIGH)
  {
    if(monthupg==12)
    {
      monthupg==1;
      }
      else
      {
        monthupg=monthupg+1;
        }
        }
        if(digitalRead(button3)==HIGH)
        {
          if(monthupg==1)
          {
            monthupg=12;
            }
            else
            {
              monthupg=monthupg-1;
              }
              }
              g_registerArray[0]=g_digit2[3];
              g_registerArray[1]=g_digit2[8];
              g_registerArray[2]=g_digits[monthupg/10];
              g_registerArray[3]=g_digits[monthupg%10];
              sendSerialData(g_registers,g_registerArray);
              delay(200);
              }
void DisplaySetYear()
{ 
    if(digitalRead(button2)==HIGH)
    {
        yearupg = yearupg+1;
    }
    if(digitalRead(button3)==HIGH)
    {
        yearupg = yearupg-1;
    }
    g_registerArray[0]=g_digits[year/1000];
    g_registerArray[1]=g_digits[(year%1000)/100];
    g_registerArray[2]=g_digits[(yearupg%100)/10];
    g_registerArray[3]=g_digits[yearupg%10];
    sendSerialData(g_registers,g_registerArray);
    delay(200);
}
void DisplaySetAlarmHr()
{
  if(digitalRead(button2)==HIGH)
  {
    if(Ahourupg==23)
        {
            Ahourupg=0;
        }
        else
        {
            Ahourupg=Ahourupg+1;
        }
    }
    if(digitalRead(button3)==HIGH)
    {
        if(Ahourupg==0)
        {
            Ahourupg=23;
        }
        else
        {
            Ahourupg=Ahourupg-1;
        }
    }
    EEPROM.write(0,Ahourupg);
    g_registerArray[0]=g_digits[Ahourupg/10];
    g_registerArray[1]=g_digits[Ahourupg%10];
    g_registerArray[2]=g_digit2[1];
    g_registerArray[3]=g_digit2[6];
    sendSerialData(g_registers,g_registerArray);
    delay(200);
}
void DisplaySetAlarmMin()
{
  if(digitalRead(button2)==HIGH)
  {
    if(Aminuteupg==59)
        {
            Aminuteupg=0;
        }
        else
        {
            Aminuteupg=Aminuteupg+1;
        }
    }
    if(digitalRead(button3)==HIGH)
    {
        if(Aminuteupg==0)
        {
            Aminuteupg=59;
        }
        else
        {
            Aminuteupg=Aminuteupg-1;
        }
    }
    EEPROM.write(1,Aminuteupg);
    g_registerArray[0]=g_digit2[0];
    g_registerArray[1]=g_digit2[7];
    g_registerArray[2]=g_digits[Aminuteupg/10];
    g_registerArray[3]=g_digits[Aminuteupg%10];
    sendSerialData(g_registers,g_registerArray);
    delay(200);
}
void DisplayAlarmState()
{
  if(digitalRead(button2)==HIGH)
  {
    if(Astateupg==1){
      Astateupg=0;
    }else{
      Astateupg = Astateupg+1;
      }
      }
      if(digitalRead(button3)==HIGH)
      {
        if(Astateupg==0){
          Astateupg=1;
          }else{
            Astateupg = Astateupg-1;
            }
            }
            EEPROM.write(2,Astateupg);
            Astate=Astateupg;
            if(Astate==1){
              g_registerArray[0]=g_digit2[5];
              g_registerArray[1]=g_digit2[99];
              g_registerArray[2]=g_digits[0];
              g_registerArray[3]=g_digit2[3];
              sendSerialData(g_registers,g_registerArray);
              delay(200);
            }else
            if(Astate==0){
              g_registerArray[0] = g_digit2[5];
              g_registerArray[1] = g_digit2[99];
              g_registerArray[2] = g_digits[0];
              g_registerArray[3] = g_digit2[9];
              sendSerialData(g_registers, g_registerArray);
              delay(200);
            }
}
void DisplayMelodySelect()
{
   if(digitalRead(button2)==HIGH)
  {
    if(toneSelect==3){
      toneSelect=0;
    }else{
      toneSelect = toneSelect+1;
      }
      }
      if(digitalRead(button3)==HIGH)
      {
        if(toneSelect==0){
          toneSelect=3;
          }else{
            toneSelect = toneSelect-1;
            }
            }
            EEPROM.write(3,toneSelect);
              g_registerArray[0]=g_digit3[0];
              g_registerArray[1]=g_digit3[1];
              g_registerArray[2]=g_digits[toneSelect/10];
              g_registerArray[3]=g_digits[toneSelect%10];
              sendSerialData(g_registers,g_registerArray);
              delay(200);
}
void StoreAgg()
{
    RTC.adjust(DateTime(yearupg,monthupg,dayupg,hourupg,minupg,0));
    delay(200);
    g_registerArray[0]=g_digits[8];
    g_registerArray[1]=g_digits[8];
    g_registerArray[2]=g_digits[8];
    g_registerArray[3]=g_digits[8];
    sendSerialData(g_registers,g_registerArray);
    delay(200);
    g_registerArray[0]=g_digits[99];
    g_registerArray[1]=g_digits[99];
    g_registerArray[2]=g_digits[99];
    g_registerArray[3]=g_digits[99];
    sendSerialData(g_registers,g_registerArray);
    delay(200);
}
void AlarmActive()
{
  if(EEPROM.read(3)==0){
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval)
    {
      anyrtttl::nonblocking::begin(buzzer, pinky);
      if(currentMillis - previousMillis >= 2*interval)
      {
        previousMillis = currentMillis;
      }
    }
    else
    {
      anyrtttl::nonblocking::stop();
    }
 }
}
void DisplayAlarmTime()
{
 Ahourupg = EEPROM.read(0);
 Aminuteupg = EEPROM.read(1);
 Astateupg = EEPROM.read(2);

 g_registerArray [0] = g_digits [Ahourupg /10];
 g_registerArray [1] = g_digits [Ahourupg %10];
 g_registerArray [2] = g_digits [Aminuteupg /10];
 g_registerArray [3] = g_digits [Aminuteupg %10];
 sendSerialData(g_registers,g_registerArray);
 delay(1000);
 g_registerArray [0] = g_digit2 [5];
 g_registerArray [1] = g_digit2 [99];
 g_registerArray [2] = g_digits [Astateupg /10];
 g_registerArray [3] = g_digits [Astateupg % 10];
 sendSerialData(g_registers,g_registerArray);
 delay(1000);
}