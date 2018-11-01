//驱动
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
  servo1.write(171);
  servo2.write(180);
}

void qianzp()
{
  servo1.write(0);
  servo2.write(171);
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
  servo1.write(171);
  servo2.write(171);  
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
  delay(244);
}

void zuoz_45_banyun()
{
  servo1.write(168);
  servo2.write(168);
  delay(617);
}

void zuoz_90()
{
  zuoz();
  delay(488);
}

void zuoz_90_banyun()
{
  servo1.write(168);
  servo2.write(168);
  delay(1234);
}

void zuoz_135()
{
  zuoz();
  delay(732);
}

void zuoz_135_banyun()
{
  servo1.write(168);
  servo2.write(168);
  delay(1851);
}

void youz_45()
{
  youz();
  delay(244);
}

void youz_45_banyun()
{
  servo1.write(174);
  servo2.write(174);
  delay(740);
}

void youz_90()
{
  youz();
  delay(488);
}

void youz_90_banyun()
{
  servo1.write(174);
  servo2.write(174);
  delay(1480);
}

void youz_135()
{
  youz();
  delay(732);
}

void youz_135_banyun()
{
  servo1.write(174);
  servo2.write(174);
  delay(2220);
}

void zhuan_180()
{
  servo1.write(176);
  servo2.write(176);
  delay(1630);
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
      delay(700);
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
      delay(700);
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
      delay(700);
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

void push_back_A()     //放到相应点后,回中
{
  hou();
  delay(500);
//  zhuan_180();
  servo1.write(177);
  servo2.write(177);
  delay(1444);
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
      delay(700);
      break;      
    }
    else
    {
      xj_qian();
    }
  }  
}

void push_back_B()     //放到相应点后,回中
{
  hou();
  delay(500);
//  zhuan_180();
  servo1.write(177);
  servo2.write(177);
  delay(1444);
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
      delay(700);
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
  
  {

  }
  
  gohome();
  tingzhi();
  
}

void loop()
{
  
}
