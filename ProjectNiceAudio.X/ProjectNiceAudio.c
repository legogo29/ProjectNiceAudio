// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
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
#include "../DotMatrix.X/HCMS29.h"

#define _XTAL_FREQ  4000000

// For ADC
#define NUMBER_OF_STEPS 40                          //number of LEDs this program will control
#define INPUTBITS 1024                              //the size of the input value
#define HYSTERESIS ((int) (INPUTBITS * 0.01))       //the size of the offset for hysteresis, this is 1% of the input range
#define STEPSIZE (INPUTBITS / (NUMBER_OF_STEPS+1))  //the size of the steps between intervals there has to be accounted for an extra LED, because there has to be an equal empty space at the end


void picinit(void);

void main(void)
{
    picinit();
    
    /*
     * Setup Displays
     */
    struct matric_29 display1;                  /* Declare the identifier 'display1' to the compiler */                         
    struct matric_29 display2;                  /* Declare the identifier 'display2' to the compiler */
        
    HCMS29struct_s(&display1.BL, &PORTD, 0x06); /* PORTDbits.DS6 is connected to the blank pin of the dot matrix */
    HCMS29struct_s(&display1.RST, &PORTD, 0x02);/* PORTDbits.DS2 is connected to the reset pin of the dot matrix */
    HCMS29struct_s(&display1.CE, &PORTD, 0x04); /* PORTDbits.DS4 is connected to the chip enable pin of the dot matrix */
    HCMS29struct_s(&display1.RS, &PORTD, 0x07); /* PORTCbits.DS7 is connected to the register select pin of the dot matrix */
    
    HCMS29struct_s(&display2.BL, &PORTC, 0x06); /* PORTCbits.DS6 is connected to the blank pin of the dot matrix */
    HCMS29struct_s(&display2.RST, &PORTC, 0x02);/* PORTCbits.DS2 is connected to the reset pin of the dot matrix */
    HCMS29struct_s(&display2.CE, &PORTC, 0x04); /* PORTCbits.DS4 is connected to the chip enable pin of the dot matrix */
    HCMS29struct_s(&display2.RS, &PORTC, 0x07); /* PORTCbits.DS7 is connected to the register select pin of the dot matrix */
    
    PORTDbits.RD6 = 0;
    
    config0 conf0;
    conf0.brightness = PWM18;
    conf0.current = 0b11;
    conf0.sleep = 0b1;
    
    config1 conf1;
    conf1.data_out = 0b0;
    conf1.prescaler = 0b1;
    
    HCMS29wakeup(display1);

    HCMS29ctl0(display1, conf0);
    __delay_ms(100);
    HCMS29ctl1(display1, conf1);
    __delay_ms(100);
    
    HCMS29send_number(display1, 21);
    
    char volume = 0;
    
    while (1)
    {
        /*
         * ADC
         */
        if (!GO)
        {
            GO = 1;
            short analog_result = ((short) ADRESH << 8) | ADRESL;
            for (char i = 0; i < NUMBER_OF_STEPS; i++) {                 //iterate through the LEDS
                int current_step = STEPSIZE * (i+1);
                if ((analog_result > (current_step + HYSTERESIS)) && (analog_result < (current_step + STEPSIZE - HYSTERESIS)))
                {
                    volume = i + 1u;
                }
//                if (analog_result > (current_step + HYSTERESIS)) {         //test if the dial is past the breaking point for the step
//                    PORTA &= (char) ~(1<<i);                                    //disable the LED if the condition is met
//                } else if (analog_result < (current_step - HYSTERESIS)) {  //test if the dial is before the breaking point for the step
//                    PORTA |= (char) (1<<i);                                     //enable the LED if the condition is met
//                }
            }
        }
        //HCMS29send_number(display1, volume);
        __delay_ms(10);
    }
}

void interrupt ISR()
{
    /*
     * Rotary Encoder
     */
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
                PORTA <<= 1;
                PORTAbits.RA0 = 1;                  // Set RA0 off, when bitshifting, a 0 was shifted in here, we want a 1 because the output will be inverted.
                break;
            case 0:                                 /* The rotary encoder went contra clockwise */
                
                if(!PORTAbits.RA0) {                // If Input 4 was on, we should rollover
                    PORTAbits.RA0 = 1;              // manualy set the new condition
                    PORTAbits.RA3 = 0;
                    break;                          // we won't continue to bitshift, because we are already in the desired state
                }
                PORTA >>= 1;
                PORTAbits.RA7 = 1;                  // Set RA7 off, when bitshifting, a 0 was shifted in here, we want a 1 because the output will be inverted.
                break;
        }
        INTCONbits.RBIF = 0;                        /* Clear the interrupt flag for RB
                                                     * This causes that we leave the ISR and new interrupts are welcome */
    }
    INTCONbits.INTF = 0;                        /* Clear interrupt flag */

}

void picinit(void) 
{
    
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
    INTCONbits.GIE      = 0;                    /* Enable global interrupts, this should happen after all other setup */

}