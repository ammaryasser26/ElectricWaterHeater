/*
 * I2C.c
 *
 * Created: 3/29/2023 5:40:41 AM
 *  Author: Ahmed Nour
 */ 
#include "I2C.h"
void TWI_voidInitMaster(uint8 Copy_Address)
{
	/*Enable ACKnowledge bit */ 
	Set_Bit(TWCR_REG,TWCR_TWEA);
	
	/* assume the speed = 400 ,8MHZ freq */
	/* by sub in the equation : the TWBR=2,TWPS=0  */
	/*we made this to generate clk by master*/
	TWBR_REG=2;
	
	/* Prescaler bits: 2bit in TWPS*/
	Clear_Bit(TWSR_REG,TWSR_TWPS0);
	Clear_Bit(TWSR_REG,TWSR_TWPS1);
	
	/*to make any change or make any step we need to clear flag*/
	Clear_Bit(TWCR_REG,TWCR_TWINT);
	
	/*we need to check if the master has address or not*/
	if(Copy_Address!=0)
	{
		/*Set the Master address*/
		TWAR_REG=Copy_Address<<1;
	}
	else
	{
		//do nothing
	}
	/*Enable TWI*/
	Set_Bit(TWCR_REG,TWCR_TWEN);
}

void TWI_voidInitSlave(uint8 Copy_address)
{
	/*Set the slave address*/
	TWAR_REG=Copy_address<<1;
	
	/*Enable ACKnowledge bit */
	Set_Bit(TWCR_REG,TWCR_TWEA);
	
	
	/*Enable TWI*/
	Set_Bit(TWCR_REG,TWCR_TWEN);
}

TWI_ErrStatus TWI_SendStartCondition()
{
	TWI_ErrStatus Local_Error=NoError;
	
	/*Send start condition*/
	Set_Bit(TWCR_REG,TWCR_TWSTA);
	
	/*to make any change or make any step we need to clear flag*/
	/*Clear the interrupt flag to start the previous operation*/
	Set_Bit(TWCR_REG,TWCR_TWINT);
	
	/*wait until the operation finishes and the flag is raised*/
	while((Get_Bit(TWCR_REG,TWCR_TWINT))==0);
	
	/*Check the operation status*/
	// we need to clear the last 3 bit (3 LSB bits) so we use 0xF8
	if((TWSR_REG & 0xF8)!=START_ACK)
	{
		Local_Error=StartconditionERR;
	}
	else
	{
		//do nothing
	}
	return Local_Error;

}

TWI_ErrStatus TWI_SendRepeatedStart()
{
	TWI_ErrStatus Local_Error = NoError;
	
	/* enable start condition*/
	Set_Bit(TWCR_REG,TWCR_TWSTA);
	
	/*clear the interrupt flag*/
	Set_Bit(TWCR_REG,TWCR_TWINT);
	
	while((Get_Bit(TWCR_REG,TWCR_TWINT))==0);
	
	/*Check the operation status*/
	// we need to clear the last 3 bit (3 LSB bits) so we use 0xF8
	if((TWSR_REG & 0xF8)!= REP_START_ACK)
	{
		Local_Error=RepeatedStartError;
	}
	else
	{
		// do nothing
	}
	return Local_Error;
}

TWI_ErrStatus TWI_SendSlaveAddressWithWrite(uint8 copy_Slaveaddress)
{
	TWI_ErrStatus Local_Error=NoError;
	
	/*set the address slave */
	TWDR_REG=copy_Slaveaddress<<1;
	
	/* Enable the write bit*/
	Clear_Bit(TWDR_REG,0);
	
	/*we must clear start condition by software after send the start condition */
	Clear_Bit(TWCR_REG,TWCR_TWSTA);
	
	/*clear the interrupt flag*/
	Set_Bit(TWCR_REG,TWCR_TWINT);
	
	/*wait until the flag rising up*/
	while((Get_Bit(TWCR_REG,TWCR_TWINT))==0);
	
	/* check the condition */
	if((TWSR_REG & 0xF8)!=Slave_ADD_AND_WR_ACK)
	{
		Local_Error=SlaveAddressWithWriteErr;
	}
	else
	{
		//do nothing 
	}
	return Local_Error;
}

TWI_ErrStatus TWI_SendSlaveAddressWithRead(uint8 copy_Slaveaddress)
{
	TWI_ErrStatus Local_Error=NoError;
	
	/*send the 7bit slave address to the bus*/
	TWDR_REG=copy_Slaveaddress<<1;
	
	/*set the read request in the LSB in the data register*/
	Set_Bit(TWDR_REG,0);
	
	/*we must clear start condition by software after send the start condition */
	Clear_Bit(TWCR_REG,TWCR_TWSTA);
	
	/*to make any change or make any step we need to clear flag*/
	Set_Bit(TWCR_REG,TWCR_TWINT);
	
	/*wait until the operation finishes and the flag is raised*/
	while((Get_Bit(TWCR_REG,TWCR_TWINT))==0);
	
	if((TWSR_REG & 0xF8)!=Slave_ADD_AND_RD_ACK)
	{
		Local_Error= SlaveAddresswithReadErr;
	}
	else
	{
		//do nothing
	}
	return Local_Error;
}

TWI_ErrStatus TWI_MasterWriteDataByte(uint8 copy_DataByte)
{
	TWI_ErrStatus Local_Error = NoError;
	
	/*send the data to the bus */ 
	TWDR_REG = copy_DataByte ;
	
	/* clear the interrupt flag*/
	Set_Bit(TWCR_REG,TWCR_TWINT);
	
	/*wait until the operation finishes and the flag is raised*/
	while((Get_Bit(TWCR_REG,TWCR_TWINT))==0);

	/*Check the operation status*/
	if((TWSR_REG & 0xF8) !=MSTR_WR_BYTE_ACK  )
	{
		Local_Error = MasterWriteByteErr ;
	}
	else
	{
		/*Do nothing*/
	}
	return Local_Error;
}

TWI_ErrStatus TWI_MasterReadDataByte(uint8 *copy_DataByte)
{
	TWI_ErrStatus Local_Error = NoError;
	
	/*we need to imagine the send scenario the master send start 
	 * then address with read and ack then the flag will rise so 
	 * to the address send the data we need to clear the flag first 
	 * to get the CLK work again and the slave can send the data */
	Set_Bit(TWCR_REG,TWCR_TWINT);
	
	/*wait until the operation finishes and the flag is raised*/
	while((Get_Bit(TWCR_REG,TWCR_TWINT))==0);
	
	/*Check the operation status*/
	if((TWSR_REG & 0xF8)!=MSTR_RRD_BYTE_WITH_ACK)
	{
		Local_Error=MasterReadByteErr;
	}
	else
	{
		*copy_DataByte = TWDR_REG;
	}
	return Local_Error;
}

void TWI_SendStopCondition(void)
{
	/*Enable the stop condition*/
	Set_Bit(TWCR_REG,TWCR_TWSTO);
	/*Clear the interrupt flag to start the previous operation*/
	Set_Bit(TWCR_REG,TWCR_TWINT);
}