// This file defines the GPIO port addresses and PRU address
// See the cape interface spec page for more info on PRU
// https://elinux.org/Beagleboard:BeagleBone_cape_interface_spec#PRU
#warning "Found AI"
// These are addresses for the am5729
// First define the 9 GPIO port addresses
#define GPIO1 0x4AE10000
#define GPIO2 0x48055000
#define GPIO3 0x48057000
#define GPIO4 0x48059000
#define GPIO5 0x4805B000
#define GPIO6 0x4805D000
#define GPIO7 0x48051000
#define GPIO8 0x48053000

// These define which bits in a given port go to the USR LEDs
// GPIO3
#define USR0 (1<<17)
#define USR2 (1<<15)
#define USR3 (1<<14)
#define USR4 (1<< 7)
// GPIO5
#define USR1 (1<< 5)
// PRU header pins
// pru1_0
#define P8_11   (1<<4)		/* pr1_pru0 */
#define P8_12   (1<<3)		/* pr1_pru0 */
#define P9_15   (1<<5)		/* pr1_pru0 */
#define P9_26   (1<<17)		/* pr1_pru0 */
// pru1_1
#define P8_13   (1<<7)		/* pr1_pru1 */
#define P8_14   (1<<9)		/* pr1_pru1 */
#define P8_15   (1<<16)		/* pr1_pru1 */
#define P8_16   (1<<18)		/* pr1_pru1 */
#define P8_18   (1<<5)		/* pr1_pru1 */
#define P8_19   (1<<6)		/* pr1_pru1 */
#define P8_26   (1<<17)		/* pr1_pru1 */
#define P9_14   (1<<14)		/* pr1_pru1 */
#define P9_16   (1<<15)		/* pr1_pru1 */
#define P9_19   (1<<2)		/* pr1_pru1 */
#define P9_20   (1<<1)		/* pr1_pru1 */
#define P9_27   (1<<11)		/* pr1_pru1 */
#define P9_41   (1<<3)		/* pr1_pru1 */
#define P9_42   (1<<10)		/* pr1_pru1 */
// pru2_0
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
// pru2_1
#define P8_07   (1<<16)		/* pr2_pru1 */
#define P8_09   (1<<6)		/* pr2_pru1 */
#define P8_10   (1<<15)		/* pr2_pru1 */
#define P8_27   (1<<17)		/* pr2_pru1 */
#define P8_32   (1<<0)		/* pr2_pru1 */
#define P8_43   (1<<20)		/* pr2_pru1 */
#define P8_45   (1<<18)		/* pr2_pru1 */
#define P8_46   (1<<19)		/* pr2_pru1 */
#define P9_17   (1<<9)		/* pr2_pru1 */
#define P9_18   (1<<8)		/* pr2_pru1 */
#define P9_25   (1<<5)		/* pr2_pru1 */
#define P9_28   (1<<13)		/* pr2_pru1 +in1*/
#define P9_29   (1<<11)		/* pr2_pru1 */
#define P9_30   (1<<12)		/* pr2_pru1 +in3*/
#define P9_31   (1<<10)		/* pr2_pru1 */
