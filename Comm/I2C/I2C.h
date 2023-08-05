/*
 * I2C.h
 *
 * Created: 3/29/2023 5:41:09 AM
 *  Author: Ahmed Nour
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "../../utilities/BIT_MATH.h"
#include "../../utilities/Data_Type.h"
#include "../../utilities/DIO_REG.h"
#include "../../utilities/Error_State.h"
#include "I2C_Config.h"
typedef enum
{
	NoError,
	StartconditionERR,
	RepeatedStartError,
	SlaveAddressWithWriteErr,
	SlaveAddresswithReadErr,
	MasterWriteByteErr,
	MasterReadByteErr,
	}TWI_ErrStatus;

/*set master address to 0 if master will not be addressed*/
void TWI_voidInitMaster(uint8 Copy_Address);

void TWI_voidInitSlave(uint8 Copy_address);

TWI_ErrStatus TWI_SendStartCondition();

TWI_ErrStatus TWI_SendRepeatedStart();

TWI_ErrStatus TWI_SendSlaveAddressWithWrite(uint8 copy_Slaveaddress);

TWI_ErrStatus TWI_SendSlaveAddressWithRead(uint8 copy_Slaveaddress);

TWI_ErrStatus TWI_MasterWriteDataByte(uint8 copy_DataByte);

TWI_ErrStatus TWI_MasterReadDataByte(uint8 *copy_DataByte);

void TWI_SendStopCondition(void);

TWI_ErrStatus TWI_SendSlaveAddressWithRead(uint8 copy_Slaveaddress);



#endif /* I2C_H_ */