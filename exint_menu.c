 #include<lpc21xx.h>
 #include "lcd.h"
 #include "types.h"
 #include "exint_menu.h"
 #include "kpm.h"
 #include "lcd.h"
 #include "string.h"
 #include"delay.h"
 #include "i2c_eeprom.h"
 #include "r305.h"
 #include "l293d.h"
//#define NEW_ROM 
#define OLD_ROM
extern u8 menu_flag;
static u8 flag=0;
u8 admin_id,ids=0;
s8 pswd1[5],pswd2[5]="2026";
u32 id;
void init_eint2(void)
{
	//cfg po.7 pin as  eint2
	 //input pins
	 //clr bit pair 14&15 
  	//without affecting other bits
	PINSEL0 &= ~((u32)3<<14);
	
	//update bit14&15 EINT2
	//pin fuction
	PINSEL0 |= EINT2_INPUT_PIN;	
	//VIC confg

	//VICIntSelect =0;//default
	VICIntEnable |= 1<<EINT2_VIC_CHNO;
	VICVectCntl1 = (1<<5)|EINT2_VIC_CHNO;
	VICVectAddr1 =(u32)eint2_isr;

	//cfg EINTO,EINT1 via
	//external interrupts peripherail
	//enable EINT0 &EINT1
	//EXTINT =0;//DEFAULT;

	//CFG EINT0,EINT1 as edge triggering interrupt
	EXTMODE =(1<<2);
	//cfg EINT0,EINT1 as falling edge trigger
	//EXPOLAR =0;//DEFAULT
}
void init_ids(void)
 {
	#ifdef NEW_ROM
	ids = 0;
	i2c_eeprom_write_byte(0X50,0X0000,ids);
	#endif
    #ifdef OLD_ROM
	ids = i2c_eeprom_read_byte(0x50,0x0000); 
	#endif

}
 void eint2_isr(void) __irq
{

	menu_flag=1;
	
//CLEAR EINT1 STATUS IN EXINT 
EXTINT =1<<2;
//CLEAR EINT1 STATUS IN VIC 
VICVectAddr =0;
}
u16 is_id_in_db(char id)
{
	char i=0,id2;
	cmd_lcd(0x01);
  for(i=0;i<ids;i++)
  {
  	id2 = i2c_eeprom_read_byte(0x50,0x0001+(i*6));
  	if(id2==id)
	{
		return 0x0001+(i*6);
	}
  }
  return 0;
}
u16 is_psw_in_db(char *pswd)
{
    char i;
    char pswd_db[6];

    for(i = 0; i < ids; i++)
    {
        i2c_eeprom_seq_read(0x50,0x0002 + (i * 6),(s8 *)pswd_db,5);

        pswd_db[5] = '\0';

        if(strcmp((const char *)pswd,(const char *)pswd_db) == 0)
        {
            return 0x0002 + (i * 6);
        }
    }

    return 0;
}					 
void enroll_id(void)
{
u8 flag=0; 
u8 ids;
//s8 j=0;
ID:			cmd_lcd(0x01);
			str_lcd("ENTER ID(1-5):");
			cmd_lcd(0xc0);
			id = readnum();
			if(!((id>=1)&&(id<=5)))
			{
				cmd_lcd(0x01);
				char_lcd(0);
				str_lcd("INVALID ENTRY");
				char_lcd(0);
				cmd_lcd(0xc0);
				str_lcd("try again...");
				delay_ms(500);
				goto ID;
			} 
			if(is_id_in_db(id))
			{
				cmd_lcd(0x01);
				str_lcd("IT ALREADY EXIST");
				cmd_lcd(0xc0);
				str_lcd("TRY OTHER ID");
				delay_s(1);
				goto ID;
			}
			else if(id<3)
			{
			flag=1;
			i2c_eeprom_write_byte(0x50,0x0001,id); 
			}
			else
			{
			i2c_eeprom_write_byte(0x50,0x0007,id); 
			} 
PSWD:		cmd_lcd(0x01);
			str_lcd("ENTER PASSWORD:");
			cmd_lcd(0xc0);
			password_kpm(pswd1);
			cmd_lcd(0x01);
			str_lcd("ENTER AGAIN:");
			cmd_lcd(0xc0);
			password_kpm(pswd2);
			if(strcmp((const char*)pswd1,(const char*)pswd2)!=0)
			{
			cmd_lcd(0x01);
			str_lcd("NOT MATCHED");
			cmd_lcd(0xc0);
			str_lcd("try again");
			delay_ms(500);
			goto PSWD;	
			}
			if(is_psw_in_db(pswd1))
			{
			str_lcd("pswd exist");
			}
			else if(flag==1)
			{	
				flag=0;
			 i2c_eeprom_write_page(0x50,0x0002,pswd1,5); 
			} 
			else
			{
			i2c_eeprom_write_page(0x50,0x0008,pswd1,5); 
			} 
		   		cmd_lcd(0x01);
				char_lcd(2);
				str_lcd(" go to finger");	
				delay_ms(500);

FP1:		if(enroll(id)!=0)
			{
				cmd_lcd(0x01);
				str_lcd("FAIL FINGER");
				cmd_lcd(0xc0);
				str_lcd("PLEASE TRY AGAIN");
				delay_ms(500);
				goto FP1;
			}			 
			 //i2c_eeprom_write_byte(0x50,0x0001+(ids*6),id);
			 //i2c_eeprom_write_page(0x50,0x0002+(ids*6),(s8*)pswd1,5);
			 ids++;
			 i2c_eeprom_write_byte(0x50,0x0000,ids);
			cmd_lcd(0x01);			
			str_lcd("ENROLL DONE ");
			char_lcd(1);
			delay_s(1);
}
void login(void)
{
	int l_id,k;
	s8 buf1[5],buf2[5],l,retry=0;
	
	u16 found = 0;
	if(ids==0)
	{
		if(flag==0)
		{
			  flag =1;
			  cmd_lcd(0x01);
			  str_lcd("NO DATA FOUND");
			  cmd_lcd(0xc0);
			  str_lcd("PRESS SW TO ENROLL");
			                                                                  
		}
		 return; 
	}
	flag = 0; 
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("ENTER THE ID:"); 
//	cmd_lcd(0xc0);
	l_id = readnum();
	if(l_id == -1)
	{
		return;
	}
	if((found = is_id_in_db(l_id))==0)
	{
	  cmd_lcd(0x01);
	  str_lcd("ID IS NOT FOUND");
	  delay_ms(1000);
	  return;
	}
	cmd_lcd(0x01);
	str_lcd("ID IS FOUND");
	delay_ms(600);
	i2c_eeprom_seq_read(0x50,found+1,(s8 *)buf1,5);
PSWD:
	cmd_lcd(0x01);
	str_lcd("ENTER PASSWORD:");
	cmd_lcd(0xc0);
	password_kpm((s8 *)buf2);
	if(strcmp((const char*)buf1,(const char *)buf2)!=0)
	{
		cmd_lcd(0x01);
		str_lcd("WRONG PASSWORD ");
		char_lcd(0);
		retry++;
		if(retry==3)
		{
		  cmd_lcd(0x01);
		  str_lcd("LOGIN FAILS");
		  delay_ms(1000);
		  return;
		}
		cmd_lcd(0xc0);
		str_lcd("you have ");
		char_lcd(3-retry +48);
		str_lcd(" chances");
		delay_ms(1000);
		goto PSWD;
	}
	cmd_lcd(0x01);
	str_lcd("PASSWORD VERIFED");
	delay_ms(500);
	retry = 0;
FP:
	cmd_lcd(0x01);
	if(verify(l_id)!=0)
	{
		cmd_lcd(0x01);
		str_lcd("FINGER NOT FOUND");
		cmd_lcd(0xc0);
		str_lcd("try again");
		delay_ms(600);
		retry++;
		 if(retry==1)
		 {
			cmd_lcd(0x01);
			str_lcd("two more chance");
			delay_ms(800);
			goto FP;
		 }
	 	else if(retry==2)
	 	{	 
			cmd_lcd(0x01);
			str_lcd("last chance");
			delay_ms(800);
			goto FP;
		 }
		 else
		 {
			cmd_lcd(0x01);
			char_lcd(0);
			str_lcd("ACCESS DENIED");
			char_lcd(0);
			delay_ms(1000);
			return;

	 	}
	}
	
	
		cmd_lcd(0x01);	
		str_lcd("DOOR OPENING ");
		char_lcd(3);
		char_lcd(5);
		char_lcd(4);
		door_open();
		delay_ms(5000);


		motor_stop();
		cmd_lcd(0x01);	
		str_lcd("DOOR OPENED ");
		char_lcd(4);
		
		delay_ms(500);
		for(l=10;l>0;l--)
		{
		cmd_lcd(0x80);
		str_lcd("DOOR CLOSE IN ");
		cmd_lcd(0xc0);
		str_lcd("  ");
		char_lcd((l/10)+48);
		char_lcd((l%10)+48);
		str_lcd(" sec ");
		
		//delay_ms(960);			
		for(k=0;k<9;k++)
		{
			cmd_lcd(0xc0+9);
		    char_lcd(4);
			if(k&1)
			{
				char_lcd(5);
				char_lcd(' ');
			}
			else
			{
				
				char_lcd(' ');
				char_lcd(5);	
			}
			char_lcd(3);
			delay_ms(100);
		} 

		}
		
		cmd_lcd(0x01);	
		str_lcd("DOOR IS CLOSEING");
		cmd_lcd(0xc0);
		str_lcd("   ");
		char_lcd(4);
		char_lcd(5);
		char_lcd(3);
		door_close();
		delay_ms(5000);
		motor_stop();
		cmd_lcd(0x01);
		str_lcd("DOOR IS CLOSED ");
		char_lcd(3);
		delay_ms(1000);

}  
void disable(void)
{
	VICIntEnClr = 1<<EINT2_VIC_CHNO;
}
void enable(void)
{
	VICIntEnable |= 1<<EINT2_VIC_CHNO;	
}
void password_edit(void)
{
	s8 p_id,pw1[5],pw2[5],retry=0;
	u16 found=0;
	cmd_lcd(0x01);
	str_lcd("ENTER UR ID:");
	p_id = readnum();
	if((found=is_id_in_db(p_id))==0)
	{
	  cmd_lcd(0x01);
	  str_lcd("ID IS NOT FOUND");
	  delay_ms(1000);
	  return;	
	}
	cmd_lcd(0x01);
	str_lcd("ID IS FOUND");
	delay_ms(700);
	i2c_eeprom_seq_read(0x50,found+1,pw1,5);
PSWD:
	cmd_lcd(0x01);
	str_lcd("ENTER OLD PASSWORD:");
	cmd_lcd(0xc0);
	password_kpm(pw2);
	if(strcmp((const char*)pw2,(const char *)pw1)!=0)
	{
		cmd_lcd(0x01);
		str_lcd("WRONG PASSWORD");
		retry++;
		if(retry==3)
		{
		  cmd_lcd(0x01);
		  char_lcd(0);
		  str_lcd("   NOT MATCHED ");
		  char_lcd(0);
		  cmd_lcd(0xc0);
		  str_lcd(" CAN'T CHANGED ");
		  delay_ms(1000);
		  return;
		}
		cmd_lcd(0xc0);
		str_lcd("you have ");
		char_lcd(3-retry +48);
		str_lcd(" chances");
		delay_ms(1000);
		goto PSWD;
	}
	cmd_lcd(0x01);
	str_lcd("PASSWORD VERIFED");
	delay_ms(500);
	retry = 0;

FP:
	cmd_lcd(0x01);
	if(verify(p_id)!=0)
	{
		cmd_lcd(0x01);
		str_lcd("FINGER NOT FOUND");
		cmd_lcd(0xc0);
		str_lcd("try again");
		delay_ms(600);
		retry++;
		 if(retry==1)
		 {
			cmd_lcd(0x01);
			str_lcd("two more chance");
			delay_ms(500);
			goto FP;
		 }
		 else if(retry==2)
	 	{	 
			cmd_lcd(0x01);
			str_lcd("last chance");
			delay_ms(500);
			goto FP;
		 }
	 	else
	 	{
		  cmd_lcd(0x01);
		  char_lcd(0);
		  str_lcd("   NOT MATCHED ");
		  char_lcd(0);
		  cmd_lcd(0xc0);
		  str_lcd("    CAN'T CHANGED ");
		  delay_ms(1000);
		  return;
	    }
	}
	else
	{
PSWD1:
		cmd_lcd(0x01);	
		str_lcd("ENTER NEW PASSWORD:");
		cmd_lcd(0xc0);
		password_kpm(pw1);
		cmd_lcd(0x01);
		str_lcd("ENTER AGAIN:");
		cmd_lcd(0xc0);
		password_kpm(pw2);
		if(strcmp((const char*)pw1,(const char*)pw2)!=0)
		{
			cmd_lcd(0x01);
			str_lcd("NOT MATCHED");
			cmd_lcd(0xc0);
			str_lcd("try again");
			delay_ms(500);
			goto PSWD1;	
		}
		else
		{
			i2c_eeprom_write_page(0x50,found+1,pw1,5);	

				cmd_lcd(0x01);
				str_lcd(" NEW PASSWORD ");
				cmd_lcd(0xc0);
				str_lcd("UPDATED ");
				//char_lcd(2);
				char_lcd(' ');
				char_lcd(1);
				delay_ms(500);
		}
			
	}




	
}
void  finger_edit(void)
{
	s8 f_id,pw1[5],pw2[5],retry=0;
	u16 found;

//==========================================//
//           	   ID		                //
//==========================================//
	
	cmd_lcd(0x01);
	str_lcd("ENTER THE ID:");
	f_id = readnum();
	if((found=is_id_in_db(f_id))==0)
	{
		cmd_lcd(0x01);
		str_lcd("ID IS NOT FOUND");
		delay_ms(1000);
		return;
	}
	cmd_lcd(0x01);
	str_lcd("ID IS FOUND");
	delay_ms(900);
//=========================================//
//		PASSWORD		                   //
//=========================================//
	i2c_eeprom_seq_read(0x50,found+1,pw2,5);
PSWD:	
	cmd_lcd(0x01);
	str_lcd("ENTER PASSWORD:");
	cmd_lcd(0xc0);
	password_kpm(pw1);
	if(strcmp((const char*)pw2,(const char *)pw1)!=0)
	{
		cmd_lcd(0x01);
                str_lcd("WRONG PASSWORD");
		delay_ms(1000);
                retry++;
                if(retry==3)
                {
                  cmd_lcd(0x01);
                  char_lcd(0);
                  str_lcd("   NOT MATCHED ");
                  char_lcd(0);
                  cmd_lcd(0xc0);
                  str_lcd("CAN'T CHANGED ");
                  delay_ms(1000);
                  return;
                }
                cmd_lcd(0xc0);
                str_lcd("you have ");
                char_lcd(3-retry +48);
                str_lcd(" chances");
                delay_ms(1000);
                goto PSWD;		
	}
//===============================================//
//      	 FINGER UPDATATION	             	 //
//===============================================//
FP1:
		 delete(f_id);           
		 if(enroll(f_id)!=0)
           {
		   	  cmd_lcd(0x01);
              str_lcd("FAIL FINGER");
              str_lcd("please try again");
		      delay_ms(500);
		      goto FP1;
		  }            
    	cmd_lcd(0x01);
		str_lcd(" SUCCESSFULLY ");	
		cmd_lcd(0xc0);	
		str_lcd("  UPDATED ");
		char_lcd(1);
 		
}
void forget_password(void)
{
	s8 fo_id,pw1[5],pw2[5],retry=0;
	u16 found;

	//========================================//
	//                ID                      //
	//========================================//
	cmd_lcd(0x01);
	str_lcd("ENTER THE ID:");
	fo_id = readnum();
	if((found=is_id_in_db(fo_id))==0)
	{
		cmd_lcd(0x01);
		str_lcd("ID IS NOT FOUND");
		delay_ms(1000);
		return;
	}
	cmd_lcd(0x01);
	str_lcd("ID IS FOUND");
	delay_ms(1000);

	//====================================//
	//            PASSWORD                //
	//====================================//
PSWD:
	cmd_lcd(0x01);
	str_lcd("ENTER THE DEFAULT ");
	cmd_lcd(0xc0);
	str_lcd("PASSWORD:");
	password_kpm(pw1);

	if(strcmp((const char*)"1729",(const char *)pw1)!=0)
	{
		cmd_lcd(0x01);
                str_lcd("WRONG PASSWORD");
		delay_ms(1000);
                retry++;
                if(retry==2)
                {
                  cmd_lcd(0x01);
                  char_lcd(0);
                  str_lcd("   NOT MATCHED ");
                  char_lcd(0);
                  cmd_lcd(0xc0);
                  str_lcd(" CAN'T CHANGED ");
                  delay_ms(1000);
                  return;
                }
                cmd_lcd(0xc0);
                str_lcd("try one more");
                delay_ms(1000);
                goto PSWD;		
	}

	//=================================//
	//      PASSWORD UPDATION          //
	//=================================//
PSWD1:
		cmd_lcd(0x01);	
		str_lcd("ENTER NEW PASSWORD:");
		cmd_lcd(0xc0);
		password_kpm(pw1);
		cmd_lcd(0x01);
		str_lcd("ENTER AGAIN:");
		cmd_lcd(0xc0);
		password_kpm(pw2);
		if(strcmp((const char*)pw1,(const char*)pw2)!=0)
		{
			cmd_lcd(0x01);
			str_lcd("NOT MATCHED");
			cmd_lcd(0xc0);
			str_lcd("try again");
			delay_ms(500);
			goto PSWD1;	
		}
		else
		{
			i2c_eeprom_write_page(0x50,found+1,pw1,5);	

				cmd_lcd(0x01);
				str_lcd(" NEW PASSWORD ");
				cmd_lcd(0xc0);
				str_lcd("UPDATED ");
				//char_lcd(2);
				char_lcd(' ');
				char_lcd(1);
				delay_ms(500);
		}


}
void edit(void)
{
	u8 choice;
	if(ids==0)
	{
		if(flag==0)
		{
			  flag =1;
			  cmd_lcd(0x01);
			  str_lcd("NO DATA FOUND");
			  cmd_lcd(0xc0);
			  str_lcd("PRESS SW TO ENROLL");
			                                                                  
		}
		 return; 
	}
	flag = 0;
while(1)
  {	
	  cmd_lcd(0x01);
	  str_lcd("1.PSSWRD 2.FINGER");
	  cmd_lcd(0xc0);
	  str_lcd("3.FORGOT 4.EXIT");
	  choice = keyscan();
   	  switch(choice)
	  {
	   	 case '1':password_edit();
					break;
		 case '2':finger_edit();
					break;
		 case '3':forget_password();
				break;
		 case '4':break ;
	  }
	  if(choice =='4')
	  {
	  	 break;
	  }
   }
}
/*void delete_id(void)
{
	char d_id,retry=0,null_buff[5] = {'\0','\0','\0','\0','\0'};
	u16 found;
		if(check_admin())
	{
		cmd_lcd(0x01);
		char_lcd(0);
		str_lcd(" ACESS DENIED ");
		char_lcd(0);
		cmd_lcd(0xc0);
		str_lcd("  CANT BE DELETED  ");
		delay_ms(1000);
		return;
	}
ID:
	cmd_lcd(0x01);
	str_lcd("ENTER THE ID:");
	d_id =readnum();
	if((found = is_id_in_db(d_id))==0)
	{
		cmd_lcd(0x01);
		str_lcd("ID IS NOT FOUND");
		delay_ms(1000);
		retry++;
		if(retry ==2)
		{
			cmd_lcd(0xc0);
			str_lcd("CANT BE DELETED..!");
			delay_ms(1000);
			retry=0;
			return;
		}
		else
		{
			cmd_lcd(0x01);
			str_lcd("  TRY AGAIN ");
			cmd_lcd(0xc0);
			str_lcd(" ONE LAST TIME ");
			delay_ms(1000);
			goto ID;
		}			 
	}
	 //while(delete(d_id));
	 if(delete(d_id)!=0)
	 {
	 	cmd_lcd(0x01);
		str_lcd("del fails");
		delay_s(1);
		return;
	 }
	 i2c_eeprom_write_page(0x50,found+1,null_buff,5);
	 i2c_eeprom_write_byte(0x50,found,0xff);
	 cmd_lcd(0x01);
	 str_lcd("    DELETED   ");
	 cmd_lcd(0xc0);
	 str_lcd("  SUCESSFULLY ");
	 char_lcd(1);
	 delay_ms(1000);				  
}*/
void delete_id(void)
{
        s8 id,total,i;
        s8 buf[6];
        u16 found_addr,next_addr,last_addr;

ID:

        cmd_lcd(0x01);

        cmd_lcd(0x80);
        str_lcd("ENTER ID:");

        id = readnum();

        found_addr = is_id_in_db(id);

        if(found_addr == 0)
        {
                cmd_lcd(0x01);

                str_lcd("ID NOT FOUND");

                delay_ms(1000);

                goto ID;
        }

        // delete fingerprint
        if(delete(id) != 0)
        {
                cmd_lcd(0x01);

                str_lcd("DELETE FAIL");

                delay_ms(1000);

                return;
        }

        // total users
        total = i2c_eeprom_read_byte(0x50,0x0000);

        // last record address
        last_addr = 0x0001 + ((total-1) * 6);

        // shift records upward
        while(found_addr < last_addr)
        {
                next_addr = found_addr + 6;

                // read next full user record
                i2c_eeprom_seq_read(0x50,next_addr,buf,6);

                // write into current position
                i2c_eeprom_write_page
                (
                        0x50,
                        found_addr,
                        buf,
                        6
                );

                found_addr += 6;
        }

        // clear last slot
        for(i=0;i<6;i++)
        {
                buf[i] = 0xFF;
        }

        i2c_eeprom_write_page
        (
                0x50,
                last_addr,
                buf,
                6
        );

        // decrement user count
        if(total > 0)
        {
                total--;

                i2c_eeprom_write_byte
                (
                        0x50,
                        0x0000,
                        total
                );
        }

        cmd_lcd(0x01);

        cmd_lcd(0x80);
        str_lcd("USER DELETED");

        cmd_lcd(0xC0);
        str_lcd("SUCCESS");

        char_lcd(1);

        delay_ms(1500);
}
void menu(void)
{
	u8 choice;
	menu_flag=0;
	disable();
	flag =0;
	//s8 pswd1[5],pswd2[5]="2026";
	//u32 id;
	
  while(1)
  {
	cmd_lcd(LCD_CLR);
	cmd_lcd(GOTO_LINE1_POS_0);
	str_lcd("1.ENROLL 2.EDIT");
	cmd_lcd(GOTO_LINE2_POS_0); 
	str_lcd("3.DELETE 4.EXIT");
    choice = keyscan();
	switch(choice)
	{
	  case '1':
				enroll_id();
				break;
	 case '2':
	 			edit();
			 	break;
	 case '3':	
	 			delete_id();
				break;
				
	 case '4':
			break;
	}
	if(choice =='4')
	{
	cmd_lcd(0x01);
	menu_flag=0;
	enable();
	char_lcd(ids+48);	 
		break;
	}
			    
  }
	 

}


