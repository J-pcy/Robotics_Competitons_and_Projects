#include <Servo.h>

Servo servo1;
Servo servo2;

int xj_pin1 = 5;
int xj_pin2 = 6;
int xj_pin3 = 11;
int xj_pin4 = 9;
int xj_pin5 = 10;

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

void hou()
{
  servo1.write(180);
  servo2.write(0);  
}

void houzp()
{
  servo1.write(90);
  servo2.write(0);
}

void houyp()
{
  servo1.write(180);
  servo2.write(90);
}

void tingzhi()
{
  servo1.write(90);
  servo2.write(90);  
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

void xj_qian()
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

void xj_hou()
{
  if((xj_val1&xj_val2)|(!xj_val3))
  {
    hou();
  }
  
  if((!xj_val1)|xj_val3)
  {
    houyp();
  }
  
  if((!xj_val2)|xj_val3)
  {
    houzp();
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
       xj_qian();
    }
    else 
    {
      qian();
      delay(400);
      break;
    }
  }
/*  if((!xj_val4)&(!xj_val5))
  {
    qian();
    delay(200);
    tingzhi();
  }*/
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
      xj_qian();
    }
  }
}

void hui_zhong()
{
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5);    
  }
}

void tui_yellow()
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
       qian();
       delay(200);   
    }
    else 
    {
      xj_qian();
      if(!xj_val5)
      {
        qian();
        delay(100);
      }  
    }
  }
}

/*void zhitui()
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
       qian();
       delay(200);
       
    }
    else 
    {
      xj_qian();
      if()  
    }
  }  
}*/

void setup()
{
  servo1.attach(3);
  servo2.attach(4);
  pinMode(xj_pin1,INPUT);
  pinMode(xj_pin2,INPUT);
  pinMode(xj_pin3,INPUT);
  pinMode(xj_pin4,INPUT);
  pinMode(xj_pin5,INPUT);
  
  qian();
  delay(1000);
  
//  while(xj_val4|xj_val5)
//  {
//    xj_qian();
//  }
//  qian();
//  delay(200);
  zhongdian();
  zuoz_90();
  banyun();
  zhuan_180();
  tingzhi();
}

void loop()
{
//  banyun_init();
/*  qian();
  delay(1000);
  while(xj_val4|xj_val5)
  {
    xj_qian();
  }
  qian();
  delay(200);*/

  
//  xj_qian();
//  delay(5000);
//  chufa();
//  xj_qian();
//  zhongdian();
//  zuoz_90();
//  tingzhi();
//  delay(5000);
//  xj_val1=digitalRead(xj_pin1);
//  xj_val2=digitalRead(xj_pin2);
//  xj_val3=digitalRead(xj_pin3);
//  xj_val4=digitalRead(xj_pin4);
//  xj_val5=digitalRead(xj_pin5); 
  
//  qian();
//  delay(800);
//  while(1)
//  {
//    xj_qian();
//  }
//  qian();
//  delay(200);
//  qian();
//  qianzp();
//  qianyp();  
//  zuoz();
//  youz();
//  hou();
//  houzp();
//  houyp();
//  tingzhi();
/*  if((xj_val1&xj_val2)|(!xj_val3))
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
  }*/

}
