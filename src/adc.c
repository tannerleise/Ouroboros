#include "adc.h"


static int jx, jy;
static direction_typedef dir;

void adc_setup(void){
    ADC14->CTL0 =  ADC14_CTL0_ON;               /* power on and disabled during configuration */
    ADC14->CTL0 |= ADC14_CTL0_SHP;              /* S/H pulse mode */
    ADC14->CTL0 |= ADC14_CTL0_SSEL__SYSCLK;     /* sysclk */
    ADC14->CTL0 |= ADC14_CTL0_SHT0__32;         /* 32 sample clocks */
    ADC14->CTL0 |= ADC14_CTL0_CONSEQ_1;         /* Define ADC as reading a sequence of channeks */
    ADC14->CTL1 =  ADC14_CTL1_RES__12BIT;       /* 12-bit resolution */
    ADC14->MCTL[5] = 1;                         /* A1 input, single-ended, Vref=AVCC */
    ADC14->MCTL[6] = 0;                         /* A0 input, single-ended, Vref=AVCC */
    ADC14->MCTL[6] |= ADC14_MCTLN_EOS;          /* Define MCTL 6 as the end of the sequence */
    ADC14->IER0 |= ADC14_IER0_IE5;              /* Enable Interrupts */
    P5->SEL1 |= BIT4;                           /* Configure P5.4 for A1 */
    P5->SEL0 |= BIT4;
    P5->SEL1 |= BIT5;                           /* Configure P5.5 for A0 */
    P5->SEL0 |= BIT5;
    ADC14->CTL1 |= 0x00050000;                  /* convert for mem reg 5 */
}

void adc_enable(void){
    ADC14->CTL0 |= 2;                           /* enable ADC after configuration*/
}
void adc_start_convert(void){
    ADC14->CTL0 |= ADC14_CTL0_SC;               /* start a conversion */
}
void adc_NVIC_setup(void){
    // setup NVIC
    // Note: ADC is INTISR(24)
    NVIC->IP[24] |= 0x20;                       /* Set a priority */
    NVIC->ISER[0] |= 0x01000000;                /* ISER0 starts at 0 */
    return;
}
/* Interrupt for ADC writes values in MEM (scaled voltages on pins) to variables jx and jy */
void ADC14_IRQHandler(void){
    jx = ADC14->MEM[5];                         /* Write value of pin 5.5 to jx */
    jy = ADC14->MEM[6];                         /* Write value of pin 5.6 to jy */

    //assign direction based on limits;
    if(jy > 3600 && jx > 1500 && jx < 2500){
        dir = up;
        #ifdef ben
        dir = right; //Ben's board is special :(
        #endif
    }
    if(jy < 400 && jx > 1500 && jx < 2500){
        dir = down;
        #ifdef ben
        dir = left; //Ben's board is special :(
        #endif
    }
    if(jx < 400 && jy > 1500 && jy < 2500){
        dir = left;
        #ifdef ben
        dir = up; //Ben's board is special :(
        #endif
    }
    if(jx > 3600 && jy > 1500 && jy < 2500){
        dir = right;
        #ifdef ben
        dir = down; //Ben's board is special :(
        #endif
    }
    ADC14->CLRIFGR0 |= 0x00000020;              /* clear ADC interrupt  flag */
}

void get_joystick_value(int *jxTarget, int *jyTarget){
    *jxTarget = jx;
    *jyTarget = jy;
}
direction_typedef get_joystick_dir(){
    return dir;
}
