#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup()  //初始化设置
{
  servo1.attach(9); //定义上下自由度舵机
  servo2.attach(10); //定义旋转自由度舵机
  servo3.attach(11); //定义开闭舵机
  servo4.attach(12); //定义旋转机械手
  
  pinMode(9,OUTPUT); 
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT); //定义舵机使用引脚输出
}


void loop()
{
  servo1.write(0);
  servo2.write(60);
  servo3.write(120);
  servo4.write(180);
}

