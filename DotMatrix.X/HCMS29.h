#ifndef _HCMS29_H
#define _HCMS29_H
                                            
struct sfr_member
{                                               
    unsigned char *address;                     /* Address of the special function register */
    int mask;                                   /* Offset to access an individual bit */
};

struct matric_29
{
    struct sfr_member   RST;                    /* reset pin            [0 is reset] [1 is normal operation]*/
    struct sfr_member   CE;                     /* Chip enable pin      [0 write data] [1 AND clock low is data latched] */ 
    struct sfr_member   BL;                     /* blank pin            [modulated is brightness control] [1 is blank display] */ 
    struct sfr_member   RS;                     /* Register select pin  [0 is dot register] [1 is control register] */
};


/* Function prototyping */


#endif