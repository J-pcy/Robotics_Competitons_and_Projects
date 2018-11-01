//xj_qian
#include <Servo.h>
Servo servo1;
Servo servo2;
int xj_pin1 = 5;
int xj_pin2 = 6;
int xj_pin3 = 11;

int xj_val1;
int xj_val2;
int xj_val3;

void qian()
{
  servo1.write(0);
  servo2.write(180);
}

void qianyp()
{
  servo1.write(90);
  servo2.write(180);
}

void qianzp()
{
  servo1.write(0);
  servo2.write(90);
}

void youz()
{
  servo1.write(180);
  servo2.write(180);
}

void zuoz()
{
  servo1.write(0);
  servo2.write(0);
}

void tingzhi()
{
  servo1.write(90);
  servo2.write(90);  
}

void setup()
{
  servo1.attach(9);
  servo2.attach(10);
  pinMode(xj_pin1,INPUT);
  pinMode(xj_pin2,INPUT);
  pinMode(xj_pin3,INPUT);
}

void loop()
{
  xj_val1=digitalRead(xj_pin1);
  xj_val2=digitalRead(xj_pin2);
  xj_val3=digitalRead(xj_pin3);
  
//  qian();
//  qianzp();
//  qianyp();  
//  zuoz();
//  youz();
//  tingzhi();
  if((xj_val1&xj_val2)|(!xj_val3))
  {
    qian();
  }
  
  if((!xj_val1)|xj_val3)
  {
    qianyp();
  }
  
  if((!xj_val2)|xj_val3)
  {
    qianzp();
  }

}
