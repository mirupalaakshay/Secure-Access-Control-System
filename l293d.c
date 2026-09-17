#include<lpc21xx.h>
#include "macros.h"
#include "l293d.h"
void init_l293d(void)
{
	IODIR0 |= ((1<<M1)|(1<<M2));
}
void door_open(void)
{
	 G_SETBIT(IOSET0,M1);
	 G_SETBIT(IOCLR0,M2);

}
void door_close(void)
{
	 G_SETBIT(IOCLR0,M1);
	 G_SETBIT(IOSET0,M2);
}
void motor_stop(void)
{
	 G_SETBIT(IOCLR0,M1);
	 G_SETBIT(IOCLR0,M2);
}
