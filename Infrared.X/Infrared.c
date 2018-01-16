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

#define _XTAL_FREQ  4000000

void main(void) 
{
    OSCCONbits.IRCF         = 0b110;        /* Set speed to 4 MHz */
    OSCCONbits.OSTS         = 0;            /* An internal oscillat or is used */
    OSCCONbits.SCS          = 0;            /* System clock is determined by the FOSC config bit */
    
    TRISA                   = 0;            /* Configure all A-pins as an output */
    PORTA                   = 0;            /* Turn off all LEDs */
    
    TRISC                   = 0;            /* Configure all C-pins as an output */
    PORTC                   = 0;            /* Turn off motor */

    TRISBbits.TRISB0        = 1;            /* Identify pin 33 (RB0) as an input */
    ANSELHbits.ANS12        = 0;            /* Set pin 33 (RB0) to a digital input */
    
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
    T2CON                   = 0;            /* Use a 1:1 postscaler
                                             * Do not turn on the Timer2 module
                                             * Use a 1:1 prescaler */
    
    IR                      = 0;
    
    PIE1                    = 0;            /* Disable all interrupts described in the PIE1 register */
    PIE1bits.TMR1IE         = 1;            /* Enable Timer1 overflow interrupt */
    PIE1bits.TMR2IE         = 1;            /* Enable Timer2 interrupt (NOTE: only software will trigger) */
    INTCONbits.GIE          = 1;            /* All interrupts have been configured. We can enable the global interrupt */
    
    index   = 0;                            /* Start the index of the array (IRbits) at position 0 */
    oos     = 0;                            /* Let's assume that we are at init time not out of sync */
    
    while(1)    
    {
        if(index == 3 && 
           IRbits.C != 0b010 && !oos)       /* If we received 3 bits that are not 0-1-0 we are out of sync */
        {
            PIR1bits.TMR2IF;                /* Trigger an interrupt in-software */
        }
        
        
        /* Datastring check if it match Volume up */    
        //if(IR == VOLUME_UP)
        if (IR != 0) { // if new data has been recieved
            if (IR == VOLUME_UP) //(IRbits.C == 0b101) ///((!IRbits.D1)&&(!IRbits.H))
            {
                /* Then write to port 16 to turn the motor to make the volume higher */
                PORTCbits.RC0 = 0;
                PORTCbits.RC1 = 1; //rechts
                
                __delay_us(100);
                /* Put port 15 and 16 down */ 
                PORTCbits.RC0 = 0;
                PORTCbits.RC1 = 0;
                
                IR = 0;
            }
            else if /*((!IRbits.D2)&&(!IRbits.H))*/ (IR == VOLUME_DOWN)
            {
                /* Then write to port 15 to turn the motor to make the volume lower */
                PORTCbits.RC1 = 0;
                PORTCbits.RC0 = 1; //links
                
                __delay_us(100);
                /* Put port 15 and 16 down */ 
                PORTCbits.RC0 = 0;
                PORTCbits.RC1 = 0;
                
                IR = 0;
            } else 
            { 
                /* Put port 15 and 16 down */ 
                PORTCbits.RC0 = 0;
                PORTCbits.RC1 = 0;
            }
        }
    }
        
    return;                                 /* We will never reach this exit point */
}


