#ifndef _INFRARED_H
#define _INFRARED_H

int index;                              /* Used to identify structure members in the array */

extern volatile unsigned short IR @0x020;
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
    
    unsigned array[12];                 /* Used to loop through all struct members      */ 
}IRbits_t;
extern volatile IRbits_t IRbits @0x020;

#define VOLUME_UP   (0b101100100000)    /* key number 1, H  is up and D1 */ 
#define VOLUME_DOWN (0b101100010000)    /* key number 2, H  is up and D2 */
#define INPUT1      (0b101010010000)    /* key number 8, S1 is up and D2 */
#define INPUT2      (0b101100001000)    /* key number 3, H  is up and D3 */
#define INPUT3      (0b101010100000)    /* key number 7, S1 is up and D1 */
#define INPUT4      (0b101010001000)    /* key number 9, S1 is up and D3 */  

#endif