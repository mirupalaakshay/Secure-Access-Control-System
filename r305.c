#include "lcd.h"
#include "uart.h"
#include "delay.h"
u8 snd_buf[20],rec_buf[25],j=0,i=0,frame_ready=0;
void send_packet(u8 *ptr,u8 n)
{
	for(j=0;j<n;j++)
		u0_Tx_byte(ptr[j]);
}
s8 Genimg(void)
{
	u16 timeout = 0;
	frame_ready=0;
	i=0;
	for(j=0;j<25;j++)
		rec_buf[j]=0;
	for(j=0;j<12;j++)
		snd_buf[j]=0;
	snd_buf[0]=0xEF;
	snd_buf[1]=0x01;  //header
	snd_buf[2]=0xFF;
	snd_buf[3]=0xFF;
	snd_buf[4]=0xFF;
	snd_buf[5]=0xFF;  //adress
	snd_buf[6]=0x01;  //cmd select
	snd_buf[7]=0x00;
	snd_buf[8]=0x03;  //length
	snd_buf[9]=0x01; //cmd for capature
	snd_buf[10]=0x00;
	snd_buf[11]=0x05; //check sum
	
	send_packet(snd_buf,12);

	delay_ms(10);
	while(frame_ready == 0)
    {
        delay_ms(1);
        timeout++;
        if(timeout > 1000) return 1;
    }
	if(rec_buf[9]==0x00)
		return 0;
	else
		return 1;
}
s8 img2tp1(void)
{
	u16 timeout = 0;
	frame_ready=0;
	i=0;
	for(j=0;j<25;j++)
		rec_buf[j]=0;
	for(j=0;j<13;j++)
		snd_buf[j]=0;
	
	snd_buf[0]= 0xEF;
	snd_buf[1]= 0x01;  //header
	snd_buf[2]= 0xFF;
	snd_buf[3]= 0xFF;
	snd_buf[4]= 0xFF;
	snd_buf[5]= 0xFF;  //adress
	snd_buf[6]= 0x01;  //cmd select
	snd_buf[7]=	0x00;
	snd_buf[8]=	0x04;  //length
	snd_buf[9]=	0x02; //cmd for capature
	snd_buf[10]=0x01; //store in buffer1
	snd_buf[11]=0x00;
	snd_buf[12]=0x08; //check sum

	frame_ready=0;
	send_packet(snd_buf,13);
	while(frame_ready == 0)
    {
        delay_ms(1);
        timeout++;
        if(timeout > 1000) return 1;
    }

	if(rec_buf[9]==0x00)
		return 0;
	else
		return 1;

}
s8 img2tp2(void)
{
	u16 timeout = 0;
	frame_ready=0;

	i=0;
	for(j=0;j<25;j++)
		rec_buf[j]=0;
	for(j=0;j<13;j++)
		snd_buf[j]=0;
	
	snd_buf[0]= 0xEF;
	snd_buf[1]= 0x01;  //header
	snd_buf[2]= 0xFF;
	snd_buf[3]= 0xFF;
	snd_buf[4]= 0xFF;
	snd_buf[5]= 0xFF;  //adress
	snd_buf[6]= 0x01;  //cmd select
	snd_buf[7]=	0x00;
	snd_buf[8]=	0x04;  //length
	snd_buf[9]=	0x02; //cmd for 
	snd_buf[10]=0x02; //store in buffer2
	snd_buf[11]=0x00;
	snd_buf[12]=0x09; //check sum

	
	send_packet(snd_buf,13);
	while(frame_ready==0)
	{
        delay_ms(1);
        timeout++;
        if(timeout > 1000) 
			return 1;
    }

	if(rec_buf[9]==0x00)
		return 0;
	else
		return 1;

}
s8 regmodel(void)
{
	u16 timeout = 0;
    frame_ready=0;
   
	i=0;
	for(j=0;j<25;j++)
		rec_buf[j]=0;
	for(j=0;j<12;j++)
		snd_buf[j]=0;
	
	 	snd_buf[0]= 0xEF;
        snd_buf[1]= 0x01;  //header
        snd_buf[2]= 0xFF;
        snd_buf[3]= 0xFF;
        snd_buf[4]= 0xFF;
        snd_buf[5]= 0xFF;  //adress
        snd_buf[6]= 0x01;  //cmd select
        snd_buf[7]= 0x00;
        snd_buf[8]= 0x03;  //length
        snd_buf[9]= 0x05; //cmd for
        snd_buf[10]=0x00;
        snd_buf[11]=0x09; //check sum

	
	send_packet(snd_buf,12);
	while(frame_ready==0)
	{
        delay_ms(1);
        timeout++;
        if(timeout > 1000) return 1;
    }

	if(rec_buf[9]==0x00)
		return 0;
	else
		return 1;

	
}
s8 store(s8 id)
{
	u16 sum,timeout = 0;
	frame_ready=0;
	i=0;
	for(j=0;j<25;j++)
		rec_buf[j]=0;
	for(j=0;j<15;j++)
		snd_buf[j]=0;
	
		snd_buf[0]= 0xEF;
        snd_buf[1]= 0x01;  //header
        snd_buf[2]= 0xFF;
        snd_buf[3]= 0xFF;
        snd_buf[4]= 0xFF;
        snd_buf[5]= 0xFF;  //adress
        snd_buf[6]= 0x01;  //cmd select
        snd_buf[7]= 0x00;
        snd_buf[8]= 0x06;  //length
        snd_buf[9]= 0x06; //cmd for
        snd_buf[10]=0x01;//buffer1
        snd_buf[11]=0x00;
        snd_buf[12]=id;

		sum = 0x01 + 0x00 + 0x06 + 0x06 + 0x01 + 0x00 + id;

        snd_buf[13]= sum>>8;
        snd_buf[14]= sum&0xff; //check sum
	
	send_packet(snd_buf,15);
	while(frame_ready==0)
	{
        delay_ms(1);
        timeout++;
        if(timeout > 1000) return 1;
    }

	if(rec_buf[9]==0x00)
		return 0;
	else
		return 1;

}
s8 load_char(s8 id)
{
	u16 sum,timeout = 0;

    frame_ready = 0;

    i = 0;

    for(j=0;j<25;j++)
        rec_buf[j]=0;

    for(j=0;j<15;j++)
        snd_buf[j]=0;
		
	snd_buf[0]=0xEF;
    snd_buf[1]=0x01;

    snd_buf[2]=0xFF;
    snd_buf[3]=0xFF;
    snd_buf[4]=0xFF;
    snd_buf[5]=0xFF;

    snd_buf[6]=0x01;

    snd_buf[7]=0x00;
    snd_buf[8]=0x06;

    snd_buf[9]=0x07;   // LoadChar
    snd_buf[10]=0x02;  // Buffer2

    snd_buf[11]=0x00;
    snd_buf[12]=id;

    sum = 0x01 + 0x00 + 0x06 + 0x07 + 0x02 + 0x00 + id;

    snd_buf[13]=(sum >> 8);
    snd_buf[14]=(sum & 0xFF);	

	send_packet(snd_buf,15);

    while(frame_ready == 0)
    {
        delay_ms(1);
        timeout++;
        if(timeout > 1000) return 1;
    }

    return (rec_buf[9] == 0x00) ? 0 : 1;
}

