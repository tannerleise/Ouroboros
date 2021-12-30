//#ifndef _I2C_HG_
//#define _I2C_HG_
//
///************/
///* includes */
///************/
//#include "msp.h"
//#include "assert.h"
//#include <stdbool.h>
//
///***********/
///* defines */
///***********/
//#define I2C_READ        1
//#define I2C_WRITE       0
//
///************/
///* typedefs */
///************/
//
//typedef struct {
//    uint32_t        uca10;          // Own Addressing Mode
//    uint32_t        ucsla10;        // Peripheral Receiver Addressing Mode
//    uint32_t        ucmm;           // Multi-Controller Select
//    uint32_t        ucmst;          // Controller Mode Select
//    uint32_t        ucsselx;        // eUSCI_B Clock Source Select
//    uint16_t        ucbrx;          // Bit Clock prescaler
//} I2C_Open_TypeDef;
//
//typedef struct {
//    uint32_t        receiver_address;
//    uint32_t        register_address;
//    char            *data;          // this is an array!
//    uint32_t        data_length;
//} I2C_Write_Data_TypeDef;
//
//typedef struct {
//    uint32_t        receiver_address;
//    uint32_t        register_address;
//    char            *data;          // this is a pointer to a variable!
//
//} I2C_Read_Data_TypeDef;
//
///***********************/
///* function prototypes */
///***********************/
//
//void i2c_open(I2C_Open_TypeDef *open_struct);
//void i2c_write(I2C_Write_Data_TypeDef writeData);
//void i2c_read(I2C_Read_Data_TypeDef readData);
//void i2c_close(EUSCI_B_Type *i2c);
//
//#endif
//

/*
 * i2c.h
 *
 *  Created on: Jun 16, 2020
 *      Author: Tyler Davidson
 *    Modified: Oct 2, 2020
 * Modified by: Arielle Blum
 */

#ifndef I2C_H_
#define I2C_H_

#include "msp.h"
#include "assert.h"
#include "stdbool.h"


#define READ        true
#define WRITE       false


//Structs
typedef struct {

    uint32_t        uca10;          //Own Addressing Mode
    uint32_t        ucsla10;        //Peripheral Receiver Addressing Mode
    uint32_t        ucmm;           //Multi-Controller Select
    uint32_t        ucmst;          //Controller Mode Select
    uint32_t        ucsselx;        //eUSCI_B Clock Source Select
    uint16_t        ucbrx;          //Bit Clock prescaler

} I2C_OPEN_STRUCT_TypeDef;



typedef struct {
    uint8_t                 SA;         //Peripheral Receiver address to communicate with
    bool                    RW;         //read or write bit
    uint8_t                 *data;      //pointer to an array of data to be sent/received
    uint8_t                 dataBytes;  //how many bytes of data to transfer
    volatile uint8_t        byteCount;  //variable to keep track of how many bytes have TXed/RXed
    EUSCI_B_Type            *i2c;       //Device Struct for accessing I2C registers
    uint8_t                 deviceReg;  //register from which to read from

} I2C_PAYLOAD_TypeDef;

//Functions
void i2c_open(EUSCI_B_Type *i2c, I2C_OPEN_STRUCT_TypeDef *i2c_open_struct);
void i2c_start(EUSCI_B_Type *i2c, uint8_t Address, bool RW, uint8_t *data, uint8_t dataBytes,uint8_t deviceReg);
void bus_clear(EUSCI_B_Type *i2c);

void EUSCIB0_IRQHandler(void);

#endif /* I2C_H_ */
