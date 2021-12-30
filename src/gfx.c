#include "gfx.h"
#include "i2c.h"
#include "config.h"


void update_game_graphics(uint64_t screen){

    uint8_t data[17];
    uint32_t dataLength = 17;

    //break up screen into bytes and fill data
    int i;
    uint8_t x;
    for(i = 0; i < 8; i++){
         x = (uint8_t)( (screen >> (i*8)) & 0xFF);
        x = (x >> 1) | (x << (8 - 1));
        data[1 + i*2] = x;
    }

    //pad data with zeros
    for(i = 0; i < 9; i++){
        data[2*i] = (uint8_t)0x00;
    }

    //bus_clear(EUSCI_B0);
    i2c_start(EUSCI_B0, LEDMATRIX_ADDRESS, WRITE, data, 17, 0x00);

    for(i = 0; i < 1000; i++);

}


void graphics_setup(void) {

    uint8_t select_row_output = LED_SELECT_ROW_OUTPUT;
    i2c_start(EUSCI_B0, 0x70, WRITE, &select_row_output, 1, 0x00);

    uint8_t enable_osc = LED_ENABLE_OSC;
    i2c_start(EUSCI_B0, 0x70, WRITE, &enable_osc, 1, 0x00);

    uint8_t enable_display = LED_ENABLE_DISPLAY;
    i2c_start(EUSCI_B0, 0x70, WRITE, &enable_display, 1, 0x00);

    update_game_graphics(0x00);
}
