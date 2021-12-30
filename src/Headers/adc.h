#ifndef _ADC_HG
#define _ADC_HG

#include "msp.h"
#include "config.h"
//Written by Ben Pera

typedef enum{
    up, down, left, right
} direction_typedef;

//joy stick variables -- Can be read from anywhere


//functions
void adc_setup(void);           /* Sets up the ADC to read between 0 and 3.3v scaled to 0 to 4096 on pins 5.4 and 5.5 */
void adc_start_convert(void);   /* Starts a conversion in the ADC unit */
void adc_enable(void);          /* Enables the ADC */
void adc_NVIC_setup(void);      /* Sets up the NVIC for the ADC */
void get_joystick_value(int *jxTarget, int *jyTarget);  /* Get the values from the joystick */
direction_typedef get_joystick_dir(void);         /* Gets direction enum */

#endif
