#ifndef _INFRARED_H
#define _INFRARED_H

int index;                              /* Used to identify structure members in the union */
int IRindex;                            /* New name for index */
int oos;                                /* Flag which indicates out of sync */

extern volatile unsigned short IR @0x0EE;
/* According to http://microchipdeveloper.com/faq:38 absolute variables should be placed at the end of a memory block,
 * This is placed at the end of bank 1 */
typedef union 
{
    struct                              /* Anonymous structure with individual bits     */
    {
        unsigned C1     : 1;
        unsigned C2     : 1;
        unsigned C3     : 1;
        unsigned H      : 1;
        unsigned S1     : 1;
        unsigned S2     : 1;
        unsigned D1     : 1;
        unsigned D2     : 1;
        unsigned D3     : 1;
        unsigned D4     : 1;
        unsigned D5     : 1;
        unsigned D6     : 1;
    };
    
    struct                              /* Anonymous structure with compacted bits      */
    {
        unsigned C      : 3;            /* C stands for Combination Buttons             */
        unsigned H      : 1;            /* H stands for Continues Button                */
        unsigned S      : 2;            /* S stands for Single Shot Button.             */
        unsigned D      : 6;            /* D stands for input Data.                     */
    };
}IRbits_t;
extern volatile IRbits_t IRbits @0x0EE;

//THIS IS REVERSED
//                    // CCCHSSDDDDDD
//#define VOLUME_UP   (0b010100100000)    /* key number 1, H  is up and D1 */ 
//#define VOLUME_DOWN (0b010100010000)    /* key number 2, H  is up and D2 */
//#define INPUT1      (0b010010010000)    /* key number 8, S1 is up and D2 */
//#define INPUT2      (0b010100001000)    /* key number 3, H  is up and D3 */
//#define INPUT3      (0b010010100000)    /* key number 7, S1 is up and D1 */
//#define INPUT4      (0b010010001000)    /* key number 9, S1 is up and D3 */  

//                    // CCCHSSDDDDDD
//#define VOLUME_UP   (0b101011011111)    /* key number 1, H  is up and D1 */ 
//#define VOLUME_DOWN (0b101011101111)    /* key number 2, H  is up and D2 */
//#define INPUT1      (0b101101101111)    /* key number 8, S1 is up and D2 */
//#define INPUT2      (0b101011110111)    /* key number 3, H  is up and D3 */
//#define INPUT3      (0b101101011111)    /* key number 7, S1 is up and D1 */
//#define INPUT4      (0b101101110111)    /* key number 9, S1 is up and D3 */  

//This is right
                    // DDDDDDSSHCCC
#define VOLUME_UP   (0b000001001010)    /* key number 1, H  is up and D1 */ 
#define VOLUME_DOWN (0b000010001010)    /* key number 2, H  is up and D2 */
#define INPUT1      (0b000010010010)    /* key number 8, S1 is up and D2 */
#define INPUT2      (0b000100001010)    /* key number 3, H  is up and D3 */
#define INPUT3      (0b000001010010)    /* key number 7, S1 is up and D1 */
#define INPUT4      (0b000100010010)    /* key number 9, S1 is up and D3 */  

//                    // DDDDDDSSHCCC
//#define VOLUME_UP   (0b111110110101)    /* key number 1, H  is up and D1 */ 
//#define VOLUME_DOWN (0b111101110101)    /* key number 2, H  is up and D2 */
//#define INPUT1      (0b111101101101)    /* key number 8, S1 is up and D2 */
//#define INPUT2      (0b111011110101)    /* key number 3, H  is up and D3 */
//#define INPUT3      (0b111110101101)    /* key number 7, S1 is up and D1 */
//#define INPUT4      (0b111011101101)    /* key number 9, S1 is up and D3 */  

/* naar rechts draaien is volume_up. dat is RC0 */
/* naar links draaien is volume_down. dat is RC1 */

#endif
