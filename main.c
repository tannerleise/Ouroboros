#include "main.h"
/**
 * main.c
 */

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	adc_setup();
    adc_NVIC_setup();

    TimerA_Setup_TypeDef adc_timera = {
                                   ADC_TIMER,
                                   ADC_TIMER_CTL_ID,
                                   ADC_TIMER_EX0,
                                   ADC_TIMER_TICKS
    };

    TimerA_Setup_TypeDef game_timera = {
                                   GAME_TIMER,
                                   GAME_TIMER_CTL_ID,
                                   GAME_TIMER_EX0,
                                   GAME_TIMER_TICKS
    };

    timera_setup(adc_timera);
    timera_setup(game_timera);

    __enable_irq();

    I2C_OPEN_STRUCT_TypeDef i2c_open_struct;

    i2c_open_struct.uca10 = 0;                              //no self 10-bit Address
    i2c_open_struct.ucsla10 = 0;                            //no 10-bit Addressing
    i2c_open_struct.ucmm = 0;                               //No Multi-Controller
    i2c_open_struct.ucmst = EUSCI_B_CTLW0_MST;              //Controller Mode
    i2c_open_struct.ucsselx = EUSCI_B_CTLW0_SSEL__SMCLK;    //SMCLK to be selected (3MHz)
    i2c_open_struct.ucbrx = 30;                             //Prescaler for Selected Clock.
                                                            //(100kHz)
    //This sets up the I2C driver to operate with the correct settings.
    //EUSCI_B0 uses P1.7 as SCL and P1.6 as SDA
    i2c_open(EUSCI_B0, &i2c_open_struct);
    adc_enable();
    graphics_setup();
    gameStart();
    setGameState(menu);
    //timera_start(ADC_TIMER);
    //timera_start(GAME_TIMER);


    volatile int i;

    while(1){
        for(i = 0; i < 8000; i++){
            //__delay_cycles(25);
            adc_start_convert();
        }
        gameTick();
        /*
        for(i = 0; i < 100000; i++);
        update_game_graphics(HAPPY_FACE);
        for(i = 0; i < 100000; i++);
        update_game_graphics(SAD_FACE);
        */
    }

}
