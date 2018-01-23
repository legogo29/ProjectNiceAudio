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
#include "../Infrared.X/Infrared.h"

#define _XTAL_FREQ  4000000

// For ADC
#define NUMBER_OF_STEPS 40                          //number of LEDs this program will control
#define INPUTBITS 1024                              //the size of the input value
#define HYSTERESIS ((int) (INPUTBITS * 0.01))       //the size of the offset for hysteresis, this is 1% of the input range
#define STEPSIZE (INPUTBITS / (NUMBER_OF_STEPS+1))  //the size of the steps between intervals there has to be accounted for an extra LED, because there has to be an equal empty space at the end


char inputChanged = 0;

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
    
    PORTDbits.RD6 = 0;                          /* Sets blank pin for display 2 low, most likely not needed */
    
    config0 conf0;                              /* Configuration for control word 0 register */
    conf0.brightness = PWM18;                   /* Set the relative brightness to 30% of what it is capable of */
    conf0.current = 0b11;                       /* Set the peak current to 12.8 mA */
    conf0.sleep = 0b1;                          /* Do NOT go in sleep mode */
    
    config1 conf1;                              /* Configuration for control word 1 register */
    conf1.data_out = 0b0;                       /* Keep the content of bit D7 (we do not cascade HCMS displays) */
    conf1.prescaler = 0b1;                      /* Set the internal oscillator prescaler to 1:1 */
    
    HCMS29wakeup(display1);                     /* While flashing the HCMS29-xx went in sleep mode. Let's wake it up */

    HCMS29ctl0(display1, conf0);                /* Set control word 0 of the first display */
    __delay_ms(100);                            /* TODO: Let's try to execute without the delay_ms() */
    HCMS29ctl1(display1, conf1);                /* Set control word 1 of the first display */
    __delay_ms(100);                            /* TODO: Let's try to execute without the delay_ms() */
    
    HCMS29wakeup(display2);                     /* Do the same for display 2 */

    HCMS29ctl0(display2, conf0);
    __delay_ms(100);
    HCMS29ctl1(display2, conf1);
    __delay_ms(100);
    
    HCMS29send_string(display2, "Vol.    ");    /* Initialize display 1 */
    HCMS29send_string(display1, "Input: 1");    /* Initialize display 2 */
    
    char volume = 0;                            /* Initialize some variables for tracking volume */
    char previousVolume = 0;
    char targetVolume = 20;
    
