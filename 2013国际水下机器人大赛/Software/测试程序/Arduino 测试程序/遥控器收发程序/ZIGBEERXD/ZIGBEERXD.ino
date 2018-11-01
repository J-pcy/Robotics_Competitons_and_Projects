#include <Servo.h>

Servo servo1;//定义舵机1
Servo servo2;//定义舵机2
Servo servo3;//定义舵机3
Servo servo4;//定义舵机4

#define MOTO_R 5
#define MOTO_L 6
#define MOTO_C 9

#define POWER 7

int V_POWER = 0;

int V_SPEED=79;
int V_DIR=80;

int RUN_FLAG=0;
//int AD0 = 0;  //设定模拟输入IO脚
//int AD1 = 1;  //设定模拟输入IO脚
//int AD2 = 2;  //设定模拟输入IO脚
//int AD3 = 3;  //设定模拟输入IO脚
//int AD6 = 6;  //设定模拟输入IO脚
//设定模拟输入IO脚  AD7

//int IO1=12;//数字输出1
//int IO2=13;//数字输出2
//int IO3=4;//数字输出3

//int Pow = 0;
//int Val3=0;
//
//int Value=0;//
//int shujuwei=0;
//
//int SPEEDA=0;//电机1转速
//int SPEEDB=0;//电机2转速
//
//int MOT1A=3;//电机1A
//int MOT1B=5;//电机1B
//int MOT2A=11;//电机2A
//int MOT2B=6;//电机2B


long previousMillis = 0;
long interval = 1000; //定时时间

void setup()
{

servo1.attach(5);//定义舵机控制口
servo2.attach(6);//定义舵机控制口
servo3.attach(9);//定义舵机控制口
servo4.attach(10);//定义舵机控制口
//servo4.attach(10);//定义舵机控制口


//pinMode(MOTO_R,OUTPUT);//PWM端口
//pinMode(MOTO_L,OUTPUT);
//pinMode(MOTO_C,OUTPUT);

Serial.begin(38400);//设置波特率

        servo3.write(40);
        servo4.write(120);
}
void loop()
{
    static int v = 0;
    unsigned long currentMillis = millis();
    if(Serial.available())
	{
		char ch = Serial.read();//读取串口数据
		switch(ch)
		{
			case'0'...'9':v = v*10+ch-'0';//字符换成合适进制
					break;
                        case'S':V_Updowm=v;
                                        v=0;
                                        break;
                        case'H':V_Turn=v;
                                        v=0;
                                       break;
                        case'Q':V_Goback=v;
                                        v=0;
                                       break;
                        case'Z':V_Leftright=v;
                                        v=0;
                                       break;
                        
		}
	}
        if(RUN_FLAG==HIGH)
        {
          servo1.attach(5);//定义舵机控制口
          servo2.attach(6);//定义舵机控制口
          servo1.write(V_SPEED-(V_DIR-80));
          servo2.write((160-V_SPEED)-(V_DIR-80));
        }
        else
        {
          servo1.detach();
          servo2.detach();
        //  digitalWrite(5,HIGH);
        //  digitalWrite(8,HIGH);
        }

  if(currentMillis - previousMillis > interval)
    {
      previousMillis = currentMillis;
       V_POWER=(analogRead(POWER)-680)*1.35;
    if(V_POWER>=99){V_POWER=99;}   
       Serial.print(V_POWER);
       Serial.print('V');
    }
      Serial.flush();
}
