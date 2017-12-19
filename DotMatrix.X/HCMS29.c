#include <xc.h>

#include "HCMS29.h"
#include "font.h"

char    HCMS29struct_s(struct sfr_member_t *reg, const char *address, const int mask)
{
    reg->address = address;
    reg->mask = mask;
}
void    HCMS29ctl0(struct matric_29 device, config0 data)
{
    *device.RS.address |= (1 << device.RS.mask);    /* Set the register select pin to logic HIGH */
    *device.CE.address &= ~(1 << device.CE.mask);    /* Clear the chip enable to logic LOW so we can write to the display */
            
    SSPBUF =(0 << 7) | (data.sleep << 6) |  
            (data.current << 4) | (data.brightness);
    while(!PIR1bits.SSPIF);                         /* While the MSSP module is still sending a byte, let's wait */
    
    *device.CE.address |= (1 << device.CE.mask);    /* Set the chip enable to logic HIGH so de data will be latched in the control word */
    PIR1bits.SSPIF = 0;                             /* Clear the interrupt flag AFTER setting the CE pin (this has higher priority */
}

void    HCMS29ctl1(struct matric_29 device, config1 data)
{
    *device.RS.address |= (1 << device.RS.mask);    /* Set the register select pin to logic HIGH */
    *device.CE.address &= ~(1 << device.CE.mask);    /* Clear the chip enable to logic LOW so we can write to the display */
    
    SSPBUF =(1 << 7) | (0 << 6) | 
            (0 << 5) | (0 << 4) | 
            (0 << 3) | (0 << 2) | 
            (data.prescaler << 1) | (data.data_out);
    while(!PIR1bits.SSPIF);                         /* While the MSSP module is still sending a byte, let's wait */
    
    *device.CE.address |= (1 << device.CE.mask);    /* Set the chip enable to logic HIGH so de data will be latched in the control word */
    PIR1bits.SSPIF = 0;                             /* Clear the interrupt flag AFTER setting the CE pin (this has higher priority */
}
void    HCMS29send(struct matric_29 device, char c)
{
    *device.RS.address &= ~(1 << device.RS.mask);
    *device.CE.address &= ~(1 << device.CE.mask);
    
    for(int i=0; i < 5; i++)
    {
        if(i !=0)   // exclude 0 incase SSPIF wasnt set before this function was called
        {
            while(!PIR1bits.SSPIF);
            PIR1bits.SSPIF = 0;
        }
            SSPBUF = CHARACTER_SET[5*c + i];
    }
    
    *device.CE.address |= (1 << device.CE.mask);
}
