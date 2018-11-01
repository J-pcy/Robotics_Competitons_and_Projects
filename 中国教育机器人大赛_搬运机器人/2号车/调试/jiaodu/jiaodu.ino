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
  servo1.write(80);
  servo2.write(180);
}

void qianzp()
{
  servo1.write(0);
  servo2.write(80);
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

void tingzhi()
{
  servo1.write(80);
  servo2.write(80);  
}

void zuoz_135_banyun()
{
  servo1.write(77);
  servo2.write(77);
  delay(2365);//满电 
}

void youz_135_banyun()
{
  servo1.write(83);
  servo2.write(83);
  delay(2000);//满电 
}

void zhuan_180()
{
  servo1.write(85);
  servo2.write(85);
  delay(1600);//满电   电不足：1700
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
  
//  zhuan_180();
//  zuoz_135_banyun();
  youz_135_banyun();
  tingzhi();
  delay(5000);

}
