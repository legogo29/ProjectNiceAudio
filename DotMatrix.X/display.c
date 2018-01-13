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

#define _XTAL_FREQ  4000000

void main(void)
{
    OSCCONbits.IRCF         = 0b110;            /* Set the internal clock speed to 4 MHz */
    OSCCONbits.OSTS         = 0;
    OSCCONbits.SCS          = 0;
    
    TRISC                   = 0;                /* Identify all C-pins as output (used for display1) */
    PORTC                   = 0;                /* Set all C-pins to logical LOW */
    
    TRISD                   = 0;                /* Identify all D-pins as output (used for display2) */
    PORTD                   = 0;                /* Set all D-pins to logical LOW */
    
                                                /* NOTE: Since we do not have input from the display, this is unnecessary */
    SSPSTATbits.SMP         = 1;                /* Sample at the end of data output time */
    SSPSTATbits.CKE         = 1;                /* Data is transmitted on the rising edge */
    
    SSPCONbits.SSPM         = 0b0000;           /* SPI master mode
                                                 * Clock line will be Fosc / 4 (1 MHz) */ 
    SSPCONbits.CKP          = 0;                /* Clock idle is a LOW signal (See figure 13-2 in PIC16F887 datasheet) */
    SSPCONbits.SSPEN        = 1;                /* Enable pins (SCK, SDO, SDI)
                                                 * NOTE: not SS is also enabled (pin 7), but not used */
    
    struct matric_29 display1;                  /* Declare the identifier 'display1' to the compiler */                         
    struct matric_29 display2;                  /* Declare the identifier 'display2' to the compiler */
        
    HCMS29struct_s(&display2.BL, &PORTC, 0x06); /* PORTCbits.DS6 is connected to the blank pin of the dot matrix */
    HCMS29struct_s(&display2.RST, &PORTC, 0x02);/* PORTCbits.DS2 is connected to the reset pin of the dot matrix */
    HCMS29struct_s(&display2.CE, &PORTC, 0x04); /* PORTCbits.DS4 is connected to the chip enable pin of the dot matrix */
    HCMS29struct_s(&display2.RS, &PORTC, 0x07); /* PORTCbits.DS7 is connected to the register select pin of the dot matrix */

    HCMS29struct_s(&display1.BL, &PORTD, 0x06); /* PORTDbits.DS6 is connected to the blank pin of the dot matrix */
    HCMS29struct_s(&display1.RST, &PORTD, 0x02);/* PORTDbits.DS2 is connected to the reset pin of the dot matrix */
    HCMS29struct_s(&display1.CE, &PORTD, 0x04); /* PORTDbits.DS4 is connected to the chip enable pin of the dot matrix */
    HCMS29struct_s(&display1.RS, &PORTD, 0x07); /* PORTCbits.DS7 is connected to the register select pin of the dot matrix */
    
    config0 conf0;                              /* Configuration for control word 0 register */
    conf0.brightness = PWM18;
    conf0.current = 0b11;
    conf0.sleep = 0b1;
    
    config1 conf1;                              /* Configuration for control word 1 register */
    conf1.data_out = 0b0;               
    conf1.prescaler = 0b1;
    

    HCMS29wakeup(display1);                     /* While flashing the HCMS29-xx went in sleep mode. Let's wake it up */

    
    HCMS29ctl0(display1, conf0);                /* Set control word 0 of the first display */
    __delay_ms(100);                            /* TODO: Let's try to execute without the delay_ms() */
    HCMS29ctl1(display1, conf1);                /* Set control word 1 of the first display */
    __delay_ms(100);                            /* TODO: Let's try to execute without the delay_ms() */
    

    HCMS29send(display1, 'c');
    HCMS29send(display1, 'd');
    HCMS29send(display1, 'e');
    HCMS29send(display1, 'f');
    HCMS29send(display1, 'g');
    HCMS29send(display1, 'h');
    

    while(1)
    {
        __delay_ms(1000);
    }
    
    return;
}
