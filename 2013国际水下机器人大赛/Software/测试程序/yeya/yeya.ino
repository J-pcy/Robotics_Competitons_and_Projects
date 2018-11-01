#define Yeya
int gaodu = 0;

void setup()
{
  Serial.begin(9600);//设置波特率
}

void loop()
{
  gaodu = analogRead(Yeya);
  Serial.println(gaodu);
}




