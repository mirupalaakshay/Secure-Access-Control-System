#ifndef __kpm_h__
#define __kpm_h__
#include"types.h"
//rows
#define row0 16
#define row1 17
#define row2 18
#define row3 19
//columns
#define col0 20
#define col1 21
#define col2 22
#define col3 23

//declaration

void init_kpm(void);
u32 colscan(void);
s32 keyscan(void);
u32 rowcheck(void);
u32 colcheck(void);
s32 readnum(void);
//void password_by_kpm(char *p);
void id_kpm(s8 *ptr);
void password_kpm(s8 *ptr);

#endif