s8 match(void)
{
    u16 timeout = 0;

    frame_ready = 0;
    i = 0;

    for(j=0;j<25;j++)
        rec_buf[j]=0;

    for(j=0;j<12;j++)
        snd_buf[j]=0;

    snd_buf[0]=0xEF;
    snd_buf[1]=0x01;

    snd_buf[2]=0xFF;
    snd_buf[3]=0xFF;
    snd_buf[4]=0xFF;
    snd_buf[5]=0xFF;

    snd_buf[6]=0x01;

    snd_buf[7]=0x00;
    snd_buf[8]=0x03;

    snd_buf[9]=0x03;   // Match

    snd_buf[10]=0x00;
    snd_buf[11]=0x07;

    send_packet(snd_buf,12);

    while(frame_ready == 0)
    {
        delay_ms(1);
        timeout++;
        if(timeout > 100) return 1;
    }

    return (rec_buf[9] == 0x00) ? 0 : 1;
}
s8 delete(s8 id)
{
  	u16 sum,timeout = 0;

    frame_ready = 0;

    i = 0;

    for(j=0;j<25;j++)
        rec_buf[j]=0;

    for(j=0;j<15;j++)
        snd_buf[j]=0;
		
	snd_buf[0]=0xEF;
    snd_buf[1]=0x01;

    snd_buf[2]=0xFF;
    snd_buf[3]=0xFF;
    snd_buf[4]=0xFF;
    snd_buf[5]=0xFF;

    snd_buf[6]=0x01;

    snd_buf[7]=0x00;
    snd_buf[8]=0x07;

    snd_buf[9]=0x0C;   // LoadChar

    snd_buf[10]=0x00;  
    snd_buf[11]=id;  //id

    snd_buf[12]=0x00;
	snd_buf[13]= 0x01;	 //no. of templates

    sum = 0x01 + 0x00 + 0x07 + 0x0c + 0x00 + id + 0x00 +0x01;

    snd_buf[14]=(sum >> 8)&0xff;
    snd_buf[15]=(sum & 0xFF);	

	send_packet(snd_buf,16);

    while(frame_ready == 0)
    {
        delay_ms(1);
        timeout++;
        if(timeout > 1000) return 1;
    }

    return (rec_buf[9] == 0x00) ? 0 : 1;
}
s8 enroll(s8 id)
{
	s8 ret,retry=0;

    cmd_lcd(0x01);
    str_lcd("PLACE FINGER ");
    char_lcd(2);

    // WAIT FOR FINGER
	while(Genimg() != 0)
	{
   	 delay_ms(50);
   	 retry++;
    	if(retry > 100)
    	{
    	    cmd_lcd(0x01);
			str_lcd("Timeout");
			delay_ms(500);
        	return 1;
    	}
	}

	// FIRST CAPTURE
    ret = Genimg();
    if(ret != 0)
    {
        cmd_lcd(0x01);
		str_lcd("GenImg Fail");
		delay_ms(500);
        return 1;
    }

	ret = img2tp1();
    if(ret != 0)
    {
        cmd_lcd(0x01);
		str_lcd("Tz1 Fail");
		delay_ms(500);
        return 1;
    }

	cmd_lcd(0x01);
    str_lcd("Remove Finger");

    //WAIT UNTIL REMOVED
    while(Genimg() == 0)
    {
        delay_ms(50);
    }
	delay_ms(1000);
	cmd_lcd(0x01);
    str_lcd("PLACE AGAIN ");
	char_lcd(2);

    // WAIT AGAIN FOR FINGER
    while(Genimg() != 0)
    {
        delay_ms(50);
    }

    //  SECOND CAPTURE
    ret = Genimg();
    if(ret != 0)
    {
        cmd_lcd(0x01);
		str_lcd("GenImg2 Fail");
		delay_ms(500);
        return 1;
    }

	ret = img2tp2();
    if(ret != 0)
    {
        cmd_lcd(0x01);
		str_lcd("Tz2 Fail");
		delay_ms(500);
        return 1;
    }

    // CREATE TEMPLATE
    ret = regmodel();
    if(ret != 0)
    {
		cmd_lcd(0x01);
		str_lcd("Reg Fail");
		delay_ms(500);
        return 1;
    }

	
    // STORE
    ret = store(id);
    if(ret != 0)
    {
        cmd_lcd(0x01);
		str_lcd("Store Fail");
		delay_ms(500);
        return 1;
    }

   	cmd_lcd(0x01);
    	str_lcd("ENROLL DONE ");
		char_lcd(1);
	delay_ms(500);
    	return 0;



}
/*s8 enroll(s8 id)
{
	s8 ret=0;
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("place finger");
	delay_ms(2000);
	while((ret =Genimg())==1);
	delay_ms(1000);
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("remove finger");
	delay_ms(100);
	if(ret ==0)
	{
		ret = img2tp1();
		if(ret ==0)
		{
			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("place Again");
			while((ret =Genimg())==1);
			delay_ms(1000);
			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("remove finger");
			delay_ms(100);
			if(ret==0)
			{
				ret = img2tp2();
				if(ret ==0)
				{
					ret =regmodel();
					if(ret==0)
					{
						ret=store(id);
						if(ret==0)
						{
							cmd_lcd(0x01);
							str_lcd("woow nice ");
							char_lcd(id+48);
							cmd_lcd(0xc0);
							str_lcd("stored succesfully");
							delay_ms(1000);
							return 0;
						}
						else
						{
							cmd_lcd(0x01);
							str_lcd("store:fail");	
							delay_ms(500);
						}
					}
					else
					{
						cmd_lcd(0x01);
						str_lcd("store:fail");	
						delay_ms(500);
					}

				}
				else
				{
					cmd_lcd(0x01);
					str_lcd("store:fail");	
					delay_ms(500);	
				}	
			}	
			else
			{
				cmd_lcd(0x01);
				str_lcd("store:fail");	
				delay_ms(500);
			}
		}
	}
   return 1;
} 
*/
s8 verify(s8 id)
{
	 s8 ret;

    cmd_lcd(0x01);
    str_lcd("PLACE FINGER ");
	char_lcd(2);

	// WAIT FOR FINGER
    while(Genimg() != 0)
        delay_ms(50);

    // CAPTURE
    ret = Genimg();
    if(ret != 0)
    {
        cmd_lcd(0x01);
		str_lcd("Gen Fail");
		delay_ms(500);
        return 1;
    }

	// CONVERT a† Buffer1
    ret = img2tp1();
    if(ret != 0)
    {
        cmd_lcd(0x01);
		str_lcd("Tz Fail");
		delay_ms(500);
        return 1;
    }
	
    // LOAD ID at Buffer2
    ret = load_char(id);
    if(ret != 0)
    {
        cmd_lcd(0x01);
		str_lcd("Load Fail");
		delay_ms(500);
        return 1;
    }

	// MATCH
    ret = match();
	if(ret ==0)
	{
		return 0;
	}
	else 
		return 1;
	
}
