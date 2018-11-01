int val;
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
}
void loop()
{ 
  //Serial.println("OK please input data");
  //Serial.println("data as ADx xxx");
  val=Serial1.read();
  if(val=='W')
  {
    for(int k=0;k<30;k++)
   {
   Serial1.write('W');
   Serial.println("AD0");
   Serial.flush();
   int y=Serial1.read(); 
   Serial.println(y);
   delay(100);
   }
   }
  else if(val=='S')
  {
    delay(1000);
    val=Serial.read();
    Serial.println(val);
    delay(2000);
  }
    else if(val=='A')
  {
    delay(1000);
    val=Serial.read();
    Serial.println(val);
    delay(2000);
  }
    else if(val=='D')
  {
    delay(1000);
    val=Serial.read();
    Serial.println(val);
    delay(2000);
  }
    }

