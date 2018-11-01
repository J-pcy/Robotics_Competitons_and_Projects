#define E1_A 23
#define E2_A 25
#define E1_B 27
#define E2_B 29
#define E1_C 31
#define E2_C 33
#define E1_D 35
#define E2_D 37
#define E1_E 39
#define E2_E 41
#define E1_F 43
#define E2_F 45
#define E1_G 47
#define E2_G 49
#define E1_H 51
#define E2_H 53

#define PWM1 6
#define PWM2 7
#define PWM3 8
#define PWM4 9
#define PWM5 10
#define PWM6 11
#define PWM7 12
#define PWM8 13

int speed_1_Val = 0;
int speed_2_Val = 0;
int speed_3_Val = 0;
int speed_4_Val = 0;
int speed_5_Val = 0;
int speed_6_Val = 0;
int speed_7_Val = 0;
int speed_8_Val = 0;

int val = 0;

void Moto_out_1(unsigned int speed_m)
{
  digitalWrite (E1_A,0);
  digitalWrite (E2_A,1);
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
  digitalWrite (E1_D,0);
  digitalWrite (E2_D,1);
  analogWrite (PWM4,speed_m);  
}

void Moto_out_5(unsigned int speed_m)
{
  digitalWrite (E1_E,0);
  digitalWrite (E2_E,1);
  analogWrite (PWM5,speed_m);  
}

void Moto_out_6(unsigned int speed_m)
{
  digitalWrite (E1_F,0);
  digitalWrite (E2_F,1);
  analogWrite (PWM6,speed_m);  
}

void Moto_out_7(unsigned int speed_m)
{
  digitalWrite (E1_G,0);
  digitalWrite (E2_G,1);
  analogWrite (PWM7,speed_m);  
}

void Moto_out_8(unsigned int speed_m)
{
  digitalWrite (E1_H,0);
  digitalWrite (E2_H,1);
  analogWrite (PWM8,speed_m);  
}

void setup()
{
  Serial.begin(9600);//设置波特率
  Serial1.begin(9600);
}

void loop()
{
  if(Serial1.available())
  {
    val=Serial1.read();
    if(val&0x80)
    {
      speed_1_Val = val*2;
      speed_2_Val = val*2;
      speed_3_Val = val*2;
      speed_4_Val = val*2;
      val=0;
      Moto_out_1(speed_1_Val);
      Moto_out_2(speed_2_Val);
      Moto_out_3(speed_3_Val);
      Moto_out_4(speed_4_Val);
    }
    
    if(0x20==(val&0xf0))
    {
      if(0x08==val&0x08)
      {
        speed_5_Val = val*32;
        speed_6_Val = val*32;
        val=0;
        Moto_out_5(speed_5_Val);
        Moto_out_6(speed_6_Val);
      }
      if(0x00==(val&0x08))
      {
        speed_7_Val = val*32;
        speed_8_Val = val*32;
        val=0;
        Moto_out_7(speed_7_Val);
        Moto_out_8(speed_8_Val);
      }
    }
    
    if(0x30==(val&0xf0))
    {
      if(0x08==val&0x08)
      {
        speed_5_Val = val*32;
        speed_7_Val = val*32;
        val=0;
        Moto_out_5(speed_5_Val);
        Moto_out_7(speed_7_Val);
      }
      if(0x00==(val&0x08))
      {
        speed_6_Val = val*32;
        speed_8_Val = val*32;
        val=0;
        Moto_out_6(speed_6_Val);
        Moto_out_8(speed_8_Val);
      }
    }
    
    if(0x40==(val&0xf0))
    {
      if(0x08==val&0x08)
      {
        speed_6_Val = val*32;
        speed_7_Val = val*32;
        val=0;
        Moto_out_6(speed_6_Val);
        Moto_out_7(speed_7_Val);
      }
      if(0x00==(val&0x08))
      {
        speed_5_Val = val*32;
        speed_8_Val = val*32;
        val=0;
        Moto_out_5(speed_5_Val);
        Moto_out_8(speed_8_Val);
      }
    } 
  }
}


