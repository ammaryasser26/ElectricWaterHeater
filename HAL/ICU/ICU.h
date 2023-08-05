/*
 * ICU.h
 *
 * Created: 5/12/2023 8:32:08 PM
 *  Author:Ahmed Nour
 */ 


#ifndef ICU_H_
#define ICU_H_
#include "../../utilities/BIT_MATH.h"
#include "../../utilities/Data_Type.h"
#include "../../utilities/DIO_REG.h"
#include "../../MCAL/Timer/Timer.h"

#define raising	1
#define falling	0

void ICU_INIT(); // config

void set_CallBackFun_ICU(void(*func)(void));

void ICU_set_EdgeType(uint8 Edge_Type);

uint16 ICU_get_inputCap();

void ICU_clear_timer();

#ifndef		NULL
#define		NULL	(void *)0
#endif



#endif /* ICU_H_ */