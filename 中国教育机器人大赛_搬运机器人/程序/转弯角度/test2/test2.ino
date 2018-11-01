#include <Servo.h>

Servo servo1;
Servo servo2;

void qian()
{
  servo1.write(0);
  servo2.write(180);
}

void qianyp()
{
  servo1.write(172);
  servo2.write(180);
}

void qianzp()
{
  servo1.write(0);
  servo2.write(172);
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

void houzp()
{
  servo1.write(172);
  servo2.write(0);
}

void houyp()
{
  servo1.write(180);
  servo2.write(172);
}

void tingzhi()
{
  servo1.write(172);
  servo2.write(172);  
}

void zuoz_135_banyun()
{
  servo1.write(169);
  servo2.write(169);
  delay(1700);//满电   电不足：1785
}

void zhuan_180()
{
  servo1.write(177);
  servo2.write(177);
  delay(1630);//满电   电不足：1700
}

void setup()
{
  servo1.attach(3);
  servo2.attach(4);
}

void loop()
{
//  zuoz();
//  delay(500);    // 90
  
  zhuan_180();
  
  tingzhi();
  delay(5000);

}
