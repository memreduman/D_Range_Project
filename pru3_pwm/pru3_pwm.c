// This code does MAXCH parallel PWM channels.
// All channels start at the same time. It's period is 510ns
#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

//P8_09 left wheel
//P8_07 right wheel

//P9_28 en1 -> ON  : Forward
//P9_29 en2 -> OFF :

//P9_30 en3 -> OFF :
//P9_31 en4 -> ON  : Forward

#define PRU3_DRAM       0x00000         // Offset to DRAM
// Skip the first 0x200 byte of DRAM since the Makefile allocates
// 0x100 for the STACK and 0x100 for the HEAP.
volatile unsigned int *pru3_dram = (unsigned int *) (PRU3_DRAM + 0x200);


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
    uint32_t direction=1; //FORWARD
    uint32_t msg=ACKNOWLEDGE;
    uint32_t both_on = P8_09 | P8_07; //DONT FORGET TO CONFIGURE PINS
    // P8_09 duty_1
    // P8_07 duty 2
        /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
        CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
        __R30 &= ~(both_on | P9_28 | P9_29 | P9_30 | P9_31);

        while(pru3_dram[0]!=START_COMMAND);
        pru3_dram[1]=msg;

    while(1) {

    	while((on_time_1==0)||(on_time_2==0)){
    		on_time_1=pru3_dram[2];
    		on_time_2=pru3_dram[3];
    	}

    	on_time_1=pru3_dram[2];
        on_time_2=pru3_dram[3];
        direction=pru3_dram[4];

    	if(on_time_1>on_time_2){
    			__R30 |= (P9_28 | P9_31);
    			__R30 &= ~(P9_29 | P9_30);

                __R30 |= both_on;      // Set the GPIO pin to 1
                delay(on_time_2);
                __R30 &= ~(P8_07);     // Clear
                delay(on_time_1-on_time_2);
                __R30 &= ~(P8_09);     // Clear
                delay(period-on_time_1);
    		}
    	else if(on_time_1<on_time_2){
    			__R30 |= (P9_28 | P9_31);
    			__R30 &= ~(P9_29 | P9_30);

    		    __R30 |= both_on;      // Set the GPIO pin to 1
                delay(on_time_1);
                __R30 &= ~(P8_09);     // Clear
                delay(on_time_2-on_time_1);
                __R30 &= ~(P8_07);     // Clear
                delay(period-on_time_2);
    		}
    	else{

    		if(direction){
    		   __R30 |= (P9_28 | P9_31);
    		   __R30 &= ~(P9_29 | P9_30);

     	       __R30 |= both_on;      // Set the GPIO pin to 1
     	       delay(on_time_1);
     	       __R30 &= ~both_on;
     	       delay(period-on_time_1);
    			}
    		else if(!direction){
     		   __R30 &= ~(P9_28 | P9_31);
     		   __R30 |=  (P9_29 | P9_30);

      	       __R30 |= both_on;      // Set the GPIO pin to 1
      	       delay(on_time_1);
      	       __R30 &= ~both_on;
      	       delay(period-on_time_1);
    			}

    	    }

        }
}

