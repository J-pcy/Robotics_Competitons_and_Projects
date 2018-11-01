/*#####################################
                     Front
         A-1                  B-2
             E-5        F-6
             G-8        H-7
         C-4                  D-3
#######################################*/

//up-and-down
#define PWM1 2
#define PWM2 3
#define PWM3 4
#define PWM4 5

//left-and-right
#define PWM5 7
#define PWM6 6
#define PWM7 9
#define PWM8 8

int speed_m=150;

void setup()  //初始化设置
{
  /*pinMode(PWM1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(PWM3,OUTPUT);
  pinMode(PWM4,OUTPUT);
  pinMode(PWM5,OUTPUT);
  pinMode(PWM6,OUTPUT);
  pinMode(PWM7,OUTPUT);
  pinMode(PWM8,OUTPUT);*/
}

void loop()
{
  speed_m+=10;
  if(speed_m>=255) speed_m=0;
  analogWrite (PWM1,speed_m); 
  analogWrite (PWM2,speed_m);
  analogWrite (PWM3,speed_m);
  analogWrite (PWM4,speed_m);
  analogWrite (PWM5,speed_m);  
  analogWrite (PWM6,speed_m);
  analogWrite (PWM7,speed_m);  
  analogWrite (PWM8,speed_m);  
  delay(1000);
}
