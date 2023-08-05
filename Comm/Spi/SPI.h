/*
 * SPI.h
 *
 * Created: 3/22/2023 10:59:14 PM
 *  Author: Ahmed Nour
 */ 


#ifndef SPI_H_
#define SPI_H_
#include "../../utilities/BIT_MATH.h"
#include "../../utilities/Data_Type.h"
#include "../../utilities/DIO_REG.h"
#include "../../utilities/Error_State.h"

void Spi_MASTERInit();
void Spi_SlaveInit();
uint8 Spi_MasterSendData(uint8 copy_Data);
uint8 Spi_Transceive(uint8 copy_Data);


#endif /* SPI_H_ */