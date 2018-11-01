#define LCD_CE 22
#define LCD_RST 24
#define SCLK 26
#define SDIN 28
#define LCD_DC 30

char UP_DONM,CTRL,SHIFT,KB_WORD,END;                //接受字符存储变量

//***********************************************************************//
unsigned char boy_bmp[]=
{
/*--  调入了一幅图像：C:\Documents and Settings\Administrator\桌面\boy2.bmp  --*/
/*--  宽度x高度=24x24  --*/
0xFF,0x01,0x81,0xC1,0xF1,0xF9,0xF9,0xFD,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFD,0xF9,0xF1,0xE1,0x81,0x01,0xFF,0xFF,0x00,0x1F,0x7F,0xFF,0x3F,0x27,0x27,
0x27,0x27,0x3F,0x17,0x3F,0x27,0x27,0x27,0x27,0x3F,0xFF,0x3F,0x1F,0x0F,0x00,0xFF,
0xFF,0xF0,0xFC,0xFC,0xFE,0xFF,0xFE,0xFC,0xEC,0xF8,0xFA,0x5A,0xFA,0xF8,0xEC,0xFC,
0xFE,0xFF,0xFE,0xFE,0xFC,0xF8,0xF0,0xFF,
};
//**************************调入中文字符字模数据******************************//
const unsigned char HZK[][24]={ 
//深
{0x08,0xF1,0x1A,0x40,0x56,0xCA,0x46,0xF2,0xC6,0x4A,0x56,0x00,0x01,0x07,0x00,0x02,0x01,0x00,0x00,0x07,0x00,0x01,0x02,0x00,}, 
}; 
//****************************定义ASCII字符**********************//

/********************************** 
6 x 8 font 
1 pixel space at left and bottom 
index = ASCII - 32 
***********************************/ 
const unsigned char font6x8[][6] = 
{ 
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp 
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // ! 
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // " 
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // # 
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $ 
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // % 
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // & 
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // ' 
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // ( 
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // ) 
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // * 
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // + 
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // , 
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // - 
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // . 
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // / 
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0 
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1 
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2 
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3 
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4 
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5 
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6 
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7 
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8 
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9 
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // : 
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ; 
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // < 
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // = 
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // > 
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ? 
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @ 
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A 
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B 
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C 
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D 
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E 
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F 
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G 
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H 
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I 
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J 
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K 
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L 
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M 
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N 
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O 
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P 
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q 
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R 
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S 
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T 
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U 
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V 
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W 
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X 
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y 
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z 
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [ 
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55 
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ] 
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^ 
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _ 
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // ' 
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a 
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b 
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c 
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d 
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e 
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f 
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g 
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h 
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i 
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j 
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k 
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l 
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m 
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n 
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o 
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p 
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q 
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r 
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s 
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t 
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u 
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v 
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w 
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x 
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y 
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z 
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines 
}; 

/************************LCD初始化函数********************************/  
void LCD_init(void) 
{ 
    //先设置为输出 
     pinMode(SCLK,OUTPUT); 
     pinMode(SDIN,OUTPUT);
     pinMode(LCD_DC,OUTPUT); 
     pinMode(LCD_CE,OUTPUT);
     pinMode(LCD_RST,OUTPUT);

    // 产生一个让LCD复位的低电平脉冲 
     digitalWrite( LCD_RST, LOW); 

     delayMicroseconds(1);
     digitalWrite( LCD_RST, HIGH); 

    // 关闭LCD 
     digitalWrite( LCD_CE, LOW); 
     delayMicroseconds(1);

    // 使能LCD 
     digitalWrite( LCD_CE, HIGH); //LCD_CE = 1; 
     delayMicroseconds(1); 
    LCD_write_byte(0x21, 0); // 使用扩展命令设置LCD模式 
    LCD_write_byte(0xc8, 0); // 设置偏置电压 
    LCD_write_byte(0x06, 0); // 温度校正 
    LCD_write_byte(0x13, 0); // 1:48 
    LCD_write_byte(0x20, 0); // 使用基本命令 
    LCD_clear();             // 清屏 
    LCD_write_byte(0x0c, 0); // 设定显示模式，正常显示 

    // 关闭LCD 
   digitalWrite( LCD_CE, LOW);  //LCD_CE = 0; 
} 

/************************LCD清屏函数*******************************/ 
void LCD_clear(void) 
{ 
    unsigned int i; 
    LCD_write_byte(0x0c, 0);  
    LCD_write_byte(0x80, 0); 
    for (i=0; i<504; i++) 
      { 
        LCD_write_byte(0, 1); 
      }    
}

/*************************设置字符位置函数**************************/
void LCD_set_XY(unsigned char X, unsigned char Y) 
{ 
    LCD_write_byte(0x40 | Y, 0);// column 
    LCD_write_byte(0x80 | X, 0);// row 
} 
/*************************ASCII字符显示函数*************************/ 
void LCD_write_char(unsigned char c) 
{ 
    unsigned char line; 
    c -= 32; 
    for (line=0; line<6; line++) 
    { 
        LCD_write_byte(font6x8[c][line], 1); 
    } 
}

/*******************************************************************/ 
/*------------------------------------------------- 
LCD_write_english_String  : 英文字符串显示函数 
输入参数：*s      ：英文字符串指针； 
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5 
--------------------------------------------------*/ 
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s) 
{ 
    LCD_set_XY(X,Y); 
    while (*s)  
    { 
     LCD_write_char(*s); 
     s++; 
    } 
}

