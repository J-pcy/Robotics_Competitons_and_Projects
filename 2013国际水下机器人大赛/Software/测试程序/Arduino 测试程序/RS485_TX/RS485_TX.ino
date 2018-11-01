int val=0;
void setup()
{
  Serial.begin(9600);
}
void loop()
{ 
for(int j=0;j<30;j++)
Serial.write('W');
for(int i=0;i<50;i++)
{
val=Serial.read();
if(val=='W')
for(int j=0;j<10;j++)
Serial.write(158);
}
}  
  
