//uart defines

#define FOSC 12000000
#define PLL  5
#define CCLK (FOSC*PLL)
#define PCLK (CCLK/4)
#define BAUD 9600
#define DIVISOR (PCLK/(16*BAUD))


//PIN CONNECT BLOCK DEFINE
#define TXD0_PIN 0x00000001
#define RXD0_PIN 0x00000004

//DEFINES FOR UxLCR
#define DLAB 7
#define _8BIT 3
#define WORD_LENGTH_SELECT _8BIT

//DEFINES FOR UxLSP
#define TEMT_BIT 6
#define DR_BIT  0


//FUNCTIONN DECLARATIONS
void init_uart0(void);//intialization
void u0_Tx_byte(unsigned char byte);
unsigned char u0_Rx_byte(void);
void u0_Tx_str(char *str); 
char* u0_Rx_str(void);
void UART0_isr(void) __irq;




