#ifndef _HCMS29_H
#define _HCMS29_H
                                            /**/
struct matric_29
{
    unsigned    rst   : 1;                  /* reset pin        [0 is reset] [1 is normal operation]*/
    unsigned    ce    : 1;                  /* Chip enable pin  [0 write data] [1 AND clock low is data latched] */
    unsigned    bl    : 1;                  /* blank pin        [modulated is brightness control] [1 is blank display] */
};


#endif