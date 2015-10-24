#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "UART01.h"

#define CLOCK_SET  0x70

// 16F886
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

//CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#ifndef _XTAL_FREQ

    #define _XTAL_FREQ 8000000

#endif

void ad_init(void)
{
    
    PORTA = 0x00;           // PORTA????
    PORTB = 0x00;           // PORTB????
    PORTC = 0x00;
    
    TRISA = 0b00001100;     // PORTA??????
    TRISB = 0b00011100;     // PORTB??????
    TRISC = 0b10000000;     // RC7??
                            // RC6??
    
    OSCCON  = (OSCCON | CLOCK_SET);
    INTCON  = 0b01000000;
    CM1CON0 = 0;
    CM2CON0 = 0;
    ANSEL   = 0b00000110;
    ANSELH  = 0;
    ADCON1  = 0b00000000;
    
}

 
int main(void)
{
    /* ADcon max */
    const unsigned int range = 0x3ff;
    
    /* AD value */
    int  ad_value = 0x00;
    char ad_value2;
    char bit_mask = 0b10000000;
    char my_adresh;
    char my_adresl;
    char buf = 0x00;
    int  buf2;
    int i, j;
    
    
    
    ad_init();
    initUART();             // ????????????
    
    __delay_ms(1000);
    printf("Start Program:\r\n");
    
    
    
    while(1)
    {
        __delay_ms(1000);
        
        ADCON0 = 0b10001001;
        __delay_ms(10);
        ADCON0bits.GO = 1;
        
        while(ADCON0bits.GO_DONE)
        {
            ;
        }
        
        my_adresh = ADRESH;
        my_adresl = ADRESL;
        printf("my_adresh = %d, my_adresl = %d\r\n", my_adresh, my_adresl);
        
        for(i = 0; i < 8; i++)
        {
            buf2 = 0;
            buf = my_adresh & bit_mask;
            buf = buf >> 7;
            buf2 = (int)buf;
            
            ad_value |= buf2;
           
            ad_value = ad_value << 1;
            
            my_adresh = my_adresh << 1;
        }
        
        printf("ad_value = 0x%x\r\n", ad_value);
        
        for(j = 0; i < 2; i++)
        {
            
            buf = my_adresl & bit_mask;
            buf = buf >> 7;
            ad_value |= (int)buf;
            if (i < 1)
            {
                ad_value = ad_value << 1; 
            }
            my_adresl = my_adresl << 1;
        }
        printf("ad_value = 0x%x\r\n", ad_value);
        
        ad_value2 = ad_value * 5.00 / range;
        
        printf("AD2 = %.2f [V]\r\n", ad_value2);
        
    }
       
}
