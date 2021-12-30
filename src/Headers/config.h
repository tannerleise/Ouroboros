#ifndef _CONFIG_HG
#define _CONFIG_HG

//define if you're Ben or using the 0.8" LED matrix
//#define ben

// ADC settings


// Game settings
#define INITIAL_POSITION        0x800000000
#define INITIAL_LENGTH           2
#define INITIAL_FRUIT_POSITION   0x200000000
#define INITIAL_DIRECTION        right


// GFX settings
#define SAD_FACE            0x3C42A58199A5423C
#define HAPPY_FACE          0x3C42A581A599423C
                            //3C42A581A599423C

#define LED_SELECT_ROW_OUTPUT   0b00100001
#define LED_ENABLE_OSC          0b10100000
#define LED_ENABLE_DISPLAY      0b10000001

// I2C settings
#define I2C_UCA10           0u
#define I2C_UCSLA10         0u
#define I2C_UCMM            0u
#define I2C_UCMST           EUSCI_B_CTLW0_MST
#define I2C_UCSSELX         EUSCI_B_CTLW0_SSEL__SMCLK
#define I2C_UCBRX           (uint32_t)30u

// Timer A settings
#define ADC_TIMER           TIMER_A0
#define ADC_TIMER_CTL_ID    TIMER_A_CTL_ID__1
#define ADC_TIMER_EX0       TIMER_A_EX0_IDEX__1
#define ADC_TIMER_TICKS     (uint32_t)1499u      // TICKS = (freq / 2^id) * 2 * period
                                    // freq = 3MHz
                                    // ID = 4
                                    // period = 0.05

#define GAME_TIMER          TIMER_A1
#define GAME_TIMER_CTL_ID   TIMER_A_CTL_ID__8
#define GAME_TIMER_EX0      TIMER_A_EX0_IDEX__2
#define GAME_TIMER_TICKS    (uint32_t)46874u

#define LEDMATRIX_ADDRESS   (uint8_t)0x70

#endif
