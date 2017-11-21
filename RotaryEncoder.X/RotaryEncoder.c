// PIC16F887 Configuration Bit Settings
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

#include <xc.h>                                     //PIC hardware mapping
#define _XTAL_FREQ 500000                           //Used by the XC8 delay_ms(x) macro


void main() {
    OSCCON = 0b00110001;                            //500KHz clock speed
    
    TRISA               = 0;                        /* Define all RA pins as an output */
    PORTA               = ~1;                       /* Turn on Input number 1 */
    
    TRISBbits.TRISB4    = 1;                        /* Define pin 37 as input (rotary A) */
    TRISBbits.TRISB5    = 1;                        /* Define pin 38 as input (rotary B) */
    
    ANSELHbits.ANS11    = 0;                        /* Define pin 37 as a digital input */                             
    ANSELHbits.ANS13    = 0;                        /* Define pin 38 as a digital input */
    ANSEL               = 0;                        // define low anaog pins as digital, these corrospond to digital pins RA0-RA3, RA5 and RE0-RE2
    
    PORTB               = 0x30;                      // Preset the output values, so the interrupt won't trigger

    INTCONbits.GIE      = 0;                        /* Disables global interrupts */
    INTCONbits.PEIE     = 0;                        // Disables all peripheral interrupts
    INTCONbits.T0IE     = 0;                        // Disables the Timer0 interrupt
    INTCONbits.INTE     = 0;                        // Disables the INT external interrupt
    INTCONbits.RBIE     = 0;                        // Disables the PORTB change interrupt
    INTCONbits.INTF     = 0;                        /* Clear flag while flashing */
    INTCONbits.RBIF     = 0;                        // clear flag
    IOCB                = 0;                        //
    IOCBbits.IOCB4      = 1;                        /* Cause IOC for pin 37 */
    
    INTCONbits.RBIE     = 1;                        // Enables the PORTB change interrupt
    INTCONbits.GIE      = 1;                        /* Enable global interrupts, this should happen after all other setup */

    
    
    while(1)
    {
        /* We usually do other tasks here, delay until there is something here */
        __delay_us(50);
    }
    
}

void interrupt ISR()
{
    if(!PORTBbits.RB4)                               /* Is the interrupt caused by external interrupt on PORTB? */
    {
        int value   = PORTBbits.RB5;                /* Isolate the measured voltage on pin 38 (rotary B) */
        
        switch(value)                               /* Determine the direction of the rotary encoer */
        {
            case 1:                                 /* The rotary encoder went clockwise */
                if(!PORTAbits.RA3)                  // If Input 4 was on, we should rollover
                {
                    PORTAbits.RA3 = 1;              // manualy set the new condition
                    PORTAbits.RA0 = 0;
                    break;                          // we won't continue to bitshift, because we are already in the desired state
                }
                PORTA = PORTA << 1;
                PORTAbits.RA0 = 1;                  // Set RA0 off, when bitshifting, a 0 was shifted in here, we want a 1 because the output will be inverted.
                break;
            case 0:                                 /* The rotary encoder went contra clockwise */
                
                if(!PORTAbits.RA0) {                // If Input 4 was on, we should rollover
                    PORTAbits.RA0 = 1;              // manualy set the new condition
                    PORTAbits.RA3 = 0;
                    break;                          // we won't continue to bitshift, because we are already in the desired state
                }
                PORTA = PORTA >> 1;
                PORTAbits.RA7 = 1;                  // Set RA7 off, when bitshifting, a 0 was shifted in here, we want a 1 because the output will be inverted.
                break;
        }
        INTCONbits.RBIF = 0;                        /* Clear the interrupt flag for RB
                                                     * This causes that we leave the ISR and new interrupts are welcome */
    }
        INTCONbits.INTF = 0;                        /* Clear interrupt flag */

}