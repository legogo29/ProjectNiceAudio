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
    
    TRISC                   = 0;            /* Configure all C-pins as an output */
    PORTC                   = 0;            /* Turn off motor */

    TRISBbits.TRISB0        = 1;            /* Identify pin 33 (RB0) as an input */
    ANSELHbits.ANS8         = 0;            /* Set pin 33 (RB0) to a digital input */
    
    IOCB                    = 0;            /* Disable interrupt-on-change on all B-pins */
    IOCBbits.IOCB0          = 1;            /* Enable interrupt-on-change for pin 33 (RB0) */
    
    INTCON                  = 0;            /* Disable any kind of interrupt */
    INTCONbits.RBIE         = 1;            /* Enable interrupt-on-change on PORTB register */
    INTCONbits.PEIE         = 1;            /* Enable interrupts from the outside (Maybe for IOC?) */
                                            /* NOTE: At this point the global interrupt is not enabled yet */
    
    T1CON                   = 0;            /* Timer1 is on (not dependent of a gate)    
                                             * Use a 1:1 prescaler
                                             * Do not use a Low-Power oscillator
                                             * Use the internal oscillator / 4 (instruction time)
                                             * Timer1 is off */
                                            /* NOTE: The value in register TMR1H and TMR1L is set once a IOC occurred 
                                             * Here the Timer1 module will be turned on */ 
    
    PIE1                    = 0;            /* Disable all interrupts described in the PIE1 register */
    PIE1bits.TMR1IE         = 1;            /* Enable Timer1 overflow interrupt */
    
    index = 0;                              /* Start the index of the array (IRbits) at position 0 */
    while(1)    
    {
        PORTAbits.RA0 = IRbits.D1;
        PORTAbits.RA1 = IRbits.D2;
        PORTAbits.RA2 = IRbits.D3;
        PORTAbits.RA3 = IRbits.D4;
        /* Datastring check if it match Volume up */    
        if(IR == VOLUME_UP)
         {
            /* Then write to port 15 or 16 (one of those will put the volume up) */
            PORTCbits.RC1 = 0;
            PORTCbits.RC0 = 1; //links
         }
         else if(IR == VOLUME_DOWN)
         {
             /* Then write to port 15 or 16 (one of those will put the volume down) */
             PORTCbits.RC0 = 0;
             PORTCbits.RC1 = 1; //rechts
         }
         else 
         { 
             /* Put port 15 / 16 down */ 
             PORTCbits.RC0 = 0; //stil
             PORTCbits.RC1 = 0;
         }
    }
    
    return;                                 /* We will never reach this exit point */
}


void interrupt isr()                        /* If any kind of interrupt occurs the program counter is set to this line */
{
    if(INTCONbits.RBIF)                     /* The voltage on pin 33 (RB0) changed */
    {
        if(PORTBbits.RB0)                   /* Was the change from negative to positive (rising edge)? */
        {
            TMR1 = 63436;                   /* See footnote 3 and footnote 4 */
            T1CONbits.TMR1ON = 1;           /* Turn on the Timer1 module */
        }
        INTCONbits.RBIF = 0;                /* Clear the interrupt flag in software. New changes are welcome */
    }
    
    if(PIR1bits.TMR1IF)
    {
        T1CONbits.TMR1ON = 0;               /* Stop the Timer1 module (so not another interrupts will occur and wait) */
        /* We should check the RB0 pin state here and write a bit to the structure */
        if(PORTBbits.RB0)
        {
            IRbits.array[index] = 1;
        }
        else
        {
            IRbits.array[index] = 0;
        }
        
        index += 1;                         /* Increment the index by 1 (next array position */
        if(index == 12) index = 0;          /* The transmission is complete. Let's point to the begin of the array */
        PIR1bits.TMR1IF = 0;                /* Clear the interrupt flag in software (so we leave the isr) */
    }
}



/************************************************************************************************************************************
 *                                                                                                                                  *
 *  Footnote 1                                                                                                                      *
 *      Our clock speed is 4 MHz (4 000 000 Hz). This means that we execute (4 000 000 / 4) 1 000 000 instructions per second       *
 *      1 instruction takes (1 / 1 000 000) 0,000001 second per instruction. This is equal to 0,001 microseconds                    *
 *                                                                                                                                  *
 *  Footnote 2                                                                                                                      *
 *      When a 0 is received pin 33 is HIGH for 0,2 milliseconds                                                                    *
 *      When a 1 is received pin 33 is HIGH for 0,6 milliseconds                                                                    *
 *      Let's start a timer module and generate an interrupt after 0,2 milliseconds. If the pin is still high we received a 1.      *
 *                                                                                                                                  *
 *  Footnote 3                                                                                                                      *
 *      Every 0,001 microsecond the Timer1 module adds 1. Our goal is give an interrupt after 2 milliseconds                        *
 *      2 = 0,001 * x     (x is 2 000)                                                                                              *
 *      To have 100% assurance let's generate an interrupt after 2100 milliseconds                                                  *
 *                                                                                                                                  *
 *  Footnote 4                                                                                                                      *
 *      Timer1 module contains a 16 bit resolution (65 536)                                                                         *
 *      65 536 - 2100 = 63 436 (this should be the starting value)                                                                  *
 *      (2 100 * 0,001) = 2,1 milliseconds                                                                                          *
 *                                                                                                                                  *                                                                 
 ************************************************************************************************************************************/
