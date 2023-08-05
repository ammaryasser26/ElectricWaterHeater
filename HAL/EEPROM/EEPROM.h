/*
 * EEPROM.h
 *
 * Created: 4/26/2023 1:52:34 AM
 *  Author: Ahmed Nour
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include "../../Comm/I2C/I2C.h"
#include "../../Comm/I2C/I2C_Config.h"
void EEPROM_Write(uint16 location_Address,uint8 Copy_Data);

uint8 EEPROM_Read(uint16 location_Address);

#endif /* EEPROM_H_ */