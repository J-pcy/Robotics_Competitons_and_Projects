int pin = 9;
int val;
void setup()
{
//   pinMode(ledPin,OUTPUT);
   pinMode(pin,INPUT);
   Serial.begin(9600);
}

void loop()
{
  val=digitalRead(pin);
  Serial.println(val);
}
