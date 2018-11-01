#define hongwai 2
#define jidianqi 1
int data = 0;

void setup()
{
//  Serial.begin(9600);//设置波特率
  pinMode(hongwai,INPUT);
  pinMode(jidianqi,OUTPUT);
}

void loop()
{
  data = digitalRead(hongwai);
//  Serial.println(data);
  digitalWrite(jidianqi,data);
  delay(1);
}




