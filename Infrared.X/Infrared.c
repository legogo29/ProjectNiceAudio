/* Configuration bits */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

/* Includes */
#include <xc.h>
#include "Infrared.h"



void main(void) 
{
    OSCCONbits.IRCF         = 0b110;        /* Set speed to 4 MHz */
    OSCCONbits.OSTS         = 0;            /* An internal oscillator is used */
    OSCCONbits.SCS          = 0;            /* System clock is determined by the FOSC config bit */
    
    TRISA                   = 0;            /* Configure all A-pins as an output */
    PORTA                   = 0;            /* Turn off all LEDs */
    
    TRISBbits.TRISB0        = 1;            /* Identify pin 33 (RB0) as an input */
    ANSELHbits.ANS8         = 0;            /* Set pin 33 (RB0) to a digital input */
    
    while(1)
    {
        
    }
    
    return;                                 /* We will never reach this exit point */
}


void interrupt isr()                        /* If any kind of interrupt occurs the program counter is set to this line */
{
    
}