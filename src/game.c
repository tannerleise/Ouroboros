#include "game.h"

typedef struct{
    direction_typedef dir;
    uint64_t headIndex;
    uint64_t headLocal;
    gameState_typedef state;
    uint64_t snakeBuffer[64];
} snakeState_typedef;

uint64_t fruitPos;


static snakeState_typedef ss;

void setGameState(gameState_typedef state){
    ss.state = state;
}

gameState_typedef getGameState(){
    return ss.state;
}

bool overlapWithSnake(uint64_t testPosition){
    int i;
    for(i = 0; i <= ss.headIndex; i++){
        if(testPosition == ss.snakeBuffer[i])
            return true;
    }
    return false;
}

int randomPos(int lower, int upper)
{
    int num = (rand() % (upper - lower)) + lower;
    uint64_t output;
    output = 1 << num;
    return output;
}

void PORT5_IRQHandler(void)
{
    __disable_irq();
    __NVIC_DisableIRQ(PORT5_IRQn);

    (ss.state == menu) ? (ss.state = play) : (ss.state = menu);

     P5-> IFG &= ~BIT2; //clear interrupt flag

     __NVIC_EnableIRQ(PORT5_IRQn);
     __enable_irq();
}

void setupButton(void){
    __disable_irq();

    //pin stuffs
    P5 -> DIR &= ~BIT2; //configure 5.2 as input
    P5 -> REN |= BIT2; //enable pull up/down resistor
    P5 -> OUT |= BIT2; //choose pull up resistor

    P5 -> SEL0 &= ~BIT2; //configure as gpio
    P5 -> SEL1 &= ~BIT2;

    P5 -> IFG &= ~BIT2; //clear interrupt flag
    P5 -> IES |= BIT2; //set interrupt at falling edge
    P5 -> IE |= BIT2;   //enable interrupt

    __NVIC_EnableIRQ(PORT5_IRQn); //enable interrupts on port 5

    __enable_irq();
}

void gameTick(){
    if(ss.state == play){
        //update direction of snake head, checking to make sure its not backwards
        direction_typedef tempDir = get_joystick_dir();
        if ((ss.dir == left && tempDir == right) ||
            (ss.dir == right && tempDir == left) ||
            (ss.dir == up && tempDir == down) ||
            (ss.dir == down && tempDir == up)) {
            // do nothing
        } else {
            ss.dir = tempDir;
        }

        //Generate new head position
        switch (ss.dir) {
            //bullshit code follows please don't ask questions
              case up:
                  ss.headLocal = (ss.headLocal & TOP_DOTS_MASK) ? (ss.headLocal >> 56) : (ss.headLocal << 8);  // if we're going up
                  break;
              case down:
                  ss.headLocal = (ss.headLocal & BOTTOM_DOTS_MASK) ? (ss.headLocal << 56) : (ss.headLocal >> 8);  // if we're going down
                  break;
              case left:
                  ss.headLocal = (ss.headLocal & LEFT_DOTS_MASK) ? (ss.headLocal >> 7) : (ss.headLocal << 1);      // if we're going left
                  break;
              case right:
                  ss.headLocal = (ss.headLocal & RIGHT_DOTS_MASK) ? (ss.headLocal << 7) : (ss.headLocal >> 1);      // if we're going right
                  break;
              default:
                  assert(false);      // this shouldn't be possible
                  break;
          //Brought to you by Peter Magro
        }

        //check if ded
        if(overlapWithSnake(ss.headLocal)){
            ss.state = ded;
        } else{
            //check if hit fruit
            if(ss.headLocal == fruitPos){
                ss.headIndex++;
                ss.snakeBuffer[ss.headIndex] = ss.headLocal;

                //Generate fruit position to make sure it works
                while(overlapWithSnake(fruitPos) || fruitPos == 0){
                    fruitPos = randomPos(0,63);
                }
            }
            else{
                //move snake array back
                int i;
                for(i = 0; i < ss.headIndex; i++){
                    ss.snakeBuffer[i] = ss.snakeBuffer[i + 1];
                }
                //fill in snake head
                ss.snakeBuffer[ss.headIndex] = ss.headLocal;
            }
        }
        uint64_t snakeScreen = ss.snakeBuffer[0];
        int i;
        for(i = 0; i <= ss.headIndex; i++){
            snakeScreen |=ss.snakeBuffer[i];
        }
        snakeScreen |= fruitPos;
        update_game_graphics(snakeScreen);
    }else if(ss.state == ded){
        update_game_graphics(SAD_FACE);
    }else if(ss.state == menu){
        update_game_graphics(HAPPY_FACE);
        gameStart();
    }

}

void gameStart() {

    setupButton();

    //initialize ssstruct
    ss.dir = INITIAL_DIRECTION;
    ss.headIndex = INITIAL_LENGTH;
    ss.headLocal = INITIAL_POSITION;
    //ss.state = play;
    int i;
    for(i = 0; i < 64; i++){
        ss.snakeBuffer[i] = 0;
    }

    //generate initial snake screen
    for (i = ss.headIndex; i >= 0; i--) {
      uint32_t inv_i = ss.headIndex - i;
      uint64_t segmentPos = 0;
      switch (ss.dir) {
      case up:
        segmentPos = ss.headLocal >> (8*i);  // if we're going up, the tail will be above us
        break;
      case down:
        segmentPos = ss.headLocal << (8*i);  // if we're going down, the tail will be below us
        break;
      case left:
        segmentPos = ss.headLocal >> i;      // if we're going left, the tail will be to our right
        break;
      case right:
        segmentPos = ss.headLocal << i;      // if we're going right, the tail will be to our left
        break;
      default:
        assert(false);      // this shouldn't be possible
        break;
      }
      ss.snakeBuffer[inv_i] = segmentPos;
    }

    //generate initial fruit position
    fruitPos = INITIAL_FRUIT_POSITION;
}
