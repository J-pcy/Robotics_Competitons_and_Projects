#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

/*#####################################
                     Front
         A-1                  B-2
             E-5        F-6
             G-8        H-7
         C-4                  D-3
#######################################*/

//up-and-down
#define PWM1 3
#define PWM2 7
#define PWM3 9
#define PWM4 5

//left-and-right
#define PWM5 6
#define PWM6 8
#define PWM7 4
#define PWM8 2

#define divied_n_m 13
#define divied_n_h_h 170
#define divied_n_h_l 10

char UP_DONM,CTRL,SHIFT,KB_WORD,END;                //接受字符存储变量

int flag_error=1,flag_error_ud=1,flag_error_c=1,
    flag_error_s=1,flag_error_e=1,flag_error_k=1;   //错误标志位
    
int first_flag=0;                                   //初次运行标志位

char t_UP_DONM,t_CTRL,t_SHIFT,t_KB_WORD,t_END;      //临时存储位

int lock_m_up=1,lock_m_down=1,lock_m_left=1,lock_m_right=1,
    lock_m_fo=1,lock_m_fa=1,lock_m_ls=1,lock_m_rs=1;                         //动作锁位
    
int level_Moto_up_down=0,level_Moto_lr=0,level_Moto_ff=0,
    level_Hand_catch=0,level_hand_spin=0,level_Hand_ud=90,
    level_Hand_ls=90;          //运行程度

int t_level_Moto_up_down=0,t_level_Moto_lr=0,t_level_Moto_ff=0,
    t_level_Hand_catch=0,t_level_hand_spin=0;          //运行程度2
    
void Moto_out_1(int speed_m)     //A-1 Moto
{
  analogWrite (PWM1,speed_m); 
}

void Moto_out_2(int speed_m)     //B-2 Moto
{
  analogWrite (PWM2,speed_m);
}

void Moto_out_3(int speed_m)     //C-3 Moto
{
  analogWrite (PWM3,speed_m);
}

void Moto_out_4(int speed_m)     //D-4 Moto
{
  analogWrite (PWM4,speed_m);
}

void Moto_out_5(int speed_m)     //E-5 Moto
{
  analogWrite (PWM5,speed_m);  
}

void Moto_out_6(int speed_m)     //F-6 Moto
{
  analogWrite (PWM6,speed_m);
}

void Moto_out_7(int speed_m)     //G-7 Moto
{
  analogWrite (PWM7,speed_m);  
}

void Moto_out_8(int speed_m)     //H-8 Moto
{
  analogWrite (PWM8,speed_m);  
}

void Machine_up(int level)     //level 0~10 //Move up
{
  int speed_m=level*25;
  Moto_out_1(255-speed_m);
  Moto_out_2(255-speed_m);
  Moto_out_3(255-speed_m);
  Moto_out_4(255-speed_m);
}

void Machine_down()                    //level 0~10 //Move down
{
  int speed_m=255;
  Moto_out_1(speed_m);
  Moto_out_2(speed_m);
  Moto_out_3(speed_m);
  Moto_out_4(speed_m);
}

void Machine_left(int level) //level 0~10 //Move left
{
  int speed_m=255-level*25;
  Moto_out_5(255);
  Moto_out_6(speed_m);
  Moto_out_7(speed_m);
  Moto_out_8(255);
}

void Machine_right(int level) //level 0~10 //Move right
{
  int speed_m=255-level*25;
  Moto_out_5(255);
  Moto_out_6(speed_m);
  Moto_out_7(speed_m);
  Moto_out_8(255);
}

void Machine_forward(int level) //level 0~10 //Move forward
{
  int speed_m=level*25;
  Moto_out_5(255-speed_m);
  Moto_out_6(255-speed_m);
  Moto_out_7(255);
  Moto_out_8(255);
}

void Machine_fall_back(int level) //level 0~10 //Fall back
{
  int speed_m=level*25;
  Moto_out_5(255);
  Moto_out_6(255);
  Moto_out_7(255-speed_m);
  Moto_out_8(255-speed_m);
}

void Machine_left_spin(int level) //level 0~10 //Left spin
{
  int speed_m=level*25;
  Moto_out_5(255);
  Moto_out_6(255-speed_m);
  Moto_out_7(255);
  Moto_out_8(255-speed_m);
}

void Machine_right_spin(int level) //level 0~10 //Right spin
{
  int speed_m=level*25;
  Moto_out_5(255-speed_m);
  Moto_out_6(255);
  Moto_out_7(255-speed_m);
  Moto_out_8(255);
}

