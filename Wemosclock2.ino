
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_sensor.h>

#define DHTPIN D5
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

const char ssid[] = "Chall WH EXT";  //  your network SSID (name)
const char pass[] = "Challenge123";       // your network password

// NTP Servers:
static const char ntpServerName[] = "ntp.cs.mu.OZ.AU";
//static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

const int timeZone = +10;     // Central European Time
//const int timeZone = -5;  // Eastern Standard Time (USA)
//const int timeZone = -4;  // Eastern Daylight Time (USA)
//const int timeZone = -8;  // Pacific Standard Time (USA)
//const int timeZone = -7;  // Pacific Daylight Time (USA)


WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

const int g_pinData = D1;
const int g_pinCommLatch = D2;
const int g_pinClock = D3;
const int dot1 = D7;
const int dot2 = D8;
int temp = 0;
int humid = 0;

byte g_digits [10];             // Definitions of the 7-bit values for displaying digits
byte g_digits2 [10];
int g_numberToDisplay = 0;      // default number being displayed, 0 
const int g_registers = 4;      // Number of shift registers in use, 4
byte g_registerArray [g_registers]; // Array of numbers to pass to shift registers

void setup()
{
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  delay(250);
  Serial.println("TimeNTP Example");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

 // when the digital clock was displayed

  pinMode (g_pinData, OUTPUT);            //define 74595 pin as output
  pinMode (g_pinCommLatch, OUTPUT);       //define 74595 pin as output
  pinMode (g_pinClock, OUTPUT);           //define 74595 pin as output
  pinMode (dot1, OUTPUT);
  pinMode (dot2, OUTPUT);

int a = 1, b = 2, c = 4, d = 8, e = 16, f = 32, g = 64;

    g_digits [0] = 1 + 2 + 4 + 8 + 16 + 32;
    g_digits [1] = 2 + 4;
    g_digits [2] = 1 + 2 + 64 + 16 + 8;
    g_digits [3] = 1 + 2 + 4 + 8 + 64;
    g_digits [4] = 32 + 64 + 2 + 4;
    g_digits [5] = 1 + 32 + 64 + 4 + 8;
    g_digits [6] = 1 + 32 + 16 + 8 + 4 + 64;
    g_digits [7] = 1 + 2 + 4;
    g_digits [8] = 1 + 2 + 4 + 8 + 16 + 32 + 64;
    g_digits [9] = 8 + 4 + 2 + 1 + 32 + 64;
    g_digits2 [0] = 1 + 2 + 64 + 32;           // Degree dot
    g_digits2 [1] = 1 + 8 + 16 + 32;           // Capital C
    g_digits2 [2] = 64 + 16;                   //  r, 80
    g_digits2 [3] = 4 + 16 + 32 + 64;           //  h, 116
    g_digits [99] = 0;

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
}
time_t prevDisplay = 0;

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
}


void loop()
{
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if(isnan(event.temperature)){
    Serial.println(F("error reading temperature!"));
  }
  else{
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }/*
  dht.humidity().getEvent(&event);
  if(isnan(event.relative_humidity)){
    Serial.println(F("Error reading humidity!"));
  }
  else{
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }*/

   
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      if ((second() == 20) || (second() == 21)){
        digitalDateDisplay();
      }else{
      digitalClockDisplay();
      }
      if((second() == 40) || (second() == 41)){
        digitalTempDisplay();
      }else{
        digitalClockDisplay();
      }/*
      if((second() == 45) || (second() == 46)){
        digitalHumidDisplay();
      }else{
        digitalClockDisplay();
      }*/
    }
  }
}

void digitalClockDisplay()
{
  Serial.print(hour());
  Serial.print(':');
  Serial.print(minute());
  Serial.print(':');
  Serial.print(second());
  Serial.println();
  
  // digital clock display of the time
   g_registerArray [0] = g_digits [hourFormat12() / 10];
   g_registerArray [1] = g_digits [hourFormat12() % 10];
   g_registerArray [2] = g_digits [minute() / 10];
   g_registerArray [3] = g_digits [minute() % 10];

   sendSerialData (g_registers, g_registerArray);
   digitalWrite(dot1, HIGH);
   digitalWrite(dot2, HIGH);
   delay(500);
   digitalWrite(dot1, LOW);
   digitalWrite(dot2, LOW);
   delay(500);
}

void digitalDateDisplay(){

  Serial.print(day());
  Serial.print(':');
  Serial.print(month());
  Serial.print(':');
  Serial.print(year());
  Serial.println();

  g_registerArray [0] = g_digits [day() / 10];
  g_registerArray [1] = g_digits [day() % 10];
  g_registerArray [2] = g_digits [month() / 10];
  g_registerArray [3] = g_digits [month() % 10];

  sendSerialData (g_registers, g_registerArray);
  delay(500);

  digitalWrite(dot1, HIGH);
  digitalWrite(dot2, HIGH);
  delay(500);
  digitalWrite(dot1, LOW);
  digitalWrite(dot2, LOW);
  delay(500);

  g_registerArray [0] = g_digits [year() / 1000];
  g_registerArray [1] = g_digits [(year() % 1000) / 100];
  g_registerArray [2] = g_digits [(year() % 100) / 10];
  g_registerArray [3] = g_digits [year() % 10];

  sendSerialData (g_registers, g_registerArray);
  delay(500);

  digitalWrite(dot1, HIGH);
  digitalWrite(dot2, HIGH);
  delay(500);
  digitalWrite(dot1, LOW);
  digitalWrite(dot2, LOW);
  delay(500);
}

void digitalTempDisplay(){
  
  Serial.print(temp);
  Serial.println();
  
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  temp = event.temperature;
  
  g_registerArray [0] = g_digits [temp / 10];
  g_registerArray [1] = g_digits [temp % 10];
  g_registerArray [2] = g_digits2 [0];
  g_registerArray [3] = g_digits2 [1];

  sendSerialData (g_registers, g_registerArray);

  digitalWrite(dot1, HIGH);
  digitalWrite(dot2, HIGH);
  delay(500);
  digitalWrite(dot1, LOW);
  digitalWrite(dot2, LOW);
  delay(500);
}
/*
void digitalHumidDisplay(){
  Serial.print(humid);
  Serial.println();
  
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  humid = event.relative_humidity;
  
  g_registerArray [0] = g_digits [humid / 10];
  g_registerArray [1] = g_digits [humid % 10];
  g_registerArray [2] = g_digits2 [2];
  g_registerArray [3] = g_digits2 [3];

  sendSerialData (g_registers, g_registerArray);

  digitalWrite(dot1, HIGH);
  digitalWrite(dot2, HIGH);
  delay(500);
  digitalWrite(dot1, LOW);
  digitalWrite(dot2, LOW);
  delay(500);
}*/

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
