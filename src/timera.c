#include "timera.h"

/********************/
/* public functions */
/********************/

// Sets up a TimerA of your choosing. Relatively modular!
void timera_setup(TimerA_Setup_TypeDef timera_setup_struct) {

    // Make sure arguments are valid
    assert(                                 // Verify timer value
           timera_setup_struct.timera == TIMER_A0 ||
           timera_setup_struct.timera == TIMER_A1
           );

    assert(                                 // Verify first division
           timera_setup_struct.ctl_id == TIMER_A_CTL_ID_0 ||
           timera_setup_struct.ctl_id == TIMER_A_CTL_ID_1 ||
           timera_setup_struct.ctl_id == TIMER_A_CTL_ID_2 ||
           timera_setup_struct.ctl_id == TIMER_A_CTL_ID_3
           );

    assert(                                 // Verify second division
           timera_setup_struct.ex0 == TIMER_A_EX0_TAIDEX_0 ||
           timera_setup_struct.ex0 == TIMER_A_EX0_TAIDEX_1 ||
           timera_setup_struct.ex0 == TIMER_A_EX0_TAIDEX_2 ||
           timera_setup_struct.ex0 == TIMER_A_EX0_TAIDEX_3 ||
           timera_setup_struct.ex0 == TIMER_A_EX0_TAIDEX_4 ||
           timera_setup_struct.ex0 == TIMER_A_EX0_TAIDEX_5 ||
           timera_setup_struct.ex0 == TIMER_A_EX0_TAIDEX_6 ||
           timera_setup_struct.ex0 == TIMER_A_EX0_TAIDEX_7
           );

    assert(                                 // Verify tick value
           timera_setup_struct.ticks < 65536 &&
           timera_setup_struct.ticks > 0
           );

    // General settings settin'
    timera_setup_struct.timera->CTL |= TIMER_A_CTL_CLR;
    timera_setup_struct.timera->CTL |= TIMER_A_CTL_SSEL__SMCLK;
    timera_setup_struct.timera->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_0;
    timera_setup_struct.timera->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_4;

    // Set timing
    timera_setup_struct.timera->CTL |= timera_setup_struct.ctl_id;
    timera_setup_struct.timera->EX0 = timera_setup_struct.ex0;
    timera_setup_struct.timera->CCR[0] = timera_setup_struct.ticks;


    // Enable interrupts
    timera_setup_struct.timera->CCTL[1] |= TIMER_A_CCTLN_CCIE;
    timera_setup_struct.timera->CTL |= TIMER_A_CTL_IE;

}

// Does what it says on the tin
void timera_start(Timer_A_Type *timera) {
    timera->CTL |= TIMER_A_CTL_MC__UP;
    (timera == TIMER_A0) ? __NVIC_EnableIRQ(TA0_N_IRQn) : __NVIC_EnableIRQ(TA1_N_IRQn);
}

// The yin to timera_start()'s yang
void timera_stop(Timer_A_Type *timera) {
    timera->CTL |= TIMER_A_CTL_MC__STOP;
}

//void timera_enable_interrupts(Timer_A_Type *timera) {
//    switch(timera) {
//    case TIMER_A0:
//        _NVIC_EnableIRQ()
//    }
//}

void timera_test(TimerA_Setup_TypeDef timera_setup_struct) {
    timera_setup(timera_setup_struct);

    P2->DIR |= BIT4;
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~BIT4;
    P2->OUT = 0x00;

    timera_start(timera_setup_struct.timera);

}

void TA0_N_IRQHandler(void) {
    __disable_irq();
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;

    adc_start_convert();

    __enable_irq();
}

void TA1_N_IRQHandler(void) {
    __disable_irq();
    TIMER_A1->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;

    gameTick();

    __enable_irq();
}
