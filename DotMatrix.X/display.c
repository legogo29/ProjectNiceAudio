// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>                             
#include "HCMS29.h"                             /* Library for the HCMS-29xx dot matrix display */
                                            
void main(void)
{
    OSCCONbits.IRCF         = 0b110;            /* Set the internal clock speed to 4 MHz */
    
    TRISC                   = 0;                /* Identify all C-pins as output */
    PORTC                   = 0;                /* Set all C-pins to logical LOW */
    
    TRISD                   = 0;                /* Identify all D-pins as output */
    PORTD                   = 0;                /* Set all D-pins to logical LOW */
    
    SSPSTATbits.SMP         = 1;                /* Sample at the end of data output time */
    SSPSTATbits.CKE         = 1;                /* Data is transmitted on the rising edge */
    
    SSPCONbits.SSPM         = 0b0000;           /* SPI master mode
                                                 * Clock line will be Fosc / 4 (1 MHz) */ 
    SSPCONbits.CKP          = 0;                /* Clock idle is a LOW signal */
    SSPCONbits.SSPEN        = 1;                /* Enable pins (SCK, SDO, SDI)
                                                 * NOTE: not SS is also enabled (pin 7), but not used */
    
    struct matric_29 display1;                         
    struct matric_29 display2;
    
    display1.bl             = {&PORTC, 0x0006};
    /*
    display1.rst            = PORTCbits.RC2;
    display1.ce             = PORTCbits.RC4;
    
    display2.bl             = PORTDbits.RD6;
    display2.rst            = PORTDbits.RD2;
    display2.ce             = PORTDbits.RD4;
    */
    
    while(1)
    {

    }
}

void interrupt isr()
{
    
}

