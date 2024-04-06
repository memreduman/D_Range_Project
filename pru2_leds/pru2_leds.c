//
//
#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

#define P8_03   (1<<10)		/* pr2_pru0 */
#define P8_04   (1<<11)		/* pr2_pru0 */
#define P8_05   (1<<6)		/* pr2_pru0 */
#define P8_06   (1<<7)		/* pr2_pru0 */
#define P8_08   (1<<20)		/* pr2_pru0 */
#define P8_17   (1<<15)		/* pr2_pru0 */
#define P8_20   (1<<3)		/* pr2_pru0 */
#define P8_21   (1<<2)		/* pr2_pru0 */
#define P8_22   (1<<9)		/* pr2_pru0 */
#define P8_23   (1<<8)		/* pr2_pru0 */
#define P8_24   (1<<5)		/* pr2_pru0 */
#define P8_25   (1<<4)		/* pr2_pru0 */
#define P8_28   (1<<17)		/* pr2_pru0 */
#define P8_29   (1<<18)		/* pr2_pru0 */
#define P8_30   (1<<19)		/* pr2_pru0 */
#define P8_31   (1<<11)		/* pr2_pru0 */
#define P8_33   (1<<10)		/* pr2_pru0 */
#define P8_34   (1<<8)		/* pr2_pru0 */
#define P8_35   (1<<9)		/* pr2_pru0 */
#define P8_36   (1<<7)		/* pr2_pru0 */
#define P8_37   (1<<5)		/* pr2_pru0 */
#define P8_38   (1<<6)		/* pr2_pru0 */
#define P8_39   (1<<3)		/* pr2_pru0 */
#define P8_40   (1<<4)		/* pr2_pru0 */
#define P8_41   (1<<1)		/* pr2_pru0 */
#define P8_42   (1<<2)		/* pr2_pru0 */
#define P8_44   (1<<0)		/* pr2_pru0 */
#define P9_11   (1<<14)		/* pr2_pru0 */
#define P9_13   (1<<15)		/* pr2_pru0 */

#define PRU2_DRAM       0x00000         // Offset to DRAM
// Skip the first 0x200 byte of DRAM since the Makefile allocates
// 0x100 for the STACK and 0x100 for the HEAP.
volatile unsigned int *pru2_dram = (unsigned int *) (PRU2_DRAM + 0x200);


#define START_COMMAND   0x1AD37ACD
#define ACKNOWLEDGE   	0x0ABEF6FF
#define Left_msg		0x00000001
#define Right_msg		0x00000002
#define Stop_msg		0x00000003
#define Back_msg		0x00000004
#define delay_time			22400000 // 0.5 s

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
        /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
        CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

        __R30 &=~(P8_03|P8_04|P8_05);

        while(pru2_dram[0]!=START_COMMAND);
        pru2_dram[1]=msg;

        __R30 |= P8_23;

    while(1) {

    	switch(pru2_dram[2]){
    	 case Left_msg:
    		 __R30 &= ~(P9_13);
    		 __R30 |= (P8_05);
    		 delay(delay_time);
    		 __R30 |= (P8_04);
    		 delay(delay_time);
    		 __R30 |= (P8_03);
    		 delay(delay_time);
    		 __R30 &=~(P8_03|P8_04|P8_05);
    		 delay(delay_time+delay_time);
    		 break;
    	 case Right_msg:
    		 __R30 &= ~(P9_13);
    		 __R30 |= (P8_20);
    		 delay(delay_time);
    		 __R30 |= (P8_21);
    		 delay(delay_time);
    		 __R30 |= (P8_22);
    		 delay(delay_time);
    		 __R30 &=~(P8_20|P8_21|P8_22);
    		 delay(delay_time+delay_time);
    		 break;
    	 case Stop_msg:
    		 __R30 |= (P9_13);
    		 delay(4*delay_time);
    		 break;
    	 case Back_msg:
    	     __R30 |= (P9_13);
    	     delay(4*delay_time);
    	     __R30 &= ~(P9_13);
			 delay(4*delay_time);
			 break;
    	 default:
    		 __R30 &= ~(P9_13);
    		 __R30 &=~(P8_03|P8_04|P8_05);
    		 __R30 &=~(P8_20|P8_21|P8_22);
    		 break;
    	}
    	pru2_dram[2]=0;

    }


}

