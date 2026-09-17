#include "types.h"
#include "i2c.h"
#include "delay.h"
void i2c_eeprom_write_byte(u8 SlaveAddr,
						   u16 BuffAddr,
						   u8 data)
{
		i2c_start();
		i2c_write(SlaveAddr<<1);
		i2c_write(BuffAddr>>8);
		i2c_write(BuffAddr);
		i2c_write(data);
		i2c_stop();
		delay_ms(10);
}
u8 i2c_eeprom_read_byte(u8 SlaveAddr,
						u16 BufferAddr)
{
	u8 data	 ;
	i2c_start();
	i2c_write(SlaveAddr<<1);
	i2c_write(BufferAddr>>8);
	i2c_write(BufferAddr);
	i2c_restart();
	i2c_write(SlaveAddr<<1|1);
	data =i2c_nack();
	i2c_stop();
	return data;
}
void i2c_eeprom_write_page(u8 SlaveAddr,
						   u16 BuffAddr,
						   s8 *p,
						   u8 nbytes)
{
	u8 i;
	i2c_start();
	i2c_write(SlaveAddr<<1);
	i2c_write(BuffAddr>>8);
	i2c_write(BuffAddr);
	for(i=0;i<nbytes;i++)	
	{
		i2c_write(p[i]);
	}
	i2c_stop();
	delay_ms(10);
}
void i2c_eeprom_seq_read(u8 SlaveAddr,
						 u16 BuffAddr,
						 s8 *p,
						 u8 nbytes
						)
{
		u8 i;
		i2c_start();
		i2c_write(SlaveAddr<<1);
		i2c_write(BuffAddr>>8);
		i2c_write(BuffAddr);
		i2c_restart();
		i2c_write(SlaveAddr<<1|1);
		for(i=0;i<nbytes-1;i++)
		{
			p[i] = i2c_mack();
		}
			p[i]  =i2c_nack();
			i2c_stop();
			delay_ms(10);
}
