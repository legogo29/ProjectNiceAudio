#ifndef _HCMS29_H
#define _HCMS29_H
                   
typedef struct
{
    unsigned brightness : 4;                    /* PWM brightness control*/
    unsigned current    : 2;                    /* Maximal brightness allowed */
    unsigned sleep      : 1;                    
}config0;

typedef struct
{
    unsigned data_out   : 1;                    /* [0 is hold D7 bit] [1 is cascaded] */
    unsigned prescaler  : 1;                    /* [0 is 1:1] [1 is 1:8] */
    unsigned reserved   : 5;                    /* Not used (should always be 0) */
}config1;

struct sfr_member_t
{                                               
    unsigned char *address;                     /* Address of the special function register */
    int mask;                                   /* Offset to access an individual bit */
};

struct matric_29
{
    struct sfr_member_t   RST;                  /* reset pin            [0 is reset] [1 is normal operation]*/
    struct sfr_member_t   CE;                   /* Chip enable pin      [0 write data] [1 AND clock low is data latched] */ 
    struct sfr_member_t   BL;                   /* blank pin            [modulated is brightness control] [1 is blank display] */ 
    struct sfr_member_t   RS;                   /* Register select pin  [0 is dot register] [1 is control register] */
};

/*  Set a member of the struct matric_29 (which is always a struct sfr_member_t)
 *  
 *  @param reg:     A pointer to a struct sfr_member_t 
 *  @param address: The address of the special function register
 *  @param mask:    An offset to set an individual bit of the special function register */
char    HCMS29struct_s(struct sfr_member_t *reg, const char *address, const int mask);


void    HCMS29ctl0(struct matric_29 device, config0 data);

void    HCMD29ctl1(struct matric_29 device, config1 data);

#endif