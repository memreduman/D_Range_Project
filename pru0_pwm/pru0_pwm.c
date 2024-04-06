// This code does MAXCH parallel PWM channels.
// All channels start at the same time. It's period is 510ns
#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
//#include "prugpio.h"
#define P9_31 (1<<3) //  P8_12   (1<<3)
#define P9_29 (1<<4) //  P8_11   (1<<4)

#define PRU0_DRAM       0x00000         // Offset to DRAM
// Skip the first 0x200 byte of DRAM since the Makefile allocates
// 0x100 for the STACK and 0x100 for the HEAP.
volatile unsigned int *pru0_dram = (unsigned int *) (PRU0_DRAM + 0x200);


#define START_COMMAND   0x1AD37ACD
#define ACKNOWLEDGE   	0x0ABEF6FF

#define period 2500000 // period for 80 Hz

volatile register uint32_t __R30; //output register
volatile register uint32_t __R31; //input register

void delay(unsigned int time){
	int i;
	unsigned int _time = time/3;
	for(i=0;i<_time;i++) __delay_cycles(1);


}

void main(void)
{
    uint32_t on_time_1=0;
    uint32_t on_time_2=0;
    uint32_t msg=ACKNOWLEDGE;
    uint32_t both_on = P9_31 | P9_29; //DONT FORGET TO CONFIGURE PINS
    // p9_31 duty_1
    // p9_29 duty 2
        /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
        CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
        __R30 &= ~both_on;

        while(pru0_dram[0]!=START_COMMAND);
        pru0_dram[1]=msg;

    while(1) {

    	while((on_time_1==0)||(on_time_2==0)){
    		on_time_1=pru0_dram[2];
    		on_time_2=pru0_dram[3];
    	}

    	on_time_1=pru0_dram[2];
        on_time_2=pru0_dram[3];

    	if(on_time_1>on_time_2){
                __R30 |= both_on;      // Set the GPIO pin to 1
                delay(on_time_2);
                __R30 &= ~(P9_29);     // Clear
                delay(on_time_1-on_time_2);
                __R30 &= ~(P9_31);     // Clear
                delay(period-on_time_1);
    		}
    	else if(on_time_1<on_time_2){
    		    __R30 |= both_on;      // Set the GPIO pin to 1
                delay(on_time_1);
                __R30 &= ~(P9_31);     // Clear
                delay(on_time_2-on_time_1);
                __R30 &= ~(P9_29);     // Clear
                delay(period-on_time_2);
    		}
    	else{
    	       __R30 |= both_on;      // Set the GPIO pin to 1
    	       delay(on_time_1);
    	       __R30 &= ~both_on;
    	       delay(period-on_time_1);
    	    		}

        }
}