//    char inputCounter = 9;
    
    while (1)
    {
        /*
         * ADC
         */
        if (!GO)
        {
            GO = 1; // this should probably be after setting analog_result
            previousVolume = volume; // Set previousVolume
            short analog_result = ((short) ADRESH << 8) | ADRESL;
            if (analog_result < STEPSIZE - HYSTERESIS) {
                volume = 0;
            } else {
                for (char i = 1; i < NUMBER_OF_STEPS; i++) {                 //iterate through the LEDS
                    int current_step = STEPSIZE * (i);
                    if ((analog_result > (current_step + HYSTERESIS)) && (analog_result < (current_step + STEPSIZE - HYSTERESIS)))
                    {
                        volume = i;
                        break;
                    }
//                    if (analog_result > (current_step + HYSTERESIS)) {         //test if the dial is past the breaking point for the step
//                        PORTA &= (char) ~(1<<(i-1));                                    //disable the LED if the condition is met
//                    } else if (analog_result < (current_step - HYSTERESIS)) {  //test if the dial is before the breaking point for the step
//                        PORTA |= (char) (1<<(i-1));                                     //enable the LED if the condition is met
//                    }
                }
            }
            if (previousVolume == targetVolume) {
                targetVolume = volume; /* Sets targetVolume to volume, when they were equal before. */
            }
        }
        
        if (previousVolume != volume) {
            HCMS29send_string(display2, "Vol.  ");
            HCMS29send_number(display2, volume);
        }
        /*
         * Select input
         */
        
//        inputCounter++;
        if (inputChanged == 1) { // (inputCounter == 20) {
            *display1.BL.address |= (1u << display1.BL.mask);
            HCMS29send_string(display1, "Input: ");
            switch (PORTA & 0b1111) {
                case(0b1110):
                    HCMS29send(display1, '1');
                    break;
                case(0b1101):
                    HCMS29send(display1, '2');
                    break;
                case(0b1011):
                    HCMS29send(display1, '3');
                    break;
                case(0b0111):
                    HCMS29send(display1, '4');
                    break;
                default:
                    HCMS29send(display1, 1);
                    break;
            }
            
            *display1.BL.address &= ~(1u << display1.BL.mask);
            inputChanged = 0;
//            inputCounter = 0;
        }
//        HCMS29send_number(display1, input);
//        PORTDbits.RD0 = 1; 	//Tims Testboard - These two lines are only for Tims test board
//        PORTDbits.RD0 = 0;	//Tims Testboard
        
        /*
         * Infrared
         */
        if(IRindex == 3 && 
           IRbits.C != 0b010 && !oos)           /* If we received 3 bits that are not 0-1-0 we are out of sync */
        {
            oos = 1;                            /* We are  out of sync */
            TMR1 = 27136;                       /* See footnote 5 and 6 */
            T1CONbits.TMR1ON = 1;               /* Turn the Timer1 module on */
        }
        
        
        /* Datastring check if it match Volume up */    
        //if(IR == VOLUME_UP)
        if (IR != 0) { // if new data has been recieved
            if (IR == VOLUME_UP) //(IRbits.C == 0b101) ///((!IRbits.D1)&&(!IRbits.H))
            {
                targetVolume++;
                IR = 0;
            }
            else if /*((!IRbits.D2)&&(!IRbits.H))*/ (IR == VOLUME_DOWN)
            {
                targetVolume--;
                IR = 0;
            }
            else if (IR == INPUT1)
            {
                PORTA = ~(1<<0);
                inputChanged = 1;
                IR = 0;
            }
            else if (IR == INPUT2)
            {
                PORTA = ~(1<<1);
                inputChanged = 1;
                IR = 0;
            }
            else if (IR == INPUT3)
            {
                PORTA = ~(1<<2);
                inputChanged = 1;
                IR = 0;
            }
            else if (IR == INPUT4)
            {
                PORTA = ~(1<<3);
                inputChanged = 1;
                IR = 0;
            }
        }
        if (targetVolume == volume) {
            /* Disable motor; put port 15 and 16 down */ 
            PORTCbits.RC0 = 0;
            PORTCbits.RC1 = 0;
        } else if (targetVolume < volume){
            /* Lower volume
             * Then write to port 15 to turn the motor to make the volume lower */
            PORTCbits.RC1 = 0;
            PORTCbits.RC0 = 1; //links
        } else if (targetVolume > volume){
            /* Raise volume
             * Then write to port 16 to turn the motor to make the volume higher */
            PORTCbits.RC0 = 0;
            PORTCbits.RC1 = 1; //rechts
        }
        
        __delay_ms(10);
    }
}