/******************************************************************/ 
/*--------------------------------------------- 
LCD_write_byte    : 写数据到LCD 
输入参数：data    ：写入的数据； 
          command ：写数据/命令选择； 
---------------------------------------------*/ 
void LCD_write_byte(unsigned char dat, unsigned char command) 
{ 
    unsigned char i; 
   digitalWrite( LCD_CE, LOW); // 使能LCD_CE = 0 
    if (command == 0) 
    { 
     digitalWrite( LCD_DC, LOW);// 传送命令 LCD_DC = 0; 
    } 
    else 
    { 
      digitalWrite( LCD_DC, HIGH);// 传送数据LCD_DC = 1; 
    } 

  for(i=0;i<8;i++) 
  { 
     if(dat&0x80) 
     { 
        digitalWrite( SDIN, HIGH);//SDIN = 1; 
     } 
    else 
    { 
     digitalWrite( SDIN, LOW);//SDIN = 0; 
     } 
   digitalWrite( SCLK, LOW);//SCLK = 0; 
   dat = dat << 1; 
   digitalWrite( SCLK, HIGH);//SCLK = 1; 
  } 
    digitalWrite( LCD_CE, HIGH);//LCD_CE = 1; 

}

/**************************************************************/ 
/*------------------------------------------------- 
LCD_write_chinese_string: 在LCD上显示汉字 
输入参数：X、Y    ：显示汉字的起始X、Y坐标； 
          ch_with ：汉字点阵的宽度 
          num     ：显示汉字的个数；   
          line    ：汉字点阵数组中的起始行数 
          row     ：汉字显示的行间距 
--------------------------------------------------*/                         
void LCD_write_chinese_string(unsigned char X, unsigned char Y,unsigned char ch_with,unsigned char num,unsigned char line,unsigned char row) 
{ 
    unsigned char i,n; 
    LCD_set_XY(X,Y);                             //设置初始位置 
    for (i=0;i<num;) 
    { 
       for (n=0; n<ch_with*2; n++)              //写一个汉字 
       {  
           if (n==ch_with)                      //写汉字的下半部分 
           { 
               if (i==0) LCD_set_XY(X,Y+1); 
               else 
    { 
                  LCD_set_XY((X+(ch_with+row)*i),Y+1); 
    } 
            } 
           LCD_write_byte(HZK[line+i][n],1); 
       } 
       i++; 
       LCD_set_XY((X+(ch_with+row)*i),Y); 
    } 
}

/***********************************************/ 
/*---------------------------------------------- 
LCD_draw_map      : 位图绘制函数 
输入参数：X、Y    ：位图绘制的起始X、Y坐标； 
          *map    ：位图点阵数据； 
          Pix_x   ：位图像素（长） 
          Pix_y   ：位图像素（宽） 
-----------------------------------------------*/ 
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,unsigned char Pix_x,unsigned char Pix_y) 
{ 
    unsigned int i,n; 
    unsigned char row; 
    if (Pix_y%8==0) 
{ 
     row=Pix_y/8;      //计算位图所占行数 
    } 
    else 
{ 
        row=Pix_y/8+1; 
    } 
    for (n=0;n<row;n++) 
    { 
       LCD_set_XY(X,Y); 
        for(i=0; i<Pix_x; i++) 
          { 
            LCD_write_byte(map[i+n*Pix_x], 1); 
          } 
        Y++;                         //换行 
    }       
} 

void LCD_print_number(unsigned char X, unsigned char Y,int val)
{
          LCD_set_XY(X, Y);                       //在(35, 2)坐标上显示测出数值
          if(val>=0)
       {
          LCD_write_char('+'); //显示
          LCD_write_char( 0x30+val%1000/100); //显示百位数
          LCD_write_char( 0x30+val%100/10);   //显示十位数
          LCD_write_char( 0x30+val%10);       //显示个位数
        }
          else
        {
          val=-val;
          LCD_write_char('-'); //显示
          LCD_write_char( 0x30+val%1000/100); //显示百位数
          LCD_write_char( 0x30+val%100/10);   //显示十位数
          LCD_write_char( 0x30+val%10);       //显示个位数
        }
}

void display_program()
{
    switch(KB_WORD)
    {
      case '1':
      {
        LCD_write_english_string(0,1,"Foward") 
      }
      ;break;
      case '2':
      {
        LCD_write_english_string(0,1,"Fallback") 
      }
      ;break;
      case '3':;break;
      case '4':;break;
      case '5':;break;
      case '6':;break;
      case '7':;break;
      case '8':;break;
      case '9':;break;
      case 'A':;break;
      case 'B':;break;
      case 'C':;break;
      case 'D':;break;
      case 'E':;break;
      case 'F':;break;
      case 'G':;break;
      case 'H':;break;
      case 'I':;break;
    }
    level_Moto_up_down=0,level_Moto_lr=0,level_Moto_ff=0,
    level_Hand_catch=0,level_hand_spin=0,level_Hand_ud=90,
    level_Hand_ls=90;
    if
}

void setup()
{
  Serial.begin(115200);//设置波特率
  LCD_init();         //初始化液晶     
  LCD_clear();        //清屏
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
    
    if(KB_WORD!='0'&&t_UP_DONM!=UP_DONM||t_CTRL!=CTRL||t_SHIFT!=SHIFT||
        t_KB_WORD!=KB_WORD||t_END!=END)
     {
       t_UP_DONM=UP_DONM;t_CTRL=CTRL;t_SHIFT=SHIFT;
       t_KB_WORD=KB_WORD;t_END=END;
     }  
      LCD_write_english_string(5,0,"MOVE STATEMENT") 
     display_program();
}c
