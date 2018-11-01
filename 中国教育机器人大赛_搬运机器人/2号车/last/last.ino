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
  servo1.write(80);
  servo2.write(180);
}

void qianzp()
{
  servo1.write(0);
  servo2.write(80);
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

void tingzhi()
{
  servo1.write(80);
  servo2.write(80);  
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
  
  if(!xj_val1)
  {
    qianyp();
  }
  
  if(!xj_val2)
  {
    qianzp();
  }
}

void zuoz_45()
{
  zuoz();
  delay(246);
}

void zuoz_45_banyun()
{
  servo1.write(77);
  servo2.write(77);
  delay(788);
}

void zuoz_90()
{
  zuoz();
  delay(493);
}

void zuoz_90_banyun()
{
  servo1.write(77);
  servo2.write(77);
  delay(1577);
}

void zuoz_135()
{
  zuoz();
  delay(739);
}

void zuoz_135_banyun()
{
  servo1.write(77);
  servo2.write(77);
  delay(2365);
}

void youz_45()
{
  youz();
  delay(249);
}

void youz_45_banyun()
{
  servo1.write(83);
  servo2.write(83);
  delay(667);
}

void youz_90()
{
  youz();
  delay(498);
}

void youz_90_banyun()
{
  servo1.write(83);
  servo2.write(83);
  delay(1334);
}

void youz_135()
{
  youz();
  delay(747);
}

void youz_135_banyun()
{
  servo1.write(83);
  servo2.write(83);
  delay(2220);
}

void zhuan_180()
{
  servo1.write(85);
  servo2.write(85);
  delay(1600);
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
    
    if((!xj_val4)&(!xj_val5))
    {
      qian();
      delay(400);
      break;
    }
    else 
    {
      xj_qian();
    }
  }
}

void banyun_1()        //找到物块
{
  qian();
  delay(100);
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

void banyun_back_1()          //找到物块，并带回
{
  banyun_1();
  zhuan_180();
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5);    
    
    if((!xj_val4)|(!xj_val5))
    {
      qian();
      delay(630);
      break;      
    }
    else
    {
      xj_qian();
    }
  }
}

void banyun_2_A()           //黄，找到存放物块
{
  qian();
  delay(100);
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5); 
    
    if(!xj_val4)
    {
      qian();
      delay(20);
      break;
    }
    else
    {
      xj_qian();
    }
  }
}

void banyun_back_2_A()       //黄，找到存放物块，并带回
{
  banyun_2_A();
  zhuan_180();
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5);    
    
    if((!xj_val4)|(!xj_val5))
    {
      qian();
      delay(630);
      break;      
    }
    else
    {
      xj_qian();
    }
  }
}

void banyun_2_B()             //蓝，找到存放物块
{
  qian();
  delay(100);
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5); 
    
    if(!xj_val5)
    {
      qian();
      delay(20);
      break;
    }
    else
    {
      xj_qian();
      if(xj_val1&xj_val2&xj_val3)
      {
        qian();
        delay(300);
      }
    }
  }
}

void banyun_back_2_B()       //蓝，找到存放物块，并带回
{
  banyun_2_B();
  zhuan_180();
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5);    
    
    if((!xj_val4)|(!xj_val5))
    {
      qian();
      delay(630);
      break;      
    }
    else
    {
      xj_qian();
    }
  }
}

void push_A()        //黄，放到相应点
{
  qian();
  delay(200);
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5);      
    
    if(!xj_val4)
    {
      qian();
      delay(15);
      tingzhi();
      delay(50);
      break;
    }
    else
    {
      xj_qian();
    }
  }
}

void push_B()        //蓝，放到相应点
{
  qian();
  delay(200);
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5);      
    
    if(!xj_val5)
    {
      qian();
      delay(15);
      tingzhi();
      delay(50);
      break;
    }
    else
    {
      xj_qian();
      if(xj_val1&xj_val2&xj_val3)
      {
        qian();
        delay(400);
      }
    }
  }
}

void push_back()     //放到相应点后,回中
{
  hou();
  delay(500);
//  zhuan_180();
  servo1.write(85);
  servo2.write(85);
  delay(1644);
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5);    
    
    if((!xj_val4)|(!xj_val5))
    {
      qian();
      delay(630);
      break;      
    }
    else
    {
      xj_qian();
    }
  }  
}

void gohome()
{
  while(1)
  {
    xj_val1=digitalRead(xj_pin1);
    xj_val2=digitalRead(xj_pin2);
    xj_val3=digitalRead(xj_pin3);
    xj_val4=digitalRead(xj_pin4);
    xj_val5=digitalRead(xj_pin5); 
    
    if((!xj_val4)&(!xj_val5))
    {
      qian();
      delay(1600);
      break;
    }
    else 
    {
      xj_qian();
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
  
  chufa();
  zhongdian();
  
  
    zuoz_90();       
    banyun_back_1();
    zuoz_90_banyun();
    push_A();
    push_back();
    
    zuoz_90();    
    banyun_back_1();
    push_A();
    push_back();
    
    zuoz_135();    
    banyun_back_1();
    zuoz_45_banyun();
    push_B();
    push_back();
    
    youz_135();    
    banyun_back_1();
    youz_90_banyun();
    push_A();
    push_back();
    
    zuoz_45();
    banyun_back_2_B();
    youz_135_banyun();
    push_A();
    push_back();
    
    youz_135();
    banyun_back_2_A();
    zuoz_90_banyun();
    push_B();
    push_back();
    
    zuoz_90();
  
  
  gohome();
  tingzhi();
  
}

void loop()
{
  
}
