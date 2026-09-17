#ifndef _LCD_H_
#define _LCD_H_
#include<string.h>
#include "types.h"
#define DATA 8
#define RS 16
#define RW 18
#define EN 17
#define LCD_CLR 0X01
#define RET_CUR_HOME 0X02
//DISPLAY ON/OFF
#define DISP_OFF 0X08
#define DISP_ON 0X0C
#define DISP_ON_CUR_ON_BLK_ON 0X0F
#define DISP_ON_CUR_ON 0X0E
//ENRTY MODE SET
#define DISP_SHIFT_OFF_INC 0X06
#define DISP_SHIFT_INC 0X07
//FUNCTION SET
#define MODE_8BIT_LINE1 0X30
#define MODE_8BIT_LINE2 0X38
#define MODE_4BIT_LINE1 0X20
#define MODE_4BIT_LINE2 0X28
//
#define GOTO_LINE1_POS_0 0X80
#define GOTO_LINE2_POS_0 0XC0
#define GOTO_LINE3_POS_0 0X94
#define GOTO_LINE4_POS_0 0XD4
//
#define SHIFT_CUR_LEFT	0X10
#define SHIFT_CUR_RIGHT 0X14
//
#define GOTO_CGRAM 0X40

//function declaration
void write_lcd(u8);
void cmd_lcd(u8);
void init_lcd(void);
void char_lcd(u8);
void str_lcd(u8*);
void u32_lcd(u32);
void hex_lcd(u32);
void bulid_CGRAM(u8*,u8);
void float_lcd(f32,s32);
void s32_lcd(s32);
void bulid_symbols(void);
void title1(void);
void title2(void);
#endif