void Hand_up_down(int level)  //level 0~10 //Mechanical arm up
{ 
  int degree=level;
  servo1.write(degree);
}

void Hand_left_spin(int level)  //level 0~10 //Mechanical left spin
{
  int degree=level;
  servo2.write(degree);
}

void Hand_right_spin(int level)  //level 0~10 //Mechanical arm right spin
{
  int degree=level;
  servo2.write(degree);
}

void Hand_close(int level)  //level 0~10 //Mechanical arm close
{
  int degree=level;
  servo3.write(degree);
}

void Hand_open(int level)  //level 0~10 //Mechanical arm open
{
  int degree=level;
  servo3.write(degree);
}

void Hand_spin_l(int level)
{
  int degree=level;
  servo4.write(degree);
}

void Hand_spin_r(int level)
{
  int degree=level;
  servo4.write(degree);
}

void hand_init()
{
  servo1.write(90);
  servo2.write(0);
  servo3.write(90);
  servo4.write(90);
  //analogWrite(12,10);
  //digitalWrite(12,0);
}

void  Moto_init()
{
  Moto_out_1(255);
  Moto_out_2(255);
  Moto_out_3(255);
  Moto_out_4(255);
  Moto_out_5(255); 
  Moto_out_6(255);
  Moto_out_7(255);
  Moto_out_8(255);
}

void setup()  //初始化设置
{
  Serial.begin(125200);   //电脑调试端
  Serial1.begin(125200);  //接收端
  Serial2.begin(115200);  //发送端

  servo1.attach(10); //定义上下自由度舵机
  servo2.attach(12); //定义旋转自由度舵机
  servo3.attach(13); //定义开闭舵机
  servo4.attach(11); //定义旋转机械手

  pinMode(10,OUTPUT); 
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT); //定义舵机使用引脚输出
  
  Serial.println("Start"); //调试开始标志
  Moto_init();
  hand_init();
}
             
