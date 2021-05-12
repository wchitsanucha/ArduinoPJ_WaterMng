#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "RTClib.h"

#define uS_to_S 1000000
#define sleepTime 300

RTC_DS1307 RTC;     // Setup an instance of DS1307 naming it RTC

const char ssid[] = "SIRISAKH24";         //  your network SSID (name)
const char pass[] = "0842145431";         // your network password

// NTP Servers:
//static const char ntpServerName[] = "us.pool.ntp.org";
static const char ntpServerName[] = "time.nist.gov";
//static const char ntpServerName[] = "time-a.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-b.timefreq.bldrdoc.gov";
//static const char ntpServerName[] = "time-c.timefreq.bldrdoc.gov";

const int timeZone = +7;

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
const int wifi_counter_set = 20;

int pump1_state = 1;
int pump2_state = 1;

//Input pin
const int WS1 = D5;
const int WS2 = D6;

//Output pin
const int PUMP_1 = D3;
const int PUMP_2 = D4;
const int PowSrcTrigger = D7;

const int setHour = 7;
const int setMinute = 30;

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);
void updateRTC();

void setup()
{
  digitalWrite(PUMP_1, 1);  //Turn off relay of PUMP1
  digitalWrite(PUMP_2, 1);  //Turn off relay of PUMP2
  pinMode(PUMP_1, OUTPUT);  
  pinMode(PUMP_2, OUTPUT);  
  delay(4000);
  
  pinMode(WS1, INPUT);
  pinMode(WS2, INPUT);  
  pinMode(PowSrcTrigger, OUTPUT);
    
  Serial.begin(9600);
  ESP.wdtEnable(WDTO_8S);
  Serial.println("Reset");

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  int wifi_counter = 0;

  while ((WiFi.status() != WL_CONNECTED) && wifi_counter < wifi_counter_set) {
    delay(500);
    Serial.print(".");
    wifi_counter++;
  }
  
  RTC.begin();
  
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print("IP number assigned by DHCP is ");
    Serial.println(WiFi.localIP());
    Serial.println("Starting UDP");
    Udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(Udp.localPort());
    Serial.println("waiting for sync");
    setSyncProvider(getNtpTime);
    setSyncInterval(300);
  
    updateRTC();
  }
  else
  {
    Serial.println("Wifi not connect");
  }
  
}

void loop()
{
  Serial.print("PUMP1 status : ");
  Serial.println(pump1_state);
  Serial.print("PUMP2 status : ");
  Serial.println(pump2_state);
    
  if(time_check() && pump1_state && pump2_state)
  {
    Serial.println("Time is match with setting time.");
    
    digitalWrite(PowSrcTrigger, HIGH);    //Turn on to rain sensor
    delay(1000);
    
    if(digitalRead(WS1) == 1 && digitalRead(WS2) == 1)
    {
      Serial.println("The water is empty.");
      Serial.println("All pump is ON !!!");
      water_pump(PUMP_1, 1);
      water_pump(PUMP_2, 1);
      while(pump1_state || pump2_state)
      {
        sensor_check();
        Serial.println("Filling up with water");
        delay(1000);
      }
      Serial.println("The water is full, All pump is OFF.");
    }
    else
    {
      Serial.println("The water was full.");
      Serial.println("All pump is OFF !!!");
      water_pump(PUMP_1, 0);
      water_pump(PUMP_2, 0);
    }

    digitalWrite(PowSrcTrigger, LOW);     //Turn off to rain sensor
  }
  else
  {
    Serial.println("Out of service time.");
    Serial.println("All pump is OFF !!!");
    water_pump(PUMP_1, 0);
    water_pump(PUMP_2, 0);
  }

  digitalWrite(PUMP_1, 1);  //Turn off relay of PUMP1
  digitalWrite(PUMP_2, 1);  //Turn off relay of PUMP2
  
  Serial.println("Start deep sleep 5 minutes.");
  ESP.deepSleep(uS_to_S*sleepTime);
}

int time_check()
{
  if(RTC.isrunning())
  {
    Serial.println("RTC ---");
    DateTime now = RTC.now();
    
    Serial.print(now.hour());
    Serial.print(':');
    Serial.println(now.minute());
    
    if( (now.hour() == setHour && now.minute() >= setMinute) ||
        (now.hour() > setHour) )
    {
      Serial.println("Alarm !!!");
      return 1;
    }
    else
    {
      Serial.println("Not alarm !!!");
      return 0;
    }
  }
  else
  {
    Serial.println("NTP ---");
    Serial.print(hour());
    Serial.print(":");
    Serial.println(minute());
    
    if( (hour() == setHour && minute() >= setMinute) ||
        (hour() > setHour) )
    {
      Serial.println("Alarm !!!");
      return 1;
    }
    else
    {
      Serial.println("Not alarm !!!");
      return 0;
    }
    
  }
}

void sensor_check()
{
  digitalWrite(PowSrcTrigger, HIGH);    //Turn on to rain sensor
  delay(500);
  
  if(digitalRead(WS1) == 0)
  {
    water_pump(PUMP_1, 0);
    Serial.println("Pump1 is OFF !!!");
  }
  else
  {
    //Do nothing
  }

  if(digitalRead(WS2) == 0)
  {
    water_pump(PUMP_2, 0);
    Serial.println("Pump2 is OFF !!!");
  }
  else
  {
    //Do nothing
  }
  
  digitalWrite(PowSrcTrigger, LOW);     //Turn off to rain sensor
}

void water_pump(int pump_number, int state)
{
  if(pump_number == PUMP_1)
  {
    digitalWrite(PUMP_1, !state);
    pump1_state = state;
  }
  else if(pump_number == PUMP_2)
  {
    digitalWrite(PUMP_2, !state);
    pump2_state = state;
  }
  else
  {
    //Do nothing
  }
}

void updateRTC()
{   
  if(day()%3 == 0)
  {
    RTC.adjust(DateTime(year(), month(), day(), hour(), minute(), second()));
    Serial.println("RTC Updated");
  }
  else
  {
    Serial.println("Do nothing");
  }
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
