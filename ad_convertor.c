#include <xc.h>
#include "get_voltage.h"
#include "pic_setting.h"

#define CLOCK_SET  0x70

void ad_con_init()
{
    PORTA = 0x00;           
    PORTB = 0x00;           
    PORTC = 0x00;
    
    TRISA = 0b00001100;     
    TRISB = 0b00011100;     
    TRISC = 0b10000000;     // RC7 RX
                            // RC6 TX
    
    OSCCON  = (OSCCON | CLOCK_SET);
    INTCON  = 0b01000000;
    CM1CON0 = 0;
    CM2CON0 = 0;
    ANSEL   = 0b00000110;
    ANSELH  = 0;
    ADCON1  = 0b00000000;
    
}

static int bit_shift(char my_adresh, char my_adresl)
{
    int i, j;
    char buf = 0x00;
    int  ad_value;
     
    char bit_mask = 0b10000000;

    
    for(i = 0; i < 8; i++)
    {
        buf = my_adresh & bit_mask;
        buf = buf >> 7;
        ad_value |= (int)buf;
        ad_value = ad_value << 1;
        my_adresh = my_adresh << 1;
    }
        
        
    for(j = 0; j < 2; j++)
    {      
        buf = my_adresl & bit_mask;
        buf = buf >> 7;
        ad_value |= (int)buf;
    
        if (j < 1)
        {
            ad_value = ad_value << 1; 
        }
            
        my_adresl = my_adresl << 1;
    }
    return ad_value;
}

double get_voltage_adcon(void)
{
    /* ADcon max */
    const unsigned int range = 0x3ff;
    /* AD value */
    double ad_value2;
    char my_adresh;
    char my_adresl; 
    int ad_value = 0x00;    
       
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
    
    ad_value = bit_shift(my_adresh, my_adresl);
        
    ad_value2 = ad_value * 5.00 / range;
        
    return ad_value2;   
}

