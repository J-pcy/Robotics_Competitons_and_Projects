#include <Servo.h>
Servo servo1;
Servo servo2;

void setup()
{
  Serial.begin(115200);
  Serial.println(":-]");
  
  servo1.attach(3);
  servo2.attach(4);
}

void loop()
{
  if(Serial.available())
  {
    char val = Serial.read();
    if(val == '.')
    {
      Serial.println(' ');
      Serial.println("bye");
      Serial.end();
    }
    if(val == 'a')
    {
      servo1.write(90);
      servo2.write(90);
    }
    if(val == 'b')
    {
       servo1.write(0);
       servo2.write(180);
    }
    if(val == 'c')
    {
       servo1.write(180);
       servo2.write(0);
    }
  }
  delay(100);
}
