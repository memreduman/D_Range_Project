// This code does MAXCH parallel PWM channels.
// All channels start at the same time. It's period is 510ns
#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
//#include "prugpio.h"

#define P8_46 (1<<15)	//input echo P9_16 in AI
#define P8_45 (1<<14)	//output trigger P9_14 in AI

#define PRU1_DRAM       0x00000			// Offset to DRAM for PRU1
#define PRU_SHAREDMEM   0x10000         // Offset to shared memory
// Skip the first 0x200 byte of DRAM since the Makefile allocates
// 0x100 for the STACK and 0x100 for the HEAP.
volatile unsigned int *pru1_dram = (unsigned int *) (PRU1_DRAM + 0x200);


#define START_COMMAND   0x1AD37ACD
#define ACKNOWLEDGE   	0x0ABEF6FF

#define period 1000000 // period for 100 Hz

volatile register uint32_t __R30; //output register
volatile register uint32_t __R31; //input register

void delay(unsigned int time){
	int i;
	unsigned int _time = time/3;
	for(i=0;i<_time;i++) __delay_cycles(1);


}

void main(void)
{
    	uint32_t msg=ACKNOWLEDGE;
    	uint64_t LowLevelTime = 0;

    	//P8_46 input for echo !! DONT FORGET TO config-pin P8_46 pruin
    	//P8_45 output for trig !! DONT FORGET TO config-pin P8_46 pruout


        /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
        CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

        while(pru1_dram[0]!=START_COMMAND);
        pru1_dram[1]=msg;
        pru1_dram[2]=0;
        __R30 |= P8_45; // Trigger set to high
        __delay_cycles(100000000); //wait for 0.5s setup of URM37

        while(1){
            __R30 &= ~P8_45; // Trigger set to low
            __delay_cycles(1000);
            __R30 |= P8_45; // Trigger set to high

            while(__R31 & P8_46){
            }; //Wait until echo is low
            //Every 50us low level stands for 1 cm

			//#pragma MUST_ITERATE(5000000)
            while(!(__R31 & P8_46)){ //
            LowLevelTime++; //Count the cycle until echo is high
            //pru1_dram[3]=0x12345;

            }


            if(LowLevelTime >=5000000) pru1_dram[2] = 0xFFFFFFFF;
            else pru1_dram[2]=LowLevelTime/3000; //Write distance into memory (in cm)
            LowLevelTime=0;
            __delay_cycles(100000000);

        }

}

