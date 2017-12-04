#ifndef _INFRARED_H
#define _INFRARED_H

typedef union 
{
    struct                              /* Anonymous structure with individual bits */
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
    
    struct                              /* Anonymous structure with compacted bits */
    {
        unsigned C      : 3;
        unsigned H      : 1;
        unsigned S      : 2;
        unsigned D      : 6;
    };
    
    unsigned array[12];                 /* Used to loop through all struct members */ 
}IRbits;



#endif