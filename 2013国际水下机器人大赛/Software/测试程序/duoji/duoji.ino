#include <Servo.h>

Servo servo1;

void setup()
{
  servo1.attach(3);
}

void loop()
{
  servo1.write(65);
  delay(3000);
  servo1.write(98);
  delay(3000);
}
