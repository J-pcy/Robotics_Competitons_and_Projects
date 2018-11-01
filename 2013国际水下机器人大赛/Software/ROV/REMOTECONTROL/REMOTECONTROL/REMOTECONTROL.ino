#include <avr/pgmspace.h>

#include <avrpins.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <address.h>
#include <hidboot.h>

#include <printhex.h>
#include <message.h>
#include <hexdump.h>
#include <parsetools.h>

char *KEY_ASCII;
int i_in=0,up_flag,down_flag,ctrl_flag,shift_flag;
//*************************keyboard-program**********************************************//
class KbdRptParser : public KeyboardReportParser
{
        void PrintKey(uint8_t mod, uint8_t key);
        
protected:
	virtual void OnKeyDown	(uint8_t mod, uint8_t key);
	virtual void OnKeyUp	(uint8_t mod, uint8_t key);
	virtual void OnKeyPressed(uint8_t key);
};

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    Keyboard(&Usb);

uint32_t next_time;

KbdRptParser Prs;

/*************************以下为主函数*****************************/
void KbdRptParser::PrintKey(uint8_t m, uint8_t key)	
{
  char temp_a;
    MODIFIERKEYS mod;
    *((uint8_t*)&mod) = m;
    if(mod.bmLeftCtrl   == 1)
    {
    Serial.print("C");
    Serial2.print("C");
    Serial3.print("C");
    ctrl_flag=1;
    }
    else
    {
    Serial3.print("0");
    Serial2.print("0");
    Serial.print("0");
    }
    
    if(mod.bmLeftShift  == 1)
    {
      Serial.print("S");
     Serial2.print("S");
     Serial3.print("S");
     shift_flag=1;
    }
    else
    {
    Serial3.print("0");
    Serial2.print("0");
    Serial.print("0");
    }
    
    if(mod.bmLeftAlt    == 1)
    {
     Serial.print("A");
    }
    else
    {
     Serial.print("0");
     }
     
     if(mod.bmLeftGUI    == 1)
    {
      Serial.print("G");
    }
    else
    {
     Serial.print("0");
     }
     
    PrintHex<uint8_t>(key);
    *KEY_ASCII=key;
    if(key==0x1A)
    {
      Serial.print("1");
      Serial2.print("1");
      Serial3.print("1");
    }
    else if(key==0x16)
    {
      Serial.print("2");
      Serial2.print("2");
      Serial3.print("C");
    }
    else if(key==0x04)
    {
      Serial.print("3");
      Serial2.print("3");
      Serial3.print("3");
    }
    else if(key==0x07)
    {
      Serial.print("4");
      Serial2.print("4");
      Serial3.print("4");
    }
    else if(key==0x06)
    {
      Serial.print("5");
      Serial2.print("5");
      Serial3.print("5");
    }
    else if(key==0x19)
    {
      Serial.print("6");
      Serial2.print("6");
      Serial3.print("6");
    }
    else if(key==0x52)
    {
      Serial.print("7");
      Serial2.print("7");
      Serial3.print("7");
    }
    else if(key==0x51)
    {
      Serial.print("8");
      Serial2.print("8");
      Serial3.print("8");
    }
    else if(key==0x50)
    {
      Serial.print("9");
      Serial2.print("9");
      Serial3.print("9");
    }
    else if(key==0x4F)
    {
      Serial.print("A");
      Serial2.print("A");
      Serial3.print("A");
    }
    else if(key==0x57)
    {
      Serial.print("B");
      Serial2.print("B");
      Serial3.print("B");
    }
    else if(key==0x56)
    {
      Serial.print("C");
      Serial2.print("C");
      Serial3.print("C");
    }
    else if(key==0x5A)
    {
      Serial.print("D");
      Serial2.print("D");
      Serial3.print("D");
    }
    else if(key==0x28)
    {
      Serial.print("E");
      Serial2.print("E");
      Serial3.print("E");
    }
    else if(key==0x2E)
    {
      Serial.print("F");
      Serial2.print("F");
      Serial3.print("F");
    }
    else if(key==0x2D)
    {
      Serial.print("G");
      Serial2.print("G");
      Serial3.print("G");
    }
    else if(key==0x5C)
    {
      Serial.print("H");
      Serial2.print("H");
    }
    else if(key==0x5E)
    {
      Serial.print("I");
      Serial2.print("I");
      Serial3.print("I");
    }
    else 
    {
    Serial.print("0");
    Serial2.print("0");
    Serial3.print("0");
    }
    
    if(mod.bmRightCtrl   == 1)
    {
      Serial.print("C");
    }
    else
    {
     Serial.print("0");
     }
     
    if(mod.bmRightShift  == 1)
    {
      Serial.print("S");
    }
    else
    {
     Serial.print("0");}
    
    if(mod.bmRightAlt    == 1)
    {
      Serial.print("A");
    }
    else
    {
     Serial.print("0");
    }
    
    if(mod.bmRightGUI  == 1)
    {
     Serial.println("G");
    }
    else
    {
     Serial.print("0");
     } 
     Serial.println("E");
     Serial2.print("E");
     Serial3.print("E");
     for(temp_a=125;temp_a>0;temp_a--)
     {
        if(up_flag==1) 
        {
          Serial2.print("U");
          Serial3.print("U");
        }
        else if(down_flag==1) 
        {
          Serial2.print("D");
          Serial3.print("D");
        }
        else 
        {
          Serial2.print("0");
          Serial3.print("0");
        }
        if(ctrl_flag==1) 
        {
          Serial2.print("C");
          Serial3.print("C");
        }
        else 
        {
          Serial2.print("0");
          Serial3.print("0");
        }
        if(shift_flag==1) 
        {
          Serial2.print("S");
          Serial3.print("S");
        }
        else 
        {
          Serial2.print("0");
          Serial3.print("0");
        }
        if(key==0x1A) 
        {
          Serial2.print("1");
          Serial3.print("1");
        }
        else if(key==0x16) 
        {
          Serial2.print("2");
          Serial3.print("2");
        }
        else if(key==0x04) 
        {
          Serial2.print("3");
          Serial3.print("3");
        }
        else if(key==0x07) 
        {
          Serial2.print("4");
          Serial3.print("4");
        }
        else if(key==0x06) 
        {
          Serial2.print("5");
          Serial3.print("5");
        }
        else if(key==0x19) 
        {
          Serial2.print("6");
          Serial3.print("6");
        }
        else if(key==0x52) 
        {
          Serial2.print("7");
          Serial3.print("7");
        }
        else if(key==0x51) 
        {
          Serial2.print("8");
          Serial3.print("8");
        }
        else if(key==0x50) 
        {
          Serial2.print("9");
          Serial3.print("9");
        }
        else if(key==0x4F) 
        {
          Serial2.print("A");
          Serial3.print("A");
        }
        else if(key==0x57) 
        {
          Serial2.print("B");
          Serial3.print("B");
        }
        else if(key==0x56) 
        {
          Serial2.print("C");
          Serial3.print("C");
        }
        else if(key==0x5A) 
        {
          Serial2.print("D");
          Serial3.print("D");
        }
        else if(key==0x28) 
        {
          Serial2.print("E");
          Serial3.print("E");
        }
        else if(key==0x2E) 
        {
          Serial2.print("F");
          Serial3.print("F");
        }
        else if(key==0x2D) 
        {
          Serial2.print("G");
          Serial3.print("G");
        }
        else if(key==0x5C) 
        {
          Serial2.print("H");
          Serial3.print("H");
        }
        else if(key==0x5E) 
        {
          Serial2.print("I");
          Serial3.print("I");
        }
        else 
        {
          Serial2.print("0");
          Serial3.print("0");
        }
        Serial2.println("E");
        Serial3.println("E");
     }
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)	
{
    Serial.print("D");
    Serial2.print("D");
    down_flag=1;
    PrintKey(mod, key);
    uint8_t c = OemToAscii(mod, key);
    if (c) OnKeyPressed(c);
    
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)	
{
    Serial.print("U");
    Serial2.print("U");
    up_flag=1;
    PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)	
{
    *KEY_ASCII=(char)key;
};

/*************************以下为主函数*****************************/

void setup()
{
  Serial.begin(115200);//设置波特率
  Serial1.begin(115200);//设置波特率
  Serial2.begin(115200);
  Serial3.begin(115200);//设置波特率
  Serial.println("Start");
  if (Usb.Init() == -1) Serial.println("OSC did not start.");
  delay(20);
  next_time = millis() + 50;
  Keyboard.SetReportParser(0, (HIDReportParser*)&Prs);
}

void loop()
{
  up_flag=0;down_flag=0;ctrl_flag=0;shift_flag=0;
  Usb.Task();
  delay(20);
}
