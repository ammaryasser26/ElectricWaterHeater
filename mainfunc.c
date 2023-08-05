/*
 * mainfunc.c
 *
 * Created: 6/1/2023 3:16:00 PM
 *  Author: Ahmed Nour
 */ 
#include <util/delay.h>
#include "utilities/BIT_MATH.h"
#include "utilities/Data_Type.h"
#include "utilities/DIO_REG.h"

#include "MCAL/DIO/DIO.h"
#include "MCAL/ADC/ADC.h"
#include "MCAL/Timer/Timer.h"
#include "MCAL/EXTI/Interrupt.h"
#include "Comm/Spi/SPI.h"
#include "RTOS/RTOS_interface.h"
#include "HAL/ICU/ICU.h"
#include "Comm/UART/UART.h"

#include 	"HAL/Button/Button.h"
#include	"HAL/LED/led.h"
#include 	"HAL/LCD/lcd.h"
#include 	"HAL/EEPROM/EEPROM.h"
#include	"mainfunc.h"
#include	"mainfuncConfig.h"

// this function to calculate the temp in the simulation for lm35
// i use the 8 bit for adc conversion 

void temp_cal(uint8 *copy_address_temp)
{	uint16 volatile reading , analog;
	//_delay_ms(100);
	reading= ADC_Start_Conversion(pin0);
	_delay_ms(100);
	//analog = (uint32)(reading)*(5000)/(uint32)(256); // the right equation from data sheet (*1000) for increase the sensitivity
	analog = (uint32)(reading)* (500.0 / 1023.0*500)/12;
	
	*copy_address_temp=analog/10;
}

// this function for count up the button from the user to cal the temp that take from user
void button_up(uint8 * counter,uint8 * max_temp)
{
		// inc by 5 per pressed
		*counter+=5;
		// if exceed the max temp return the counter to the max
		if(*counter>=*max_temp)
		{
			*counter =*max_temp;
		}
}

// this function for count down the button from the user to cal the temp that take from user
void button_down(uint8 * counter,uint8 * min_temp)
{
		//dec the counter by -5
		*counter-=5;
		// if less than the min temp ret
		if(*counter<=*min_temp)
		{
			*counter =*min_temp;
		}
}
