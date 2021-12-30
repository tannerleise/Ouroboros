#ifndef _TIMERA_HG
#define _TIMERA_HG

/************/
/* includes */
/************/
#include "msp.h"
#include "assert.h"

/***********/
/* defines */
/***********/

/************/
/* typedefs */
/************/
typedef struct {
    Timer_A_Type *timera;
    uint32_t ctl_id;
    uint32_t ex0;
    uint32_t ticks;
} TimerA_Setup_TypeDef;

/***********************/
/* function prototypes */
/***********************/
void timera_setup(TimerA_Setup_TypeDef timera_setup);
void timera_start(Timer_A_Type *timera);
void timera_stop(Timer_A_Type *timera);
void timera_test(TimerA_Setup_TypeDef timera_setup);

#endif
