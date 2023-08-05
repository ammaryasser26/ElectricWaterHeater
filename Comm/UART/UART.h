/*
 * UART.h
 *
 * Created: 3/21/2023 3:28:26 PM
 *  Author: Ahmed Nour
 */ 


#ifndef UART_H_
#define UART_H_
#include "../../utilities/BIT_MATH.h"
#include "../../utilities/Data_Type.h"
#include "../../utilities/DIO_REG.h"
#include "../../utilities/Error_State.h"
void UART_init();
void UART_SendData(uint8 data);
uint8 UART_ReceivedData();

#endif /* UART_H_ */