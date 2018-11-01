#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

#define E1_A 25
#define E2_A 27
#define E1_B 33
#define E2_B 35
#define E1_C 41
#define E2_C 43
#define E1_D 49
#define E2_D 51
#define E1_E 23
#define E2_E 29
#define E1_F 31
#define E2_F 37
#define E1_G 39
#define E2_G 45
#define E1_H 47
#define E2_H 53

#define PWM1 6
#define PWM2 8
#define PWM3 10
#define PWM4 12
#define PWM5 7
#define PWM6 9
#define PWM7 11
#define PWM8 13

//#define E_485 3

//long previousMillis = 0;
//long interval = 1000; //定时时间

int V_Updown = 0;
int V_Turn = 0;
int V_Goback = 0;
int V_Leftright = 0;
int V_jidianqi = 0;
int V_duoji1 = 20;
int V_duoji2 = 90;
int V_duoji3 = 90;
int V_CH7 =0;
int V_CH8 =0;

int speed_1_Val = 0;
int speed_2_Val = 0;
int speed_3_Val = 0;
int speed_4_Val = 0;
int speed_5_Val = 0;
int speed_6_Val = 0;
int speed_7_Val = 0;
int speed_8_Val = 0;

void Moto_out_1(unsigned int speed_m)
{
  digitalWrite (E1_A,1);
  digitalWrite (E2_A,0);
  analogWrite (PWM1,speed_m);  
}

void Moto_out_2(unsigned int speed_m)
{
  digitalWrite (E1_B,0);
  digitalWrite (E2_B,1);
  analogWrite (PWM2,speed_m);  
}

void Moto_out_3(unsigned int speed_m)
{
  digitalWrite (E1_C,0);
  digitalWrite (E2_C,1);
  analogWrite (PWM3,speed_m);  
}

void Moto_out_4(unsigned int speed_m)
{
  digitalWrite (E1_D,1);
  digitalWrite (E2_D,0);
  analogWrite (PWM4,speed_m);  
}

void Moto_out_5(unsigned int speed_m)
{
  digitalWrite (E1_E,1);
  digitalWrite (E2_E,0);
  analogWrite (PWM5,speed_m);  
}

void Moto_out_6(unsigned int speed_m)
{
  digitalWrite (E1_F,1);
  digitalWrite (E2_F,0);
  analogWrite (PWM6,speed_m);  
}

void Moto_out_7(unsigned int speed_m)
{
  digitalWrite (E1_G,1);
  digitalWrite (E2_G,0);
  analogWrite (PWM7,speed_m);  
}

void Moto_out_8(unsigned int speed_m)
{
  digitalWrite (E1_H,1);
  digitalWrite (E2_H,0);
  analogWrite (PWM8,speed_m);  
}

