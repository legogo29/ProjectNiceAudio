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

#define BACKWARD 1                                  //define variables FORWARD and BACKWARD
#define FORWARD 0

void rotary_encoder();
/*
 * 
 */
void main() {
    OSCCON = 0b00110001;                            //500KHz clock speed
    
    TRISBbits.TRISB4 = 1;                           //pin B4 and B5 as input
    TRISBbits.TRISB5 = 1;
    
    TRISA = 0x00;                                   //set the A resiter as output pins
    
  
    while (1) {
        rotary_encoder();
    }
    
}

void rotary_encoder() {
    char pin_a = PORTBbits.RB4;
    char pin_b = PORTBbits.RB5;
    int direction_current;
    int transistor_current;
    int direction_previous;
    
    if(pin_a == 0 && pin_b == 0)
    {
        direction_current = BACKWARD;
    }
    else if(pin_a == 0 && pin_b == 1)
    {
        direction_current = FORWARD;
    }
    else 
    {
        return;
    }

    // keuze uit forward of backward procedure
    // wanneer direction = forward
    if(direction_current == FORWARD)
    {
        if(transistor_current == 4)
        {
            transistor_current = 1;
            PORTA = 1;
            direction_previous = direction_current;
            direction_current = -1;
        }
        else
        {
            transistor_current = transistor_current +1;
            PORTA = PORTA << 1;
            direction_previous = direction_current;
            direction_current = -1;
        }
    }
    // wanneer direction = backward
    else if(direction_current == BACKWARD)
    {
        if(transistor_current == 1)
        {
            transistor_current = 4;
            PORTA = 0b1000;
            direction_previous = direction_current;
            direction_current = -1;
        }
        else
        {
            transistor_current = transistor_current -1;
            PORTA = PORTA >> 1;
            direction_previous = direction_current;
            direction_current = -1;
        }
    }
}