#include <xc.h>
#include "HCMS29.h"

void    HCMS29ctl0(struct matric_29 device, config0 data)
{
    SSPBUF = (0 << 7) | (data.sleep << 6) | (data.current << 5) | (data.current << 3);
}

void    HCMD29ctl1(struct matric_29 device, config1 data)
{
    SSPBUF = (1 << 7) | (0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0 << 2) | (data.prescaler << 1) | (data.data_out);
    
}