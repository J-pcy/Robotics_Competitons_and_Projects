int16_t az, gy;
void setup()
{
  Serial.begin(9600);//设置波特率
}

void loop()
{
  az = analogRead(0);
  gy = analogRead(1);
  Serial.print("a/g:\t");
  Serial.print(az); Serial.print("\t");
  Serial.println(gy);
}
