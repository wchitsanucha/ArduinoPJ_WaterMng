int ledPin = 16;
int Sensor1 = 7;
int Sensor1_Vcc = 4;
int val, i;

void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(Sensor1, INPUT);
  pinMode(Sensor1_Vcc, OUTPUT);
  
  Serial.begin(9600);
  
  for(i=0;i<5;i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(300);
    digitalWrite(ledPin, LOW);
    delay(300);
  }
}
 
void loop() 
{
  digitalWrite(4, HIGH);
  delay(10);
  val = digitalRead(Sensor1);
  digitalWrite(4, LOW);
  
  Serial.print("val = ");
  Serial.println(val);
  
  if (val == 0)
  { 
    digitalWrite(ledPin, LOW);
    Serial.println("Detected");
  }
  else 
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("No water");
  }
  delay(1000);
}
