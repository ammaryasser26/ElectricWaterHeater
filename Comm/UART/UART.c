/*
 * UART.c
 *
 * Created: 3/21/2023 3:27:53 PM
 *  Author: Ahmed Nour
 */ 

#include "UART.h"



void UART_SendData(uint8 data)
{
	while(Get_Bit(UCSRA_REG,5)==0);// wait until empty ;
	UDR_REG=data;
	while(Get_Bit(UCSRA_REG,6)==0); // transmitter complete
	
}

uint8 UART_ReceivedData()
{
	uint8 data ;
	while(Get_Bit(UCSRA_REG,7)==0); // received  received complete
	data= UDR_REG;
	return data; 
}

void UART_init()
{
	uint8	Local_UCSRC;
	Set_Bit(UCSRB_REG,4); //received enabled
	Set_Bit(UCSRB_REG,3); //TX enabled
	
	Clear_Bit(Local_UCSRC,6); // ASY
	Clear_Bit(Local_UCSRC,5); // parity disabled
	Clear_Bit(Local_UCSRC,4); // 
	
	Clear_Bit(Local_UCSRC,3); // one stop bit
	Set_Bit(Local_UCSRC,2); // 
	Set_Bit(Local_UCSRC,1); //
	
	Set_Bit(UCSRB_REG,2); // enabled
	UCSRC_REG=Local_UCSRC; 
	
	UBRRL_REG=6; // 9600 boud rate
	
}