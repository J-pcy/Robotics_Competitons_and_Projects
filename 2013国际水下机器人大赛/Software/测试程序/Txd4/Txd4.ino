#define CH1 3
#define CH2 2
#define CH3 0
#define CH4 1

#define CH5 2
#define CH6 3
#define CH7 9
#define CH8 10
#define CH9 11
#define CH10 12

int V_CH1=0;
int V_CH2=0;
int V_CH3=0;
int V_CH4=0;
int V_CH5=0;
int V_CH6=0;
int V_CH7=0;
int V_CH8=0;
int V_CH9=0;
int V_CH10=0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  V_CH1 = analogRead(CH1);
  V_CH2 = analogRead(CH2);
  V_CH3 = analogRead(CH3);
  V_CH4 = analogRead(CH4);
  
  V_CH5 = digitalRead(CH5);
  V_CH6 = digitalRead(CH6);
  V_CH7 = digitalRead(CH7);
  V_CH8 = digitalRead(CH8);
  V_CH9 = digitalRead(CH9);
  V_CH10 = digitalRead(CH10);
  
  Serial.print(V_CH1);Serial.print("\t");
  Serial.print(V_CH2);Serial.print("\t");
  Serial.print(V_CH3);Serial.print("\t");
  Serial.print(V_CH4);Serial.print("\t");
  Serial.print(V_CH5);Serial.print("\t");
  Serial.print(V_CH6);Serial.print("\t");
  Serial.print(V_CH7);Serial.print("\t");
  Serial.print(V_CH8);Serial.print("\t");
  Serial.print(V_CH9);Serial.print("\t");
  Serial.println(V_CH10);
}
