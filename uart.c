#include"uart.h"
#include "lcd.h"
#include "types.h"
#include<LPC21xx.H>
char ch,dummy;
extern u8 i,rec_buf[25],frame_ready;
void init_uart0(void)
{
	//cfg the p0.0,p0.1 as tx&px pins
	PINSEL0 &=~(15<<0);
	PINSEL0 |=(TXD0_PIN|RXD0_PIN);

	//cfg the  DLAB_BIT in UXLCR  for the change the baud rate 
	U0LCR =(1<<DLAB);
	//cfg the UxLCR for select the word length
	U0LCR |=(WORD_LENGTH_SELECT);

	//cfg the speed/freq/baud rate for communication
	//by using UxDLM and UxDLL
	U0DLM =DIVISOR>>8;
	U0DLL =DIVISOR;
	
	//SETTING THE BAUD RATE IS COMPLETED 
	//reset DLAB_BIT TO START COMMUNICATION 
	U0LCR &=~(1<<DLAB);

	VICIntSelect = 0x00000000; // IRQ
  	VICVectAddr0 = (unsigned)UART0_isr;
  	VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */
 	VICIntEnable = 1 << 6;   /* Enable UART0 Interrupt */
  	U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */  	
}
void UART0_isr(void) __irq
{
  if((U0IIR & 0x04)) //check if receive interrupt
  {
  	
		ch = U0RBR;	/* Read to Clear Receive Interrupt */
		rec_buf[i++] = ch;
		if(i>=12)
		{
			frame_ready=1;
		}	 
  }
  else
  {
      dummy=U0IIR; //Read to Clear transmit interrupt
  }
   VICVectAddr = 0; /* dummy write */
}
void u0_Tx_byte(unsigned char byte)
{
	//data is assigned to the transmitter buffer(u0thr)
	U0THR =byte;
	//wait here upto transmission complete
	while(((U0LSR>>TEMT_BIT)&1)==0);
}
unsigned char u0_Rx_byte(void)
{
	//wait for recieved  signal
	   while(((U0LSR>>DR_BIT)&1)==0);
	//then recived data is assigned to reciver buffer(u0rbr)
	//so return the recived buffer;
	return U0RBR;
}

void u0_Tx_str(char *str)
{
 	 while(*str)
	 {
		  u0_Tx_byte(*str++);
	 }
}
 char* u0_Rx_str(void)
 {
 	static char str[50];
	char i=0;
	do
	{
		str[i]=u0_Rx_byte();
		u0_Tx_byte(str[i]);
		if((str[i]=='\r')||(str[i]=='\n'))
		{
			break;
		}
		i++;
	}while((i!=49));
	str[i-1]=0;
	return str;

 }
