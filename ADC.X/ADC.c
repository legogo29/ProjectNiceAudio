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

#define NUMBER_OF_LEDS 4                            //number of LEDs this program will control
#define INPUTBITS 1024                              //the size of the input value
#define HYSTERESIS ((int) (INPUTBITS * 0.01))       //the size of the offset for hysteresis, this is 1% of the input range
#define STEPSIZE (INPUTBITS / (NUMBER_OF_LEDS+1))   //the size of the steps between intervals there has to be accounted for an extra LED, because there has to be an equal empty space at the end

void main()
{
    OSCCON = 0b00110001;                            //500KHz clock speed
    
    TRISA               = 0;                        /* Define all RA pins as an output */
    PORTA               = ~1;                       /* Turn on Input number 1 */
    
    TRISBbits.TRISB4    = 1;                        /* Define pin 37 as input (rotary A) */
    TRISBbits.TRISB5    = 1;                        /* Define pin 38 as input (rotary B) */
    TRISEbits.TRISE2    = 1;                        // Define pin 10 as input (potentiometer) */
    
    ANSELHbits.ANS11    = 0;                        /* Define pin 37 as a digital input */                             
    ANSELHbits.ANS13    = 0;                        /* Define pin 38 as a digital input */
    ANSEL               = 0x80;                     // define low anaog pins as digital, these corrospond to digital pins RA0-RA3, RA5, RE0 and RE1, set RE2 as analog
    
    ADCON0bits.ADCS   = 0b00;                       // Set the speed of the ADC to FOSC/2
    ADCON0bits.CHS    = 7;                          // Select chanel AN7, this is connected to RE2
    ADCON0bits.GO     = 0;                          // Set the GO (status) bit to 0
    ADCON0bits.ADON   = 1;                          // Enable the ADC

    ADCON1bits.ADFM   = 1;                          // Right justify the output
    ADCON1bits.VCFG0  = 0;                          // connect Vref+ to Vss
    ADCON1bits.VCFG1  = 0;                          // connect Vref- to Vdd
    
    while (1)
    {
        if (!GO)
        {
            GO = 1;
            short analog_result = ((short) ADRESH << 8) | ADRESL;
            for (char i = 0; i < 8 /* NUMBER_OF_LEDS*/; i++) {                 //iterate through the LEDS
                if (analog_result > (STEPSIZE * (i+1) + HYSTERESIS)) {         //test if the dial is past the breaking point for the step
                    PORTA &= ~(1<<i);                                    //disable the LED if the condition is met
                } else if (analog_result < (STEPSIZE * (i+1) - HYSTERESIS)) {  //test if the dial is before the breaking point for the step
                    PORTA |= (1<<i);                                     //enable the LED if the condition is met
                }
            }
        }
    }
}