int Relay1 = 3;
int Relay2 = 5;


void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay1, 1);
  digitalWrite(Relay2, 1);
}

void loop() {

  RelayTest2();

}

void RelayTest1()
{
  digitalWrite(Relay1, 0);
  delay(3000);
  digitalWrite(Relay1, 1);
  delay(1000);

  digitalWrite(Relay2, 0);
  delay(3000);
  digitalWrite(Relay2, 1);
  delay(1000);
}

void RelayTest2()
{
  digitalWrite(Relay1, 0);
  digitalWrite(Relay2, 0);
  delay(3000);
  
  digitalWrite(Relay1, 1);
  digitalWrite(Relay2, 1);
  delay(1000);
}
