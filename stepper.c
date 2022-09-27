#include<lpc214x.h>
#include<stdint.h>

// Yellow = 12V, Connect the rest in the same order (Yellow top to Red bottom) - OP1 to OP4

void delay_ms(int x){	//1s 12 clock cycle
	
	int i,j;
	
	for(i=0;i<x;i++)
		for(j=0;j<120000;j++);
}

void half_step(){	// each step = 3.75 degrees
	
	int i,j;
	uint32_t val[8] = {0x008C0000, 0x00CC0000, 0x004C0000, 0x006C0000, 0x002C0000, 0x003C0000, 0x001C0000, 0x009C0000};
	//										1000				1100				0100				0110				0010				0011				0001				1001
	// 										Always keep ENable ON => 1100 = C
	
	for(i=0;i<12;i++){	// clockwise full rotation		
		for(j=0;j<8;j++){
			IO0SET = val[j];
			delay_ms(10);
			IO0CLR = val[j];
		}
	}
	for(i=0;i<12;i++){	// counter clockwise full rotation
		for(j=7;j>=0;j--){
			IO0SET = val[j];
			delay_ms(10);
			IO0CLR = val[j];
		}
	}
}

void full_step(void){	// each step = 7.5 degrees
	
	int i,j;
	uint32_t val[4] = {0x008C0000, 0x004C0000, 0x002C0000, 0x001C0000};
	//										1000				0100				0010				0001
	
	for(i=0;i<12;i++){	// clockwise full rotation	
		for(j=0;j<4;j++){
			IO0SET = val[j];
			delay_ms(10);
			IO0CLR = val[j];
		}
	}
	for(i=0;i<12;i++){	// counter clockwise full rotation
		for(j=3;j>=0;j--){
			IO0SET = val[j];
			delay_ms(10);
			IO0CLR = val[j];
		}
	}
}

int main(){
	
	IO0DIR = 0x00FC0000; // P0.23 to 18 = IN4,IN3,IN2,IN1,ENA,ENB configured as GPIO outputs
	
	while(1){
		//half_step();
		full_step();
	}
	return 0;
}