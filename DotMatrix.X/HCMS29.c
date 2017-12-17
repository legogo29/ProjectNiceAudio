#include <xc.h>
#include "HCMS29.h"

char    HCMS29struct_s(struct sfr_member_t *reg, const char *address, const int mask)
{
    reg->address = address;
    reg->mask = mask;
}
void    HCMS29ctl0(struct matric_29 device, config0 data)
{
    SSPBUF = (0 << 7) | (data.sleep << 6) | (data.current << 4) | (data.brightness);
    /*PORTA  = (0 << 7) | (data.sleep << 6) | (data.current << 4) | (data.brightness);*/    /* uncomment to verify*/
}

void    HCMS29ctl1(struct matric_29 device, config1 data)
{
    SSPBUF = (1 << 7) | (0 << 6) | (0 << 5) | (0 << 4) | (0 << 3) | (0 << 2) | (data.prescaler << 1) | (data.data_out);
}