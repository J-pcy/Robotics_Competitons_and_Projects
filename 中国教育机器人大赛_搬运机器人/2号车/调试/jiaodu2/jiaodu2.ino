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

void setup()
{
  servo1.attach(3);
  servo2.attach(4);
}

void loop()
{

//  qian();
//  qianzp();
//  qianyp();  
//  zuoz();
//  delay(493);    // 90   满电   电不足：560

  youz();
  delay(498);
//  youz();
//  hou();
//  houzp();
//  houyp();
  tingzhi();

  delay(5000);

/*  if((xj_val1&xj_val2)|(!xj_val3))
  {
    qian();
  }
  
  if((!xj_val1)|xj_val3)
  {
    qianyp();
  }
  
  if((!xj_val2)|xj_val3)
  {
    qianzp();
  }
*/
}
