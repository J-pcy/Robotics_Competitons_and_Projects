#include <Servo.h>

#define yellow 0
#define white 1
#define red 2
#define black 3
#define blue 4

Servo servo1;
Servo servo2;

int xj_pin1 = 5;
int xj_pin2 = 6;
int xj_pin3 = 11;
int xj_pin4 = 9;
int xj_pin5 = 10;

int A=0,B=1,C=3,D=4;

int xj_val1;
int xj_val2;
int xj_val3;
int xj_val4;
int xj_val5;

void qian()
{
  servo1.write(0);
  servo2.write(180);
}

void qianyp()
{
  servo1.write(172);
  servo2.write(180);
}

void qianzp()
{
  servo1.write(0);
  servo2.write(172);
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

void hou()
{
  servo1.write(180);
  servo2.write(0);  
}

void houzp()
{
  servo1.write(172);
  servo2.write(0);
}

void houyp()
{
  servo1.write(180);
  servo2.write(172);
}

void tingzhi()
{
  servo1.write(172);
  servo2.write(172);  
}

void banyun_init()
{
  xj_val1=digitalRead(xj_pin1);
  xj_val2=digitalRead(xj_pin2);
  xj_val3=digitalRead(xj_pin3);
  xj_val4=digitalRead(xj_pin4);
  xj_val5=digitalRead(xj_pin5);    
}

void chufa()
{
  qian();
  delay(1000);
}

void xj_zhix()
{
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

void zuoz_45()
{
  zuoz();
  delay(280);
}

void zuoz_90()
{
  zuoz();
  delay(560);
}

void zuoz_135()
{
  zuoz();
  delay(840);
}

void youz_45()
{
  youz();
  delay(280);
}

void youz_90()
{
  youz();
  delay(560);
}

void youz_135()
{
  youz();
  delay(840);
}

void zhuan_180()
{
  servo1.write(177);
  servo2.write(177);
  delay(1700);
}

void zhongdian()
{
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5); 
    
    if(xj_val4|xj_val5)
    {
       xj_zhix();
    }
    else 
    {
      qian();
      delay(400);
      break;
    }
  }
}

void banyun()
{
    while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5); 
    
    if(xj_val1&xj_val2&xj_val3)
    {
       tingzhi();
       break;
    }
    else 
    {
      xj_zhix();
    }
  }
}

void setup()
{
  servo1.attach(3);
  servo2.attach(4);
  pinMode(xj_pin1,INPUT);
  pinMode(xj_pin2,INPUT);
  pinMode(xj_pin3,INPUT);
  pinMode(xj_pin4,INPUT);
  pinMode(xj_pin5,INPUT);
  
  if (A=0)
  {
    
  }
}

void loop()
{
  
}
