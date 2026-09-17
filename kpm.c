#include"kpm.h"
#include"types.h"
#include"macros.h"
#include"delay.h"
#include"lcd.h"
#include<lpc21xx.h>
u8 menu_flag=0;
#define KEYPAD
#ifdef KEYPAD
s32 kpmlut[4][4]={  {'1','2','3','A'},
					{'4','5','6','B'},
					{'7','8','9','-'},
					{'*','0','=','B'}};
#else
s32 kpmlut[4][4]={  {'1','2','3','4'},
					{'5','6','7','8'},
					{'9','0','+','-'},
					{'*','/','=','B'}};
#endif
void init_kpm(void)
{
	WNIBBLE(IODIR1,row0,15);
}
u32 colscan(void)
{
	return (RNIBBLE(IOPIN1,col0)<15)?0:1;
}

u32 rowcheck(void)
{
	u32 rno;
	for(rno=0;rno<4;rno++)
	{
		WNIBBLE(IOPIN1,row0,~(1<<rno));
		if(colscan()==0)
		{
					break;
		}						   
		
	}
	WNIBBLE(IOPIN1,row0,0X0);
	return rno;
}
u32 colcheck(void)
{
	u32 cno;
	for(cno=0;cno<4;cno++)
	{
		if(RBIT(IOPIN1,(col0+cno))==0)
		{
		break;
		}
	}
	return cno;
}
s32 keyscan(void)
{
	s32 keyv,rno,cno;
	//wait for switch press
	while(colscan()&&menu_flag==0);
	if(menu_flag==1)
	{
		return -1;
	}
	//find the row number
	rno=rowcheck();
	//find the col number
	cno=colcheck();
	//get the key value from kpmLUT
	keyv=kpmlut[rno][cno];
	//wait upto realease the switch press 
	while(!colscan());
	
	delay_ms(100);
	return keyv;
}
 s32 readnum(void)
 {
	s32 key,cnt=0;
	int num=0;				
	while(1)
	{
  			key = keyscan();
			if(key==-1)
			{
				cnt = 0;
				return -1;
			}
			if((key>='0')&&(key<='9')&&(cnt<4))
			{
				 cnt++;
				 char_lcd(key);
				 num=num*10+(key-48);
			}
			else if((key=='B')&&(cnt!=0))
			{
			cnt--;
			cmd_lcd(0x10);
			char_lcd(' ');
			cmd_lcd(0x10);
			num/=10;	
			}
			else if((key =='=')&&(cnt!=0))
			{
				cnt=0;
				return num;
			}
	}  
 }
/*void password_by_kpm(char *p)
{
	while(1)
	{
		*p = keyscan();
		char_lcd(*p);
		delay_us(100);
		cmd_lcd(0x10);
		char_lcd('*');
	}
}
							.
 u32 readop(void)
 {
 	u32 key;
label:
	cmd_lcd(0x80);//pos 0 line1
	str_lcd("enter the operator:");
	key = keyscan();
	cmd_lcd(0xc0);
	char_lcd(key);
	if((key =='+')||(key == '-')||(key =='/')||(key == '*'))
	{
		return key;
	}
	cmd_lcd(0x01);
	cmd_lcd(0x80);//pos 0 line1
	str_lcd("entered wrong operator...");
	cmd_lcd(0xc0);
	str_lcd("AGAIN enter operator");
	delay_ms(1000);
	goto label;
 }

void id_by_kpm(u8 *ptr)
{
 	char i;
	for(i=0;i<4;i++)
	{
		ptr[i] =keyscan();
		char_lcd(ptr[i]);	
	}
}*/
void password_kpm(char *ptr)
{
	u8 a,cnt=0;
	while(1)
	{
		a= keyscan();
		if((a>='0')&&(a<='9')&&(cnt!=4))
		{	cnt++;
			char_lcd(a);
			delay_ms(300);
			cmd_lcd(0x10);
			char_lcd('*');
			*ptr =a;
			ptr++;
		}
		else if((a=='B')&&(cnt!=0))
		{
			cmd_lcd(0x10);
			char_lcd(' ');
			cmd_lcd(0x10);
			cnt--;
			ptr--;
		}
		else if((a=='=')&&(cnt==4))
		{
			cnt =0;
			*ptr ='\0';
			return;
		}
	}
}
void id_kpm(s8 *ptr)
{
	s8 a,cnt=0;
	while(1)
	{
	a = keyscan();
	if((a>='0')&&(a<='9')&&(cnt!=4))
	{	
		char_lcd(a);	
		ptr[cnt]=a;
		cnt++;				
	}
	else if((a=='B')&&(cnt!=0))
	{
		cnt--;
		cmd_lcd(0x10);
		char_lcd(' ');
		cmd_lcd(0x10);
	}
	else if((a == '=')&&(cnt==4))
	{
	  ptr[cnt]='\0';
	  return;
	}			 

	}	
}
