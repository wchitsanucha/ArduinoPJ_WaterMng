#define RST 4

unsigned long set_time, chk_time, cal_time;

void setup() {
  pinMode(RST, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Hello World!!!");

  digitalWrite(RST, HIGH);

  set_time = millis();
}

void loop() {

  Serial.print("Set time is :");
  Serial.print(set_time);

  chk_time = millis();
  cal_time = chk_time - set_time;

  Serial.print(" | Cal time is :");
  Serial.println(cal_time);

  if(cal_time >= 15000)
  {
    Serial.println("Run time is over 15 seconds.");
    Serial.println("Shutdown");
    delay(1000);
    digitalWrite(RST, LOW);
  }

}
