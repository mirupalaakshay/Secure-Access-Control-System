//i2c defines
#define SCL 2
#define SDA 3

//values of the pinsel0 for pin function
#define SCL_PIN_FUNC 0X00000010
#define SDA_PIN_FUNC 0X00000040

//clock defines	        
#define FOSC 	   12000000
#define CCLK 	  (FOSC*5)
#define PCLK 	  (CCLK/4)
#define I2C_SPEED 100000
#define LOAD_VAL  ((PCLK/I2C_SPEED)/2)

//sfr I2CONSET
#define AA   2
#define SI   3
#define STO  4
#define STA  5
#define I2EN 6

//sfr I2CONCLR
#define AAC    2
#define SIC    3
#define STAC   5
#define I2ENC  6