void interrupt ISR()
{
    if(INTCONbits.RBIF)                     /* The voltage on pin 33 (RB0) changed */
    {
        /*
         * Rotary Encoder
         */
        if(!PORTBbits.RB4)                               /* Is the interrupt caused by external interrupt on PORTB? */
        {
            TMR2 = 5;                               /* Export a new start value to increment from for the next time */
            T2CONbits.TMR2ON = 1;                       /* Turn on the Timer2 module. This creates the delay we want */
            INTCONbits.RBIF = 0;                        /* Clear the interrupt flag for RB */

        }
        /*Infrared*/
        if(PORTBbits.RB0 && !oos)           /* Was the change from negative to positive (rising edge)? And was the signal not oos*/
        {
            TMR1 = 64936;                   /* See footnote 3 and footnote 4 */
            T1CONbits.TMR1ON = 1;           /* Turn on the Timer1 module */
        }
        
        INTCONbits.RBIF = 0;                        /* Clear the interrupt flag for RB */
    }
    
    /*
     * Infrared
     */
    
    if(PIR1bits.TMR1IF)
    {
        T1CONbits.TMR1ON = 0;               /* Stop the Timer1 module (so not another interrupts will occur and wait) */
        
        switch(oos)
        {
            case 0:
                if(PORTBbits.RB0)           /* If the pin is still high after 0,6 milliseconds */
                {
                    IR |= (1<<IRindex);
                    //IRbits.array[IRindex] = 1;        /* We received a 1. Store this information in our union IRbits */
                } else
                {
                    IR &= ~(1<<IRindex);
                    //IRbits.array[IRindex] = 0;        /* We received a 0. Store this information in our union IRbits */
                }

                IRindex += 1;                  /* Increment the IRindex by 1 (next array position */
                break;
                
            case 1:
                IR = 0;                     /* Remove the corrupt data */
                IRindex = 0;                  /* Next time we receive a bit, save it at the first location (so we are sync) */
                
                oos = 0;                    /* Clear the out of sync flag since we are not out of sync anymore. */                
                break;
        }

        if(IRindex == 12) IRindex = 0;          /* The transmission is complete. Let's point to the begin of the array */
 
        PIR1bits.TMR1IF = 0;                /* Clear the interrupt flag in software (so we leave the isr) */
    }
    
    if(PIR1bits.TMR2IF)
    {
        T2CONbits.TMR2ON = 0;                       /* Turn off the Timer2 module so it won't count anymore */
        
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
            
        inputChanged = 1; 
        PIR1bits.TMR2IF = 0;
    }
    
    INTCONbits.INTF = 0;                        /* Clear interrupt flag
                                                 * This causes that we leave the ISR and new interrupts are welcome */
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
    INTCONbits.T0IE     = 0;                    // Disables the Timer0 interrupt
    INTCONbits.INTE     = 0;                    // Disables the INT external interrupt
    INTCONbits.INTF     = 0;                    /* Clear flag while flashing */
    INTCONbits.RBIF     = 0;                    // clear flag
    IOCB                = 0;                    // Clear IOCB register,
    IOCBbits.IOCB0      = 1;                    /* Enable interrupt-on-change for pin 33 (RB0) */
    IOCBbits.IOCB4      = 1;                    /* Cause IOC for pin 37 */
    
    INTCONbits.RBIE     = 1;                    // Enables the PORTB change interrupt
    INTCONbits.PEIE     = 1;                    /* Enable interrupts from the outside */
    INTCONbits.GIE      = 1;                    /* Enable global interrupts, this should happen after all other setup */

    T1CON               = 0;                    /* Timer1 is on (not dependent of a gate)    
                                                 * Use a 1:1 prescaler
                                                 * Do not use a Low-Power oscillator
                                                 * Use the internal oscillator / 4 (instruction time)
                                                 * Timer1 is off */
                                                /* NOTE: The value in register TMR1H and TMR1L is set once a IOC occurred 
                                                 * Here the Timer1 module will be turned on */
    T2CON               = 0b00101001;           /* Use a 1:4 prescaler
                                                 * Use a 1:6 postscaler
                                                 * Do not turn the Timer2 module on */
    
    PIE1                = 0;                    /* Disable all interrupts described in the PIE1 register */
    PIE1bits.TMR1IE     = 1;                    /* Enable Timer1 overflow interrupt */
    PIE1bits.TMR2IE     = 1;                    /* Enable Timer2 interrupt (NOTE: only software will trigger) */
    INTCONbits.GIE      = 1;                    /* All interrupts have been configured. We can enable the global interrupt */
    
    IRindex   = 0;                              /* Start the IRindex of the array (IRbits) at position 0 */
    oos     = 0;                                /* Let's assume that we are at init time not out of sync */
    IR      = 0;
}
