#include<lpc214x.h>

void I2C_INIT(){
	
	PCONP	|= 0x00000080;		/* Enables power for I2C -- Might not be needed*/	
	PINSEL0 |= 0x00000050;	/* (P0.3) 7:6 = 01 -> SDA0 is selected, (P0.2) 5:4 = 01 -> SCL0 is selected  */
	
	I2C0CONCLR = 0xff;			/* Clear all bits of I2C0STAT*/
	I2C0CONSET = 0x40;			/* 6 = 1 -> ENable i2c, 5 = 0 -> (STA) Start flag, 4 = 0 (STO) Stop flag , 3 = 0 -> (SI) Interrupt, 2 = (AA) Acknowledge*/
	I2C0SCLH = 15;					/* i2c_bit_freq = pclk/(i2c0sclh + i2c0scll), i2c_bit_freq max = 400khz */
	I2C0SCLL = 135;					/* assuming pckl = 60Mhz, i2c_bit_frew = 400khz => i2c0sclh + i2c0scll = 150*/
}

void I2C_WRITE(char dev_addr, char data, unsigned short addr){
	
	I2C0CONSET = 0x20;	// Start
	
	while(I2C0STAT != 0x08);
	
	I2C0DAT = (((dev_addr & 0xf8) | ((addr & 0x0300) >> 7)) & 0xfe);
	I2C0CONCLR = 0x38;	// 0 0 0 X
	
	while(I2C0STAT != 0x18);
	
	I2C0DAT = addr;
	I2C0CONCLR = 0x3C;	// 0 0 0 X
	
	while(I2C0STAT != 0x28);
	
	I2C0DAT = data;
	I2C0CONCLR = 0x3C;	// 0 0 0 X
	
	while(I2C0STAT != 0x28);
	
	I2C0CONCLR = 0x2C;	// 1 0 0 X
	I2C0CONSET = 0x10;	// Stop

}

char I2C_READ(char dev_addr, unsigned short addr){
	
	char data;
	
	I2C0CONSET = 0x20;	// Start // Dummy Write
	
	while(I2C0STAT != 0x08);
	
	I2C0DAT = (dev_addr & 0xfe);
	I2C0CONCLR = 0x08;
	
	while(I2C0STAT != 0x18);
	
	I2C0DAT = addr;
	I2C0CONCLR = 0x3C;
	
	while(I2C0STAT != 0x28);	
	
	I2C0CONSET = 0x20;	// Repeated Start	// Read
	I2C0CONCLR = 0x18;
	
	while(I2C0STAT != 0x10);
	
	I2C0DAT = (dev_addr | 0x01);

	I2C0CONCLR = 0x18;
	
	while(I2C0STAT != 0x40);

	I2C0CONCLR = 0x3C;
	
	while(I2C0STAT != 0x58);
	
	data = I2C0DAT;	
	
	I2C0CONSET = 0x10;	// Stop
	I2C0CONCLR = 0x28;
	
	return data;
}
