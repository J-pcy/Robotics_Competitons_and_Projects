int val=0;
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  val=Serial.read();
  Serial.println(val);
}
