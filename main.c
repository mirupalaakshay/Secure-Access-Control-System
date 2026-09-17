#include "lcd.h"
#include "kpm.h"
#include "i2c.h"
#include "i2c_eeprom.h"
#include "uart.h"
#include "r305.h"
#include "delay.h"
#include "exint_menu.h"
#include "i2c_eeprom.h"
#include "types.h"
#include "l293d.h"
extern u8 menu_flag;
main()
{
//	u8 data;
//	char buf[16];
	init_lcd();
	init_kpm();
	init_uart0();
	init_i2c();
	bulid_symbols();
	init_eint2();
	init_ids();
	init_l293d();
	title1();
	while(1)
	{

		if(menu_flag==1)
		 {
		 	menu();
	 	} 
	 	login(); 	 
	}
}

