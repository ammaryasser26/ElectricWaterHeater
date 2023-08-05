/*
 * ICU.c
 *
 * Created: 5/12/2023 8:31:54 PM
 *  Author: Ahmed Nour
 */ 

#include "ICU.h"

static void (*pf)(void)=NULL;

void ICU_INIT()
{
	// select no prescaler
	Set_Bit(TCCR1B_REG,1); 
	// enable the interrupt
	Set_Bit(TIMSK_REG,5);
	//select the raising first 
	Set_Bit(TCCR1B_REG,6);
}

void ICU_set_EdgeType(uint8 Edge_Type)
{
	switch(Edge_Type)
	{
		case raising : Set_Bit(TCCR1B_REG,6); break;
		case falling : Clear_Bit(TCCR1B_REG,6); break;
	}
	
}

uint16 ICU_get_inputCap()
{
	uint16 result;
	result=ICR1_REG;
	return result;
}

void ICU_clear_timer()
{
	ICR1_REG=0;
	
}


void set_CallBackFun_ICU(void(*func)(void))
{
	if(func!=NULL)
	{
		pf=func;
	}
	
}

ISR(TIMER1_CAPT_vect)
{
	if(pf!=NULL)
	{
		pf();
		Set_Bit(TIFR1_REG,5);
	}
}

