#include <Servo.h>
#define servo1 3
//#define servo2 4
#define servo3 5

//Servo servo1;
Servo servo2;
//Servo servo3;

void setup()
{
  pinMode(servo1,OUTPUT);
//  servo1.attach(3);
//  pinMode(servo2,OUTPUT);
  servo2.attach(4);
//  pinMode(servo3,OUTPUT);
//  servo3.attach(5);
}

void loop()
{
  digitalWrite(servo1,0);
//  servo1.write(85);
//  delay(1000);
//  servo1.write(20);
//  delay(1000);

//  digitalWrite(servo2,0);
  servo2.write(0);
  delay(2000);
  servo2.write(90);
  delay(2000);
  
  digitalWrite(servo3,0);
//  servo3.write(60);
//  delay(1000);
//  servo3.write(120);
//  delay(1000);
  
  
}



