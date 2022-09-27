#include<lpc214x.h>
#include<stdio.h>

void delay_ms(int ms){

	int i, j;

	for(j=0;j<ms;j++){
		for(i=0;i<12000;i++);
	}
}

int main(){
	
	IO1DIR=0x00FF0000;// GPIO pins P1.16-P1.23 configured as O/P
	
	while(1){
		
		IO1SET=0x00550000;
		delay_ms(500);
		IO1CLR=0X00550000;
		
		IO1SET=0X00AA0000;
		delay_ms(500);
		IO1CLR=0x00AA0000;
	}
	return 0;
}
