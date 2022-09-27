#include<lpc214x.h>
#include<stdint.h> //stdint.h used to access 32 and 12 bit integers

uint32_t i=0x00000000;

void PWM5_INIT(uint32_t cycle_rate){

	PINSEL1 = 0x00000400;	// set function of P0.21 as PWM5 O/P
	PWMMR0 = cycle_rate;	// PWMMR0 = (T_on + T_off) = PWM Cycle Rate // Match Register 0 MUST be set before MCR PCR and TCR
	
	PWMMCR = 0x00000003;	// reset(bit 1) and interrupt(bit 0) on pwmmr0
	PWMPCR = 0x00002000;	// enabling pwm output for pwm5(bit 13->pwmena5)
	PWMTCR = 0x00000009;	// bit 0-> counter enable, bit 3->PWM enable
}

int main()
{
	uint32_t i = 0x00000000;
	
	PWM5_INIT(0x000004FF);
	
	PWMPR = 60-1;					// setting resolution of pwm, i.e to 1us =(PWMPR+1)/(60x10^6) // number of times MR needs to match TC before count occurs, hence sets the frequency of square wave
	
	PWMMR5 = 0x00000000;	// PWMMRx = T_off for x channel
	PWMLER = 0x00000020;	// enabling pwmm0(bit 0) and pwmm5(bit5) to transfer from shadow registers
	
	while(1){

		while (i != (PWMMR0/3))	// while loop to increase duty cycle from 0% to 33% in steps of 1
		{
			while(PWMIR != 0x00000001);
			PWMIR = 0x00000001;
			PWMMR5 = i;
			PWMLER = 0x00000020;
			i++;
		}
		while (i != 1)					// while loop to decrease duty cycle from 33% to 0% in steps of 1
		{
			while(PWMIR != 0x00000001);
			PWMIR = 0x00000001;
			PWMMR5 = i;
			PWMLER = 0x00000020;
			i--;
		}
	}
	return 0;
}
