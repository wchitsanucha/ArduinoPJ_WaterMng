const int mainValve = 3;
const int waterSnVcc = 4;
const int waterSn = 7;
const int RST = 5;

#define time_to_rst 180000

int waterSnVal = 0;

unsigned long set_time = 0, chk_time, cal_time;

void setup() {
  Serial.begin(9600);
  pinMode(waterSn, INPUT_PULLUP);
  pinMode(mainValve, OUTPUT);
  pinMode(waterSnVcc, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(mainValve, HIGH);
  digitalWrite(RST, HIGH);

  set_time = millis();
}

void loop() {

  sensor_read();

  value_chk();

  Serial.print("Set time is :");
  Serial.print(set_time);
  
  chk_time = millis();
  cal_time = chk_time - set_time;
  Serial.print(" | Cal time is :");
  Serial.println(cal_time);

  delay(5000);
  
  time_chk();
}

void sensor_read()
{
  digitalWrite(waterSnVcc, HIGH);
  delay(10);
  waterSnVal = digitalRead(waterSn);
  digitalWrite(waterSnVcc, LOW);
    
  Serial.print("val = ");
  Serial.println(waterSnVal);
}

void value_chk()
{
  int state = 0;
  
  if(waterSnVal == 0)
  {
    Serial.println("Full water");
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(mainValve, HIGH);
  }
  else
  {
    Serial.println("Filling water");
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(mainValve, LOW);

    while(state == 0)
    {
      sensor_read();
      if(waterSnVal == 0)
      {
        state = 1;
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(mainValve, HIGH);
        Serial.println("Max water");
      }
      else
      {
        state = 0;
        Serial.println("Filling water [Inner if]");
      }
      delay(2000);
    }
  }
}

void time_chk()
{
  if(cal_time >= time_to_rst)
  {
    Serial.println("Run time is over 15 seconds.");
    Serial.println("Shutdown");
    delay(1000);
    digitalWrite(RST, LOW);
  }
}
