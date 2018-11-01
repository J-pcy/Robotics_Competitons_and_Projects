#define E1_A 24
#define E2_A 25
#define E1_B 26
#define E2_B 27
#define E1_C 28
#define E2_C 29
#define E1_D 30
#define E2_D 31

#define PWM1 3
#define PWM2 4
#define PWM3 5
#define PWM4 6

long previousMillis = 0;
long interval = 1000; //定时时间

int V_Updown = 0;
int V_Turn = 0;
int V_Goback = 0;
int V_Leftright = 0;

void setup()
{
  Serial.begin(9600);//设置波特率
  Serial1.begin(9600);
  
  pinMode(E1_A,OUTPUT);
  pinMode(E2_A,OUTPUT);
  pinMode(E1_B,OUTPUT);
  pinMode(E2_B,OUTPUT);
  pinMode(E1_C,OUTPUT);
  pinMode(E2_C,OUTPUT);
  pinMode(E1_D,OUTPUT);
  pinMode(E2_D,OUTPUT);
  
//  pinMode(PWM1,OUTPUT);
//  pinMode(PWM2,OUTPUT); 
//  pinMode(PWM3,OUTPUT);
//  pinMode(PWM4,OUTPUT); 
}
void loop()
{
    int flag = 0;
    static int v = 0;
    unsigned long currentMillis = millis();
    if(Serial1.available())
	{
		char ch = Serial1.read();//读取串口数据
		switch(ch)
		{
			case'0'...'9':v = v*10+ch-'0';//字符换成合适进制
					break;
                        case'S':V_Updown=v;
                                        flag=1;
                                        v=0;
                                        break;
                        case'P':V_Turn=v-255;
                                        flag=2;
                                        v=0;
                                       break;
                        case'F':V_Goback=v-255;
                                        flag=3;
                                        v=0;
                                       break;
                        case'H':V_Leftright=v-255;
                                        flag=4;
                                        v=0;
                                       break;
                        
		}
	}
        if(flag==1)
        {
            Serial.print(V_Updown);Serial.print("\t");
            digitalWrite(E1_A,LOW);
            digitalWrite(E2_A,HIGH); 
            analogWrite(PWM1,V_Updown);
            
            digitalWrite(E1_B,LOW);
            digitalWrite(E2_B,HIGH); 
            analogWrite(PWM2,V_Updown);
            
            digitalWrite(E1_C,LOW);
            digitalWrite(E2_C,HIGH); 
            analogWrite(PWM3,V_Updown);
            
            digitalWrite(E1_D,LOW);
            digitalWrite(E2_D,HIGH); 
            analogWrite(PWM4,V_Updown);
        }
        
        if(flag==2)
        {
            Serial.print(V_Turn);Serial.print("\t");
        }
        
        if(flag==3)
        {
            Serial.print(V_Goback);Serial.print("\t");
            digitalWrite(E1_A,LOW);
            digitalWrite(E2_A,HIGH); 
            analogWrite(PWM1,V_Updown-V_Goback);
            
            digitalWrite(E1_B,LOW);
            digitalWrite(E2_B,HIGH); 
            analogWrite(PWM2,V_Updown-V_Goback);
            
            digitalWrite(E1_C,LOW);
            digitalWrite(E2_C,HIGH); 
            analogWrite(PWM3,V_Updown+V_Goback);
            
            digitalWrite(E1_D,LOW);
            digitalWrite(E2_D,HIGH); 
            analogWrite(PWM4,V_Updown+V_Goback);
        }
        
        if(flag==4)
        {
            Serial.println(V_Leftright);
            digitalWrite(E1_A,LOW);
            digitalWrite(E2_A,HIGH); 
            analogWrite(PWM1,V_Updown-V_Leftright);
            
            digitalWrite(E1_B,LOW);
            digitalWrite(E2_B,HIGH); 
            analogWrite(PWM2,V_Updown-V_Leftright);
            
            digitalWrite(E1_C,LOW);
            digitalWrite(E2_C,HIGH); 
            analogWrite(PWM3,V_Updown+V_Leftright);
            
            digitalWrite(E1_D,LOW);
            digitalWrite(E2_D,HIGH); 
            analogWrite(PWM4,V_Updown+V_Leftright);
        }


  if(currentMillis - previousMillis > interval)
    {
      previousMillis = currentMillis;
//       V_POWER=(analogRead(POWER)-680)*1.35;
//    if(V_POWER>=99){V_POWER=99;}   
//       Serial.print(V_POWER);
//       Serial.print('V');
    }
//      Serial.flush();
}
