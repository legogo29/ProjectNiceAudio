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

#define _XTAL_FREQ  4000000

void picinit(void);

void main(void)
{
    
}

void picinit(void) {
    
    OSCCONbits.IRCF         = 0b110;            /* Set the internal clock speed to 4 MHz */
    OSCCONbits.OSTS         = 0;
    OSCCONbits.SCS          = 0;
    
    TRISA                   = 0;                /* Define all RA pins as an output */
    PORTA                   = ~1;               /* Turn on Input number 1 */
    
    PORTB                   = 0x30;             // Preset the output values, so the interrupt won't trigger
    
    TRISBbits.TRISB4        = 1;                /* Define pin 37 as input (rotary A) */
    TRISBbits.TRISB5        = 1;                /* Define pin 38 as input (rotary B) */
    
    TRISC                   = 0;                /* Identify all C-pins as output (used for display2) */
    PORTC                   = 0;                /* Set all C-pins to logical LOW */
    
    TRISD                   = 0;                /* Identify all D-pins as output (used for display1) */
    PORTD                   = 0;                /* Set all D-pins to logical LOW */
    
    TRISEbits.TRISE2        = 1;                // Define pin 10 as input (potentiometer) */
    
    ANSELHbits.ANS11        = 0;                /* Set pin 37 (RB4) as a digital input */                             
    ANSELHbits.ANS12        = 0;                /* Set pin 33 (RB0) as a digital input */
    ANSELHbits.ANS13        = 0;                /* Set pin 38 (RB5) as a digital input */
    ANSEL                   = 0;                // define low anaog pins as digital, these corrospond to digital pins RA0-RA3, RA5 and RE0-RE2
    
    /*
     * Setup ADC
     */
    
    ADCON0bits.ADCS         = 0b00;             // Set the speed of the ADC to FOSC/2
    ADCON0bits.CHS          = 7;                // Select chanel AN7, this is connected to RE2
    ADCON0bits.GO           = 0;                // Set the GO (status) bit to 0
    ADCON0bits.ADON         = 1;                // Enable the ADC

    ADCON1bits.ADFM         = 1;                // Right justify the output
    ADCON1bits.VCFG0        = 0;                // connect Vref+ to Vss
    ADCON1bits.VCFG1        = 0;                // connect Vref- to Vdd
    
    /*
     * Setup SPI
     */
                                                /* NOTE: Since we do not have input from the display, this is unnecessary */
    SSPSTATbits.SMP         = 1;                /* Sample at the end of data output time */
    SSPSTATbits.CKE         = 1;                /* Data is transmitted on the rising edge */
    
    SSPCONbits.SSPM         = 0b0000;           /* SPI master mode
                                                 * Clock line will be Fosc / 4 (1 MHz) */ 
    SSPCONbits.CKP          = 0;                /* Clock idle is a LOW signal (See figure 13-2 in PIC16F887 datasheet) */
    SSPCONbits.SSPEN        = 1;                /* Enable pins (SCK, SDO, SDI)
                                                 * NOTE: not SS is also enabled (pin 7), but not used */
    /*
     * Setup Interrupt
     */
    
    INTCONbits.GIE      = 0;                    /* Disables global interrupts */
    INTCONbits.PEIE     = 0;                    // Disables all peripheral interrupts
    INTCONbits.T0IE     = 0;                    // Disables the Timer0 interrupt
    INTCONbits.INTE     = 0;                    // Disables the INT external interrupt
    INTCONbits.RBIE     = 0;                    // Disables the PORTB change interrupt
    INTCONbits.INTF     = 0;                    /* Clear flag while flashing */
    INTCONbits.RBIF     = 0;                    // clear flag
    IOCB                = 0;                    // Clear IOCB register,
    IOCBbits.IOCB4      = 1;                    /* Cause IOC for pin 37 */
    
    INTCONbits.RBIE     = 1;                    // Enables the PORTB change interrupt
    INTCONbits.GIE      = 1;                    /* Enable global interrupts, this should happen after all other setup */

}