void running_mode() //
{
   switch(KB_WORD)
      {
      case '0':;break;
      case '1':
      {
        if(SHIFT=='S')
        {lock_m_fo=0;
         t_level_Moto_ff=level_Moto_ff;
         if(lock_m_fa==0) lock_m_fa=1;
        }
        else if(CTRL=='C') lock_m_fo=1;
        else 
        {
          if(level_Moto_ff>=divied_n_m) level_Moto_ff=divied_n_m;
          Machine_forward(level_Moto_ff);
          level_Moto_ff++;
        }
      }
      ;break;
      case '2':
      { 
        if(SHIFT=='S') 
        {lock_m_fa=0;
         t_level_Moto_ff=level_Moto_ff;
         if(lock_m_fo==0) lock_m_fo=1;
        }
        else if(CTRL=='C') lock_m_fa=1;
        else 
        {
          if(level_Moto_ff>=divied_n_m) level_Moto_ff=divied_n_m;
          Machine_fall_back(level_Moto_ff);
          level_Moto_ff++;
        }
      };break;
      case '6':
      {
        if(SHIFT=='S') 
        {lock_m_ls=0;
         t_level_Moto_lr= level_Moto_lr;
         if(lock_m_rs==0) lock_m_rs=1;
        }
        else if(CTRL=='C') lock_m_ls=1;
        else 
        {
          if(level_Moto_lr>=divied_n_m) level_Moto_lr=divied_n_m;
          Machine_left_spin(level_Moto_lr);
          level_Moto_lr++;
        }
      };break;
      case '5':
      {
        if(SHIFT=='S') 
        {lock_m_rs=0;
        t_level_Moto_lr= level_Moto_lr;
        if(lock_m_ls==0) lock_m_ls=1;
        }
        else if(CTRL=='C') lock_m_rs=1;
        else 
        {
          if(level_Moto_lr>=divied_n_m) level_Moto_lr=divied_n_m;
          Machine_right_spin(level_Moto_lr);
          level_Moto_lr++;
        }
      };break;
      case '3':
      {
        if(SHIFT=='S') 
        {lock_m_left=0;
        t_level_Moto_lr= level_Moto_lr;
        if(lock_m_right==0) lock_m_right=1;
        }
        else if(CTRL=='C') lock_m_left=1;
        else 
        {
          if(level_Moto_lr>=divied_n_m) level_Moto_lr=divied_n_m;
          Machine_left(level_Moto_lr);
          level_Moto_lr++;
        }
      };break;
      case '4':
      {
        if(SHIFT=='S') 
        {lock_m_right=0;
        t_level_Moto_lr= level_Moto_lr;
        if(lock_m_left==0) lock_m_left=1;
        }
        else if(CTRL=='C') lock_m_right=1;
        else 
        {
          if(level_Moto_lr>=divied_n_m) level_Moto_lr=divied_n_m;
          Machine_right(level_Moto_lr);
          level_Moto_lr++;
        }
      };break;
      case '7':
      {
        Hand_up_down(level_Hand_ud);
        level_Hand_ud+=10;
        if(level_Hand_ud>=divied_n_h_h) level_Hand_ud=divied_n_h_h;
      };break;
      case '8':
      {
        Hand_up_down(level_Hand_ud);
        level_Hand_ud-=10;
        if(level_Hand_ud<=divied_n_h_l) level_Hand_ud=divied_n_h_l;
      };break;
      case '9':
      {
        Hand_left_spin(level_hand_spin);
        level_hand_spin+=10;
        if(level_hand_spin>=divied_n_h_h) level_hand_spin=divied_n_h_h;
      };break;
      case 'A':
      {
        Hand_right_spin(level_hand_spin);
        level_hand_spin-=10;
        if(level_Hand_ud<=divied_n_h_l) level_hand_spin=divied_n_h_l;
      };break;
      case 'B':
      {
        Hand_close(level_Hand_catch);
        level_Hand_catch+=10;
        if(level_Hand_catch>=divied_n_h_h) level_Hand_catch=divied_n_h_h;
      };break;
      case 'C':
      {
        Hand_open(level_Hand_catch);
        level_Hand_catch-=10;
        if(level_Hand_catch<=divied_n_h_l) level_Hand_catch=divied_n_h_l;
      };break;
      case 'D':
      {
        Hand_close(10);
      };break;
      case 'E':
      {
        Hand_open(170);
      };break;
      case 'F':
      {
          if(SHIFT=='S')
          {
            lock_m_up=0;
           t_level_Moto_up_down=level_Moto_up_down;
           if(lock_m_left==0) lock_m_left=1;
          }
          else if(CTRL=='C') lock_m_up=1;
          else
         { 
          if(level_Moto_up_down>=divied_n_m) level_Moto_up_down=divied_n_m;
          Machine_up(level_Moto_up_down);
          level_Moto_up_down++;
         }
      };break;
      case 'G':
      {
          if(SHIFT=='S') lock_m_down=0;
          else if(CTRL=='C') lock_m_down=1;
          else Machine_down();
      };break;
      case 'H':
      {
        Hand_spin_l(level_Hand_ls);
        level_Hand_ls+=10;
        if(level_Hand_ls>=divied_n_h_h) level_Hand_ls=divied_n_h_h;
      };break;
      case 'I':
      {
        Hand_spin_r(level_Hand_ls);
        level_Hand_ls-=10;
        if(level_Hand_ls<=divied_n_h_l) level_Hand_ls=divied_n_h_l;
      };break;
      } 
}

void locked_mode() //
{
  if(lock_m_up==0) Machine_up(t_level_Moto_up_down);
  else if(lock_m_down==0) Machine_down();
  else if(lock_m_left==0) Machine_left(t_level_Moto_lr);
  else if(lock_m_right=0) Machine_right(t_level_Moto_lr);
  else if(lock_m_fo==0) Machine_forward(t_level_Moto_ff);
  else if(lock_m_fa==0) Machine_fall_back(t_level_Moto_ff);
  else if(lock_m_ls==0) Machine_left_spin(t_level_Moto_lr);
  else if(lock_m_rs==0) Machine_right_spin(t_level_Moto_lr);
}

