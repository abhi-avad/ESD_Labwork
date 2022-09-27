#include<lpc214x.h>
#include<stdio.h>
#include<string.h>
#include "lcd.h"
#include "i2c.h"

// EEPROM used -> AT24C08N // pin A2 is connected to GND
// EEPROM address = 1010(A2)000 = 1010000 = 0xA0 = 0x14 << 5

void LCD_PRINT_LINE(char* msg){

	LCD_CHAR(0x01, 0);  /* Display clear */
	LCD_CHAR(0x80, 0);  /* Cursor at home position */	
	LCD_STRING(msg);
	delay_ms(1000);
	LCD_CHAR(0x01, 0);  /* Display clear */
	LCD_CHAR(0x80, 0);  /* Cursor at home position */	
}

void EEPROM_I2C_WRITE(char dev_addr, char data, unsigned short addr){
	
	char eff_dev_addr;
	
	eff_dev_addr = ((dev_addr & 0xf8) | ((addr & 0x0300) >> 7));
	
	I2C_WRITE(eff_dev_addr, data, addr);
}

char EEPROM_I2C_READ(char dev_addr, unsigned short addr){
	
	char eff_dev_addr, result;
	
	eff_dev_addr = ((dev_addr & 0xf8) | ((addr & 0x0300) >> 7));
	
	result = I2C_READ(eff_dev_addr, addr);
	
	return result;
}

int main(void){

	char store;
	char lcd_tx[16];
	
	I2C_INIT();
	LCD_INIT();
	
	EEPROM_I2C_WRITE(0xA0, 0x41, 0x000F);
	snprintf(lcd_tx, 16,"%c -> 0x00F",0x41);
	LCD_PRINT_LINE(lcd_tx);
	
	EEPROM_I2C_WRITE(0xA0, 0x5A, 0x001F);
	snprintf(lcd_tx, 16,"%c -> 0x01F",0x5A);
	LCD_PRINT_LINE(lcd_tx);

	store = EEPROM_I2C_READ(0xA0, 0x000F);
	snprintf(lcd_tx, 16,"%c <- 0x00F", store);
	LCD_PRINT_LINE(lcd_tx);
	
	store = EEPROM_I2C_READ(0xA0, 0x001F);
	snprintf(lcd_tx, 16,"%c <- 0x01F", store);
	LCD_PRINT_LINE(lcd_tx);
	
	return 0;
}

// ----- OLD CODE -----

//void EEPROM_WRITE(char dev_addr, char data, unsigned short addr){	// dev_addr = 5 bit i2c address // data = 8 bits // addr = 10 bit memory address
//	
//	I2C0CONCLR = 0xff;
//	I2C0CONSET = 0x40;
//	
//	I2C0CONSET = 0x20; /* 5 = 1 -> set start flag, i2c start*/
//	
//	while(I2C0STAT != 0x08); /* Wait for SI to be set => START has been succesfully transmitted */
//	
//	// I2CDAT Can only be read from or written to while SI is set
//	I2C0DAT = ((dev_addr & 0xf8) | ((addr & 0x0300) >> 7)); /* 7:3 = i2c_device_addr, 2:1 = MSB bits of memory address inside EEPROM, 0 = 0 (R/W) - bitwise_& ensures last bit is write */
//	I2C0CONCLR = 0x2C; /* Clear START and SI*/
//	
//	while(I2C0STAT != 0x18);
//	
//	I2C0DAT = addr;
//	I2C0CONCLR = 0x2C; /* Clear START and SI*/
//	
//	while(I2C0STAT != 0x28);
//	
//	I2C0DAT = data;
//	I2C0CONCLR = 0x2C; /* Clear START and SI*/
//	
//	while(I2C0STAT != 0x28);
//	
//		I2C0CONCLR = 0x2C;
//		I2C0CONSET = 0x10; /* 4 = 1 -> set stop flag, i2c stop */
//	
//}

//char EEPROM_READ(char dev_addr, unsigned short addr){
//	
//	char result;
//	char lcd_tx[16];
//	
//	I2C0CONCLR = 0xff;
//	I2C0CONSET = 0x40;
//	
//	// ---------- DUMMY WRITE -----------
//	I2C0CONSET = 0x20; /* 5 = 1 -> set start flag, i2c start*/
//	
//	while(!(I2C0STAT & 0x08)); /* Wait for SI to be set => START has been succesfully tx */
//	//I2C0CONCLR = 0x28;	
//	
//	// I2CDAT Can only be read from or written to while SI is set
//	I2C0DAT = ((dev_addr & 0xf8) | ((addr & 0x0300) >> 7) & 0xfe); /* 7:1 = device_addr, 0 = 0 (R/W) - bitwise_& ensures last bit is write */
//	I2C0CONCLR = 0x2c; /* Clear START and SI*/
//	
//	while(!(I2C0STAT & 0x18));
//	
//	I2C0DAT = addr;
//	I2C0CONCLR = 0x2c; /* Clear START and SI*/
//	
//	while(!(I2C0STAT & 0x28));
//	
//	// ---------- CURRENT READ -----------
//	I2C0CONSET = 0x20; /* 5 = 1 -> set start flag, i2c start*/
//	
//	while(!(I2C0STAT & 0x08));
//	//I2C0CONCLR = 0x2c;
//	
//	I2C0DAT = ((dev_addr & 0xf8) | ((addr & 0x0300) >> 7) | 0x01); /* 7:1 = device_addr, 0 = 1 (R/W) - bitwise_or ensures last bit is read */
//	I2C0CONCLR = 0x2c;	/* Clear START and SI*/
//	I2C0CONCLR = 0x04;
//	
//	while(!(I2C0STAT & 0x40));
//	
//	I2C0CONSET = 0x04;
//	I2C0CONCLR = 0x08;
//	while(!(I2C0STAT & 0x58));
//	result = I2C0DAT;		/* Data read from EEPROM is stored in I2CDAT */
//	
//	snprintf(lcd_tx, 16, "I2C0DAT = %c", result);
//	reached_here(lcd_tx);
//	
//	I2C0CONCLR = 0x08;
//	I2C0CONSET = 0x10; /* 4 = 1 -> set stop flag, i2c stop */
//	
//	return result;
//	
//}