void interrupt isr()                        /* If any kind of interrupt occurs the program counter is set to this line */
{
    if(INTCONbits.RBIF)                     /* The voltage on pin 33 (RB0) changed */
    {

        if(PORTBbits.RB0 && !oos)           /* Was the change from negative to positive (rising edge)? And was the signal not oos*/
        {
            TMR1 = 64936;                   /* See footnote 3 and footnote 4 */
            T1CONbits.TMR1ON = 1;           /* Turn on the Timer1 module */
        }
        INTCONbits.RBIF = 0;                /* Clear the interrupt flag in software. New changes are welcome */
    }
                                            
    if(PIR1bits.TMR1IF)
    {
        T1CONbits.TMR1ON = 0;               /* Stop the Timer1 module (so not another interrupts will occur and wait) */
        
        switch(oos)
        {
            case 0:
                if(PORTBbits.RB0)           /* If the pin is still high after 0,6 milliseconds */
                {
                    IR |= (1<<index);
                    //IRbits.array[index] = 1;        /* We received a 1. Store this information in our union IRbits */
                } else
                {
                    IR &= ~(1<<index);
                    //IRbits.array[index] = 0;        /* We received a 0. Store this information in our union IRbits */
                }

                index += 1;                  /* Increment the index by 1 (next array position */
                break;
                
            case 1:
                IR = 0;                     /* Remove the corrupt data */
                index = 0;                  /* Next time we receive a bit, save it at the first location (so we are sync) */
                
                oos = 0;                    /* Clear the out of sync flag since we are not out of sync anymore. */
//                INTCONbits.RBIE = 1;        /* We ignored all incoming bits and are synchronized. Incoming bits are welcome */
                
                break;
        }

        if(index == 12) index = 0;          /* The transmission is complete. Let's point to the begin of the array */
 
        PIR1bits.TMR1IF = 0;                /* Clear the interrupt flag in software (so we leave the isr) */
    }
    
    if(PIR1bits.TMR2IF)
    {
        oos = 1;                            /* We are officially out of sync */
//        INTCONbits.RBIE = 0;                /* Ignore all incoming bits from the infrared receiver temporary */
        TMR1 = 27136;                       /* See footnote 5 and 6 */
        T1CONbits.TMR1ON = 1;               /* Turn the Timer1 module on */
        
        PIR1bits.TMR2IF = 0;                
    }
}



/************************************************************************************************************************************
 *                                                                                                                                  *
 *  Footnote 1                                                                                                                      *
 *      Our clock speed is 4 MHz (4 000 000 Hz). This means that we execute (4 000 000 / 4) 1 000 000 instructions per second       *
 *      1 instruction takes (1 / 1 000 000) 0,000 001 second per instruction. This is equal to 0,001 milliseconds                   *
 *                                                                                                                                  *
 *  Footnote 2                                                                                                                      *
 *      When a 0 is received pin 33 is HIGH for 0,4 milliseconds                                                                    *
 *      When a 1 is received pin 33 is HIGH for 1.2 milliseconds                                                                    *
 *      Let's start a timer module and generate an interrupt after 0,6 milliseconds, so we have some slack.                         *
 *      If the pin is still high we received a 1.                                                                                   *
 *                                                                                                                                  *
 *  Footnote 3                                                                                                                      *
 *      Every 0,001 microsecond the Timer1 module adds 1. Our goal is give an interrupt after 0.5 milliseconds                      *
 *      0.6 = 0,001 * x     (x is 600)                                                                                              *
 *                                                                                                                                  *
 *  Footnote 4                                                                                                                      *
 *      Timer1 module contains a 16 bit resolution (65 536)                                                                         *
 *      65 536 - 600 = 64936 (this should be the starting value)                                                                    *
 *      (600 * 0,001) = 0.6 milliseconds                                                                                            *
 *                                                                                                                                  *
 *  Footnote 5                                                                                                                      *
 *      Out of sync is when C bits are not on position 0, 1 and 2 of the union but at another place.                                *
 *      We detect out of sync if the index for writing is 3 and the C bits are not 0-1-0.                                           *
 *      1 bit transmission takes 1,6 milliseconds. 1 button sends its code 2 times (to be sure it arrives)                          *
 *      2 * 12 = 24 bits.       24 bits * 1,6 milliseconds = 38.4 milliseconds.                                                     *
 *      If we detect out of sync we should ignore incoming infrared bits for the duration of 38.4 milliseconds.                     *  
 *                                                                                                                                  *
 *  Footnote 6                                                                                                                      *
 *      38.4 milliseconds is equal to 38400 microseconds                                                                            *
 *      1 instruction takes 1 microsecond so we need to delay 38400 instructions.                                                   *
 *      65 536 - 38 400 = 27 136                                                                                                    *
 *      TMR1 need to contain the value 27 136                                                                                       *
 *                                                                                                                                  *     
 ************************************************************************************************************************************/
