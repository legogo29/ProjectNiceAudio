#ifndef _HCMS29_H
#define _HCMS29_H
            
enum
{
    PWM0 = 0b0000,                              /* 0% relative brightness */
    PWM1 = 0b0001,                              /* 1.7% relative brightness */
    PWM2 = 0b0010,                              /* 3.3% relative brightness */
    PWM3 = 0b0011,                              /* 5.0% relative brightness */
    PWM4 = 0b0100,                              /* 6.7 relative brightness */
    PWM5 = 0b0101,                              /* 8.3% relative brightness */
    PWM7 = 0b0110,                              /* 11.7% relative brightness */
    PWM9 = 0b0111,                              /* 15% relative brightness */
    PWM11= 0b1000,                              /* 18% relative brightness */
    PWM14= 0b1001,                              /* 23% relative brightness */
    PWM18= 0b1010,                              /* 30% relative brightness */
    PWM22= 0b1011,                              /* 37% relative brightness */
    PWM28= 0b1100,                              /* 47% relative brightness */
    PWM36= 0b1101,                              /* 60% relative brightness */
    PWM48= 0b1110,                              /* 80% relative brightness */
    PWM60= 0b1111                               /* 100% relative brightness */
};

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
    volatile unsigned char *address;            /* Address of the special function register */
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
void    HCMS29struct_s(struct sfr_member_t *reg, volatile char *address, const int mask);


/*  Configure control register 0 (first the display will listen, then it will latch the data)
 * 
 *  @param  device: Copy of the structure members (so the correct pins will be used) 
 *  @param  data:   The desired configuration (PWM brightness, peak pixel current and sleep mode) */
void    HCMS29ctl0(struct matric_29 device, config0 data);


/*  Configure control register 1 (first the display will listen, then it will latch the data)
 *
 *  @param device:  Copy of the structure members (so the correct pins will be used)
 *  @param data:    The desired configuration (data output mode and external oscillator prescaler) */
void    HCMS29ctl1(struct matric_29 device, config1 data);


/*  Send data that will be shown on the display
 * 
 *  @param device:  Copy of the structure members (so the correct pins will be used)
 *  @param c:       The character that needs to be shown (font.h is required) */
void    HCMS29send(struct matric_29 device, char c);

/*  Use HCMS29send() to sequentially write every char of str to display
 * 
 *  @param device:  Copy of the structure members (so the correct pins will be used)
 *  @param str:     The string that needs to be shown (font.h is required) */
void    HCMS29send_string(struct matric_29 device, const unsigned char *str);

/*  Use HCMS29send() to sequentially write number to display
 * 
 *  @param device:  Copy of the structure members (so the correct pins will be used)
 *  @param n:       The number that needs to be shown (font.h is required) */
void    HCMS29send_number(struct matric_29 device, char n);

/*  Wake up the HCMS29xx so the programmer can have influence on the display output
 * 
 *  @param device:  Copy of the structure members (so the correct pins will be used) */
void    HCMS29wakeup(struct matric_29 device);


/*  Turn the display in sleep mode (control words & registers will be set to 0 which results in no output to the user)
 *
 *  @param device:  Copy of the structure members (so the correct pins will be used) */
//void    HCMS29sleep(struct matric_29 device);

#endif  /* _HCMS29_H */
