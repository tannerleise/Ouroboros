#ifndef _GAME_HG
#define _GAME_HG


#include "msp.h"
#include "adc.h"
#include "config.h"
#include "assert.h"
#include "gfx.h"
#include <stdlib.h>
#include <stdbool.h>

#define LEFT_DOTS_MASK      0x8080808080808080
#define RIGHT_DOTS_MASK     0x0101010101010101
#define TOP_DOTS_MASK       0xFF00000000000000
#define BOTTOM_DOTS_MASK    0x00000000000000FF

typedef enum{
    play, ded, menu
} gameState_typedef;

void gameTick();                            /* Executes the full game run loop */
void gameStart();                           /* Initalize the snake state at default */
void setGameState(gameState_typedef state); /* sets the game state */
void setupButton(void);                       /* Sets up GPIO to handle joystick button press, call this function at MSP init */
gameState_typedef getGameState();           /* sets the game state */
#endif
