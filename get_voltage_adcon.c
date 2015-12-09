#include <xc.h>
#include "get_voltage.h"
#include "pic_setting.h"    //_XTAL_FREQ defined


/* configure internal clock setting -> 8MHz */
#define CLOCK_SET  0x70


/*---------------------------------------------
 * Initialize A/D convertor Register Setting
 *-------------------------------------------*/
void ad_con_init(void)
{
    /* Initialize PORT Register */
    PORTA = 0x00;           
    PORTB = 0x00;           
    PORTC = 0x00;

    /* Initialize TRIS Register */
    TRISA = 0b00001100;     
    TRISB = 0b00011100;     
    TRISC = 0b10000000;     // RC7 = RX, RC6 = TX

    /* Set Internal Clock Frequency */
    OSCCON  = (OSCCON | CLOCK_SET);

    
    INTCON  = 0b01000000;
    CM1CON0 = 0;
    CM2CON0 = 0;

    /* Initialize Analog Select Register */
    ANSEL   = 0b00000110;
    ANSELH  = 0;

    /* Initialize A/D convertor Register */
    ADCON1  = 0b00000000;
    
}


/*---------------------------------------------
 * get voltage from Register
 *-------------------------------------------*/
static int bit_shift(char my_adres_h, char my_adres_l)
{
    int i, j;
    int ad_value;
    
    char buf      = 0x00;
    char bit_mask = 0b10000000;

    /* get value from ADRESH */
    for(i = 0; i < 8; i++)
    {
        buf = my_adres_h & bit_mask;
        buf >>= 7;
        ad_value |= (int)buf;
        ad_value <<= 1;
        my_adres_h <<= 1;
    }
        
    /* get value from ADRESL */    
    for(j = 0; j < 2; j++)
    {      
        buf = my_adres_l & bit_mask;
        buf >>= 7;
        ad_value |= (int)buf;
    
        if (j < 1)
        {
            ad_value <<= 1; 
        }
            
        my_adres_l <<= 1;
    }
    /* return voltage value that is type of int */
    return ad_value;
}


/*---------------------------------------------
 * get voltage using A/D convertor
 *-------------------------------------------*/
double get_voltage_adcon(void)
{
    /* A/D con Range */
    const unsigned int range = 0x3ff;
    
    /* A/D value */
    int    ad_value = 0x00;
    double voltage_value;

    /* variables to recive binary from Register */
    char my_adres_h;
    char my_adres_l; 
    
    /* A/D Con Enable */
    __delay_ms(1000);    
    ADCON0 = 0b10001001;
    __delay_ms(10);
    ADCON0bits.GO = 1;

    /* waiting for finish of A/D */
    while(ADCON0bits.GO_DONE)
    {
        ;
    }

    /* get binary from Register */
    my_adres_h = ADRESH;
    my_adres_l = ADRESL;
    ad_value = bit_shift(my_adres_h, my_adres_l);

    /* calculate value of voltage */
    voltage_value = ad_value * 5.00 / range;
        
    return voltage_value;
}

