/*
 * SPI.c
 *
 * Created: 3/22/2023 10:58:57 PM
 *  Author: Ahmed Nour
 */ 

#include "SPI.h"
void Spi_MASTERInit()
{
	/*select the spi as master */
	Set_Bit(SPCR_REG,SPCR_MSTR);
	/*select the clk polarity :falling rising*/
	//Set_Bit(SPCR_REG,SPCR_CPOL);
	/*select CLK Phase :send-receive */ 
	//Set_Bit(SPCR_REG,SPCR_CPHA);
	/*select the CLK rate : (f/8) */ 
	Set_Bit(SPCR_REG,SPCR_SPR0);
	Clear_Bit(SPCR_REG,SPCR_SPR1);
	//Set_Bit(SPSR_REG,SPSR_SPI2X);
	/* spi enable */
	Set_Bit(SPCR_REG,SPCR_SPE);
}

void Spi_SlaveInit()
{
	/*select the spi as slave */
	Clear_Bit(SPCR_REG,SPCR_MSTR);
	/* 
	 * i don't interest in slave clk bec take from master
	//select the clk polarity :falling rising
	Set_Bit(SPCR_REG,SPCR_CPOL);
	//select CLK Phase :send-receive 
	Set_Bit(SPCR_REG,SPCR_CPHA);
	//select the CLK rate : (f/8) 
	Set_Bit(SPCR_REG,SPCR_SPR0);
	Clear_Bit(SPCR_REG,SPCR_SPR1);
	Set_Bit(SPSR_REG,SPSR_SPI2X);*/
	/* spi enable */
	Set_Bit(SPCR_REG,SPCR_SPE);
}

uint8 Spi_MasterSendData(uint8 copy_Data)
{
	SPDR_REG= copy_Data; //the problem is thats take one cycle and the data is 8 so we make busy wait 
	while((Get_Bit(SPSR_REG,SPSR_SPIF)==0));
	return SPDR_REG;
}

uint8 Spi_Transceive(uint8 copy_Data)
{
	SPDR_REG= copy_Data; // if we need received the send data is garbage value 
	/*wait until the received complete*/
	while((Get_Bit(SPSR_REG,SPSR_SPIF)==0)); // the same problem with same solution 
	//copy_Data=SPDR_REG;
	return SPDR_REG;
}