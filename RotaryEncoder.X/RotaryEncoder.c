/* 
 * File:   RotaryEncoder.c
 * Author: tim
 *
 * Created on November 16, 2017, 1:24 PM
 */

// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>                                     //PIC hardware mapping
#define _XTAL_FREQ 500000                           //Used by the XC8 delay_ms(x) macro


void main() {
    OSCCON = 0b00110001;                            //500KHz clock speed
    
    TRISA               = 0;                        /* Define all RA pins as an output */
    PORTAbits.RA3       = 1;                        /* Turn on LED number 4 */
    
    TRISBbits.TRISB4    = 1;                        /* Define pin 37 as input (rotary A) */
    TRISBbits.TRISB5    = 1;                        /* Define pin 38 as input (rotary B) */
    
    ANSELbits.ANS4      = 0;                        /* Define pin 37 as a digital input */                             
    ANSELbits.ANS5      = 0;                        /* Define pin 38 as a digital input */
    
    INTCONbits.INTE     = 1;                        /* Enable external interrupt */
    INTCONbits.INTF     = 0;                        /* Clear flag while flashing */
    INTCONbits.GIE      = 1;                        /* Enable global interrupts */
    IOCBbits.IOCB4      = 1;                        /* Cause IOC for pin 37 */
    
    
    while(1)
    {
        /* We usually do other tasks here */
    }
    
}

void interrupt ISR()
{
    if(INTCONbits.INTF)                             /* Is the interrupt caused by external interrupt on PORTB? */
    {
        bit value   = PORTBbits.RB5;                /* Isolate the measured voltage on pin 38 (rotary B) */
        
        switch(value)                               /* Determine the direction of the rotary encoer */
        {
            case 0:                                 /* The rotary encoder went clockwise */
                if(PORTAbits.RA3)
                {
                    PORTAbits.RA3 = 0;
                    PORTAbits.RA0 = 1;
                    break;
                }
                PORTA = PORTA << 1;
                break;
            case 1:                                 /* The rotary encoder went contra clockwise */
                PORTA = PORTA << 1;
                if(STATUSbits.C) PORTAbits.RA3 = 1; /* Turn LED 4 on if we switched from LED 1 to "Led 0" */
                break;
        }
        INTCONbits.INTF = 0;                        /* Clear the interrupt flag for PORTB
                                                     * This causes that we leave the ISR and new interrupts are welcome */
    }
}