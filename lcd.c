#include<lpc21XX.h>
#include"macros.h"
#include "types.h"
#include "delay.h"
#include "lcd.h"
void write_lcd(u8 data)
{
	 G_SETBIT(IOCLR0,RW);//WRITE OPERATION
	 WBYTE(IOPIN0,DATA,data);
	 G_SETBIT(IOSET0,EN);//ENABLE =1
	  delay_us(1);			  
	 G_SETBIT(IOCLR0,EN);
	 delay_ms(2);
}
void cmd_lcd(u8 cmd)
{
	G_SETBIT(IOCLR0,RS);//RS =0 COMMAND REG IS SELECTED
	write_lcd(cmd);
}
void init_lcd()
{
	WBYTE(IODIR0,DATA,0XFF);
	SETBIT(IODIR0,RS);
	SETBIT(IODIR0,RW);
	SETBIT(IODIR0,EN);

	delay_ms(15);
	cmd_lcd(MODE_8BIT_LINE1);
	delay_ms(5);
	cmd_lcd(0x30);
	delay_us(100);
	cmd_lcd(MODE_8BIT_LINE1);

	cmd_lcd(MODE_8BIT_LINE2);//0X38
	cmd_lcd(DISP_ON);//0X0F
	cmd_lcd(LCD_CLR);
	cmd_lcd(DISP_SHIFT_OFF_INC);
}
void char_lcd(u8 character)
{
	SETBIT(IOSET0,RS);//RS =1  DATA REG SELECTED
	write_lcd(character);

}
void str_lcd(u8 *p)
{
	while(*p)
		char_lcd(*p++);
}
void u32_lcd( u32 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
	{char_lcd('0');}
	else
	{
		while(n)
		{
			a[i]=(n%10)+48;
			i++;
			n/=10;
		}
		for(--i;i>=0;i--)
		{
			char_lcd(a[i]);
		}
	}

}
 			
 void hex_lcd(u32 n)
 {

   u8 c[10],rem;
   s32 i=0; 

   if(n==0)
   {char_lcd('0');}
   else
   {
   		while(n)
		{
			 rem =n%16;
 			c[i] = (rem<10)?rem+48:rem+55;
			i++;
			n/=16;
		}
		for(--i;i>=0;i--)
		{
			char_lcd(c[i]);
		}
   }

 }

void bin_lcd(u32 n,u8 bit)
{
	s32 i=0;
	for(i=bit-1;i>=0;i--)
	{
		char_lcd(((n>>i)&1)+48);
	}
}
void oct_lcd(u32 n)
{
	u8 c[15]={0};
	s32 i=0;
	if(n==0)
	{char_lcd('0');}
	else
	{
		while(n)
		{
			c[i] =n%8;
			i++;
			n/=8;
		}
		for(--i;i>=0;i++)
		{
			char_lcd(c[i]);
		}
	}
}
  void bulid_CGRAM(u8 *p,u8 nb)
  {
  	s32 i;
  	cmd_lcd(GOTO_CGRAM);
	for(i=0;i<=nb;i++)
	{
		char_lcd(p[i]);
	}
	cmd_lcd(GOTO_LINE1_POS_0);
  	
  }

void float_lcd(f32 f,s32 np)
{
	s32 i;
	if(f<0)
	{
		char_lcd('-');
		f*=-1;
	}
	i=f;
	u32_lcd(i);
	char_lcd('.');
	f= f-i;
	while(np)
	{
		f*=10;
		np--;
	}
	i =f;
	u32_lcd(i);


}
void s32_lcd(s32 n)
{
	if(n<0)
	{
	char_lcd('-');
	n*=-1;
	}
	u32_lcd(n);
}
void bulid_symbols(void)
{
	u8 i,sy[60]={
				 0x00,0x11,0x0A,0x04,0x0A,0x11,0x00,0x00,  // x
				 0x00,0x01,0x02,0x14,0x08,0x00,0x00,0x00,  // correct
				 0x02,0x02,0x0e,0x1f,0x1f,0x1f,0x0e,0x00,	 //finger
				 0x00,0x1F,0X11,0X11,0X1F,0X1B,0X1F,0X00, //LOCKED
				 0X1F,0X11,0X01,0X01,0X1F,0X1B,0X1F,0X00,	//UNLOCKED
				 0x00,0x04,0x06,0x1F,0X1F,0X06,0X04,0X00,
				 0X00,0X04,0X0C,0X1F,0X1F,0X0C,0X04,0X00
				 }; 
	cmd_lcd(GOTO_CGRAM);
	for(i=0;i<=55;i++)
	{
		char_lcd(sy[i]);	
	}
	cmd_lcd(0x80);
}
void title1(void)
{
 		str_lcd("SECURE ACCESS");
		delay_ms(1000);
		cmd_lcd(0xc0);
		str_lcd("CONTROL SYSTEM");
		delay_ms(1000);
}

