#ifndef _INFRARED_H
#define _INFRARED_H


typedef union 
{
    struct                              /* Anonymous structure with individual bits */
    {
        unsigned C1     : 1;            /* This one is pre set as 1                 */
        unsigned C2     : 1;            /* This one is pre set as 0                 */
        unsigned C3     : 1;            /* This one is pre set as 1                 */
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
    
    struct                              /* Anonymous structure with compacted bits */
    {
        unsigned C      : 3;
        unsigned H      : 1;
        unsigned S      : 2;
        unsigned D      : 6;
    };
    
    unsigned array[12];                 /* Used to loop through all struct members */ 
}IRbits;

/****************************************************************************************************************************
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
****************************************************************************************************************************/
#define VOLUME_UP   (0b101100100000)    /* See Note VOLUME_UP                       */ 
/****************************************************************************************************************************
        Notes VOLUME_UP
    Code: 101 100 100000
    The volume+ button is connected to key number 1. 
    So this is a Continue button so H needs to be up. Key number 1 is connected to the row of D1.
****************************************************************************************************************************/
#define VOLUME_DOWN (0b101100010000)    /* See Note VOLUME_Down                     */
/****************************************************************************************************************************
        Notes VOLUME_DOWN
    Code: 101 100 010000
    The volume- button is connected to key number 2.
    So this is a Continue button so H needs to be up. Key number 2 is connected to the row of D2
****************************************************************************************************************************/
#define INPUT1      (0b101010010000)    /* See Note INPUT1                          */
/****************************************************************************************************************************
        Notes INPUT1
    Code: 101 010 010000
    The INPUT1 button is connected to key number 8.
    So this is a Single-Shot button, connected on S1 so this one is up. Key number 8 is connected to the row of D2
****************************************************************************************************************************/
#define INPUT2      (0b101100001000)    /* See Note INPUT2                          */
/****************************************************************************************************************************
        Notes INPUT2
    Code: 101 100 001000
    The INPUT2 button is connected to key number 3.
    So this is a Continue button so H needs to be up. Key number 3 is connected to the row of D3
****************************************************************************************************************************/
#define INPUT3      (0b101010100000)    /* See Note INPUT3                          */
/****************************************************************************************************************************
        Notes INPUT3
    Code: 101 010 100000
    The INPUT3 button is connected to key number 7.
    So this is a Single-Shot button, connected on S1 so this one is up. Key number 7 is connected to the row of D1
****************************************************************************************************************************/
#define INPUT4      (0b101010001000)    /* See Note INPUT4                          */  
/****************************************************************************************************************************
        Notes INPUT4
    Code: 101 010 001000
    The INPUT1 button is connected to key number 9.
    So this is a Single-Shot button, connected on S1 so this one is up. Key number 9 is connected to the row of D3
****************************************************************************************************************************/
#endif