void move_level_clear()
{
  
  //move forward fallback
  
  if(t_KB_WORD=='1'&&KB_WORD=='2')
  {
    level_Moto_ff=0;
    t_level_Moto_ff=0;
  }
  else if(t_KB_WORD=='2'&&KB_WORD=='1')
  {
    level_Moto_ff=0;
    t_level_Moto_ff=0;
  }
  
  //move left right
  
  else if(t_KB_WORD=='3'&&KB_WORD=='4')
  {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  else if(t_KB_WORD=='4'&&KB_WORD=='3')
  {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  else if(t_KB_WORD=='3'&&KB_WORD=='5')
  {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  else if(t_KB_WORD=='5'&&KB_WORD=='3')
  {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  else if(t_KB_WORD=='3'&&KB_WORD=='6')
  {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  else if(t_KB_WORD=='6'&&KB_WORD=='3')
  {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  else if(t_KB_WORD=='4'&&KB_WORD=='5')
 {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  else if(t_KB_WORD=='5'&&KB_WORD=='4')
  {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
 else if(t_KB_WORD=='4'&&KB_WORD=='6')
 {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
   else if(t_KB_WORD=='6'&&KB_WORD=='4')
 {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  else if(t_KB_WORD=='5'&&KB_WORD=='6')
    {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  else if(t_KB_WORD=='6'&&KB_WORD=='5')
    {
    level_Moto_lr=0;
    t_level_Moto_lr=0;
  }
  
  //move up-down
  
  else if(t_KB_WORD=='G'&&KB_WORD=='F')
 {
    level_Moto_up_down=0;
    t_level_Moto_up_down=0;
  }
  else if(t_KB_WORD=='F'&&KB_WORD=='G')
  {
    level_Moto_up_down=0;
    t_level_Moto_up_down=0;
  }
  
  //hand
  
  /*else if(t_KB_WORD=='B'&&KB_WORD=='C') level_Hand_catch=0;
  else if(t_KB_WORD=='C'&&KB_WORD=='B') level_Hand_catch=0;
  else if(t_KB_WORD=='D'&&KB_WORD=='E') level_Hand_catch=0;
  else if(t_KB_WORD=='E'&&KB_WORD=='D') level_Hand_catch=0;
  else if(t_KB_WORD=='9'&&KB_WORD=='A') level_hand_spin=0;
  else if(t_KB_WORD=='A'&&KB_WORD=='9') level_hand_spin=0;
  else if(t_KB_WORD=='7'&&KB_WORD=='8') level_Hand_ud=0;
  else if(t_KB_WORD=='8'&&KB_WORD=='7') level_Hand_ud=0;
  else if(t_KB_WORD=='H'&&KB_WORD=='I') level_Hand_ls=0;
  else if(t_KB_WORD=='I'&&KB_WORD=='H') level_Hand_ls=0;*/
  
}

void loop()
{
     if(Serial1.available())
	{
           UP_DONM=Serial1.read();
           if(UP_DONM=='D'||UP_DONM=='U')  
           {
               flag_error_ud=0;
               CTRL=Serial1.read();
               flag_error_c=0;
               if(CTRL=='C'||CTRL=='0') flag_error_c=0;
               else  flag_error_c=1;
               SHIFT=Serial1.read();
               if(SHIFT=='S'||SHIFT=='0') flag_error_s=0;
               else  flag_error_s=1;
               KB_WORD=Serial1.read();
               END=Serial1.read();
               if(END=='E') flag_error_e=0;
               else  flag_error_e=1;
           }
           else flag_error_ud=1;
        }
        
     if(flag_error_ud==1||flag_error_c==1||flag_error_s==1||flag_error_e==1) 
     flag_error=1;
     else flag_error=0;
     
     if(flag_error==0)
     {
      if(first_flag==0)
      {
       Serial.print(UP_DONM);
       Serial.print(CTRL);
       Serial.print(SHIFT);
       Serial.print(KB_WORD);
       Serial.println(END);
       first_flag++;
       t_UP_DONM=UP_DONM;t_CTRL=CTRL;t_SHIFT=SHIFT;
       t_KB_WORD=KB_WORD;t_END=END;
       running_mode();
       }
      else delay(1);
      
     if(KB_WORD!='0'&&t_UP_DONM!=UP_DONM||t_CTRL!=CTRL||t_SHIFT!=SHIFT||
        t_KB_WORD!=KB_WORD||t_END!=END)
     {
       move_level_clear();
       
       Serial.print(UP_DONM);
       Serial.print(CTRL);
       Serial.print(SHIFT);
       Serial.print(KB_WORD);
       Serial.println(END);
       
       Serial.print("MU:");
       Serial.println(level_Moto_up_down);
       Serial.print("MLR:");
       Serial.println(level_Moto_lr);
       Serial.print("MFF:");
       Serial.println(level_Moto_ff);
       Serial.print("HC:");
       Serial.println(level_Hand_catch);
       Serial.print("HS:");
       Serial.println(level_hand_spin);
       Serial.print("HU:");
       Serial.println(level_Hand_ud);
       Serial.print("HLS:");
       Serial.println(level_Hand_ls);

       t_UP_DONM=UP_DONM;t_CTRL=CTRL;t_SHIFT=SHIFT;
       t_KB_WORD=KB_WORD;t_END=END;
       if(UP_DONM=='D') running_mode();
     }  
     else delay(1);
     
     locked_mode();
     
     }
     else delay(5); //must
}
