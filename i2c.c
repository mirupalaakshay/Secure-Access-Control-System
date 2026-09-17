#include <lpc21xx.h>
#include "types.h"
#include "i2c_defines.h"
void init_i2c(void)
{
	PINSEL0 &= ~(15<<2*2);
	PINSEL0 |=  SCL_PIN_FUNC|SDA_PIN_FUNC;
	I2SCLL = LOAD_VAL;
	I2SCLH = LOAD_VAL;
	I2CONSET = 1<<I2EN;
}
void i2c_start(void)
{
	I2CONSET = 1<<STA;
	
	while(((I2CONSET>>SI)&1)==0);
	
	I2CONCLR =1<<STAC;	

}
void i2c_stop(void)
{
	I2CONSET  = 1<<STO;

	I2CONCLR = 1<<SIC;
}
void i2c_restart(void)
{
	I2CONSET = 1<<STA;

	I2CONCLR = 1<<SIC;

	while(((I2CONSET>>SI)&1)==0);

	I2CONCLR = 1<<STA;
}
void i2c_write(u8 sdat)
{
	I2DAT =sdat;

	I2CONCLR =1<<SIC;

	while(((I2CONSET>>SI)&1)==0);

}
u8 i2c_nack(void)
{
//  I2CONSET = 0<<AA;

	I2CONCLR = 1<<SIC;

	while(((I2CONSET>>SI)&1)==0);

	return I2DAT;
}
u8 i2c_mack(void)
{
	I2CONSET = 1<<AA;

	I2CONCLR = 1<<SIC;

	while(((I2CONSET>>SI)&1)==0);

	I2CONCLR = 1<<AAC;

	return I2DAT;
}
