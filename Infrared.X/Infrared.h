#ifndef _INFRARED_H
#define _INFRARED_H


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
}IRbits;

/********************************************************************************
        Notes about raw button codes that the uC receives 
The code that the uC receives is 12 bits long.
The first 3 bits(C1, C2 AND C3) stand firm programed. 
    In our case this is: 101
Then we get the next 3 bits. 
    The first 1 is for the continous buttons(H), this is used for three buttons in this project: Volume + and - and Output 2.
    The other two stand for single-shot buttons(S1 AND S2), those are used for: Ipunt 1, 3, 4.
The we get the 6 last bits.
    Those bits are used for input data(D1 untill D6). There are 18 combinations,
    One key of H1, S1 or S2 and then one of the 6 last buttons.
********************************************************************************/
#define VOLUME_UP   (0b101100100000)    /* key number 1, H  is up and D1 */ 
#define VOLUME_DOWN (0b101100010000)    /* key number 2, H  is up and D2 */
#define INPUT1      (0b101010010000)    /* key number 8, S1 is up and D2 */
#define INPUT2      (0b101100001000)    /* key number 3, H  is up and D3 */
#define INPUT3      (0b101010100000)    /* key number 7, S1 is up and D1 */
#define INPUT4      (0b101010001000)    /* key number 9, S1 is up and D3 */  

#endif
