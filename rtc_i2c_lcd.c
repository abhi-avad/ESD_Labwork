#include<lpc214x.h>
#include<stdio.h>
#include<string.h>
#include "lcd.h"
#include "i2c.h"

// RTC used -> DS1037
// RTC address = 11010000 = 0xD0 = 0x68 << 1

void LCD_PRINT_LINE(char* msg){
	LCD_CHAR(0x01, 0);  /* Display clear */
	LCD_CHAR(0x80, 0);  /* Cursor at home position */	
	LCD_STRING(msg);
	delay_ms(1000);
	LCD_CHAR(0x01, 0);  /* Display clear */
	LCD_CHAR(0x80, 0);  /* Cursor at home position */
}

char bcdToAsciiLow(unsigned char bcd){
	
  char result;
	result = (char) ((bcd & 0x0f) + 48);
  return result;
}

char bcdToAsciiUp(unsigned char bcd){
	
	char result;
	result = (char) (((bcd & 0xf0) >> 4) + 48);
	return result;
}

void setTime(void){
	
	I2C_WRITE(0xD0, 0x00, 0x00);	// Seconds
	I2C_WRITE(0xD0, 0x31, 0x01);	// Minutes
	I2C_WRITE(0xD0, 0x18, 0x02);	// Hours		// 24hr format
	I2C_WRITE(0xD0, 0x01, 0x03);	// DAY
	I2C_WRITE(0xD0, 0x04, 0x04);	// Date
	I2C_WRITE(0xD0, 0x09, 0x05);	// Month
	I2C_WRITE(0xD0, 0x20, 0x06);	// Year
}

/*void setTime(void){
	
	I2C_WRITE(0xD0, 0x00, 0x00);	// Seconds
	I2C_WRITE(0xD0, 0x01, 0x31);	// Minutes
	I2C_WRITE(0xD0, 0x02, 0x18);	// Hours		// 24hr format
	I2C_WRITE(0xD0, 0x03, 0x01);	// DAY
	I2C_WRITE(0xD0, 0x04, 0x04);	// Date
	I2C_WRITE(0xD0, 0x05, 0x09);	// Month
	I2C_WRITE(0xD0, 0x06, 0x20);	// Year
}*/

int main(void){
	
	char sec[2], min[2], hrs[2];
	char time[16];
	
	LCD_INIT();	
	I2C_INIT();
	
	//setTime();
	
	while(1){
		sec[0] = I2C_READ(0xD0, 0x00);
		min[0] = I2C_READ(0xD0, 0x01);
		hrs[0] = I2C_READ(0xD0, 0x02);
		
		sec[1] = bcdToAsciiUp(sec[0]);
		sec[0] = bcdToAsciiLow(sec[0]);
		min[1] = bcdToAsciiUp(min[0]);
		min[0] = bcdToAsciiLow(min[0]);
		hrs[1] = bcdToAsciiUp(hrs[0]);
		hrs[0] = bcdToAsciiLow(hrs[0]);
		
		snprintf(time, 16, "%c%c:%c%c:%c%c",hrs[1],hrs[0],min[1],min[0],sec[1],sec[0]);
		
		LCD_PRINT_LINE(time);
	}
  
	return 0;
}