void setup()
{
  Serial.begin(9600);//设置波特率
  Serial1.begin(9600);
  
  servo1.attach(3);
  servo2.attach(4);
  servo3.attach(5);
  
  pinMode(E1_A,OUTPUT);
  pinMode(E2_A,OUTPUT);
  pinMode(E1_B,OUTPUT);
  pinMode(E2_B,OUTPUT);
  pinMode(E1_C,OUTPUT);
  pinMode(E2_C,OUTPUT);
  pinMode(E1_D,OUTPUT);
  pinMode(E2_D,OUTPUT);
  pinMode(E1_E,OUTPUT);
  pinMode(E2_E,OUTPUT);
  pinMode(E1_F,OUTPUT);
  pinMode(E2_F,OUTPUT);
  pinMode(E1_G,OUTPUT);
  pinMode(E2_G,OUTPUT);
  pinMode(E1_H,OUTPUT);
  pinMode(E2_H,OUTPUT);
  
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
}
void loop()
{
//    int flag = 0;
//    static int v = 0;
//    servo1.write(20);
//    servo2.write(90);
    servo3.write(60);
  
  digitalWrite(3,0);
  digitalWrite(4,0);
//  digitalWrite(5,0);

  while(1)
  {
    int flag = 0;
    static int v = 0;
    if(Serial1.available())
	{
		char ch = Serial1.read();//读取串口数据
		switch(ch)
		{
			case'0'...'9':v = v*10+ch-'0';//字符换成合适进制
					break;
                        case'Z':V_Updown=v;
                                        flag=1;
                                        v=0;
                                       break;
                        case'O':V_Turn=v-255;
                                        flag=2;
                                        v=0;
                                       break;
                        case'X':V_Goback=v-255;
                                        flag=3;
                                        v=0;
                                       break;
                        case'Y':V_Leftright=v-255;
                                        flag=4;
                                        v=0;
                                       break;
                        case'A':V_jidianqi=v;
                                        flag=5;
                                        v=0;
                                        break;
                        case'B':V_duoji1=v;
                                        flag=6;
                                        v=0;
                                        break;
                        case'C':V_CH7=v;
                                        flag=7;
                                        v=0;
                                        break;
                        case'D':V_CH8=v;
                                        flag=8;
                                        v=0;
                                        break;
                        case'E':V_duoji2=v;
                                        flag=9;
                                        v=0;
                                        break;
                        case'F':V_duoji2=v;
                                        flag=10;
                                        v=0;
                                        break;
                        
		}
	}

        if(flag==1)
        {
            if(V_Updown<80&V_Updown>20)
              V_Updown=80;
            if(V_Updown<=20)
              V_Updown=0;
            Serial.print(V_Updown);Serial.print("\t");
            Moto_out_1(V_Updown);
            Moto_out_2(V_Updown);
            Moto_out_3(V_Updown);
            Moto_out_4(V_Updown);
        }
        
        if(flag==2)
        {
            Serial.print(V_Turn);Serial.print("\t");
            if(V_Turn>=0)
            {
                if(V_Turn<90&V_Turn>50)
                  V_Turn=90;
                if(V_Turn<=50)
                  V_Turn=0;
                speed_6_Val = V_Turn;
                speed_7_Val = V_Turn;
                Moto_out_6(speed_6_Val);
                Moto_out_7(speed_7_Val);
            }
            else
            {
                if(V_Turn>-90&V_Turn<-50)
                  V_Turn=-90;
                if(V_Turn>=-50)
                  V_Turn=0;
                speed_5_Val = -V_Turn;
                speed_8_Val = -V_Turn;
                Moto_out_5(speed_5_Val);
                Moto_out_8(speed_8_Val);    
            }
        }
        if(flag==3)
        {
            Serial.print(V_Goback);Serial.print("\t");
            if(V_Goback>=0)
            {
                if(V_Goback<90&V_Goback>50)
                  V_Goback=90;
                if(V_Goback<=50)
                  V_Goback=0;
                if(V_Goback>=V_Leftright&V_Goback>=-V_Leftright)
                {
                    speed_5_Val = V_Goback;
                    speed_6_Val = V_Goback;
                    Moto_out_5(speed_5_Val);
                    Moto_out_6(speed_6_Val);
                }
            }
            else
            {
                if(V_Goback>-90&V_Goback<-50)
                  V_Goback=-90;
                if(V_Goback>=-50)
                  V_Goback=0;
                if(-V_Goback>=V_Leftright&-V_Goback>=-V_Leftright)
                {
                    speed_7_Val = -V_Goback;
                    speed_8_Val = -V_Goback;
                    Moto_out_7(speed_7_Val);
                    Moto_out_8(speed_8_Val);
                }    
            }    
        } 
        
        if(flag==4)
        {
            Serial.print(V_Leftright);Serial.print("\t");
            if(V_Leftright>=0)
            {
                if(V_Leftright<90&V_Leftright>50)
                  V_Leftright=90;
                if(V_Leftright<=50)
                  V_Leftright=0;
                if(V_Leftright>V_Goback&V_Leftright>-V_Goback)
                {
                    speed_6_Val = V_Leftright;
                    speed_8_Val = V_Leftright;
                    Moto_out_6(speed_6_Val);
                    Moto_out_8(speed_8_Val);
                }
            }
            else
            {
                if(V_Leftright>-90&V_Leftright<-50)
                  V_Leftright=-90;
                if(V_Leftright>=-50)
                  V_Leftright=0;
                if(-V_Leftright>V_Goback&-V_Leftright>-V_Goback)
                {
                    speed_5_Val = -V_Leftright;
                    speed_7_Val = -V_Leftright;
                    Moto_out_5(speed_5_Val);
                    Moto_out_7(speed_7_Val);  
                }  
            }  
        }
        
//        if(flag==5)
//        {
          
//        }

        if(flag==6)
        {
          servo1.write(V_duoji1);
        }
        
        if(flag==7)
        {
          if(V_CH7==0)
          {
            V_duoji3++;
            servo3.write(V_duoji3);
          }
          else
          {
            servo3.write(V_duoji3);
          }
        }
        
        if(flag==8)
        {
          if(V_CH8==0)
          {
            V_duoji3--;
            servo3.write(V_duoji3);
          }
          else
          {
            servo3.write(V_duoji3);
          }
        }
        
        if(flag==9||flag==10)
        {
          servo2.write(V_duoji2);
        }
  }

//  if(currentMillis - previousMillis > interval)
//    {
//      previousMillis = currentMillis;
//       V_POWER=(analogRead(POWER)-680)*1.35;
//    if(V_POWER>=99){V_POWER=99;}   
//       Serial.print(V_POWER);
//       Serial.print('V');
//    }
//      Serial.flush();
}
