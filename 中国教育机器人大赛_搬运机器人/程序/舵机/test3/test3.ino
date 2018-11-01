#include <Servo.h>
Servo servo1;
Servo servo2;

void setup()
{
  servo1.attach(3);
  servo2.attach(4);
}

void loop()
{
   servo1.write(171);//中心点
   servo2.write(171);
}
