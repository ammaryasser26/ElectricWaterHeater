/*
 * AMIT.c
 *
 * Created: 2/17/2023 7:48:36 PM
 * Author : Ahmed Nour
 */ 

//#include <avr/io.h>
#include <util/delay.h>
#include "HAL/keypad/keypad.h"
#include "MCAL/DIO/DIO.h"
#include "MCAL/ADC/ADC.h"
#include "MCAL/Timer/Timer.h"
#include "MCAL/EXTI/Interrupt.h"
#include "Comm/Spi/SPI.h"
#include "RTOS/RTOS_interface.h"
#include "HAL/ICU/ICU.h"
#include "Comm/UART/UART.h"
#include "HAL/EEPROM/EEPROM.h"





#include 	"HAL/Button/Button.h"

#include	"HAL/LED/led.h"
#include 	"HAL/LCD/lcd.h"
#include 	"HAL/EEPROM/EEPROM.h"
#include	"mainfunc.h"
#include	"mainfuncConfig.h"

#define EEPROM_take_temp_location	103
#define EEPROM_ideal_temp_location	100

uint8 max_temp= 75 , min_temp= 35 , ideal_temp= 59 ,counter=30;
uint8 take_temp=0 , i=0,avr=0 , avr_temp=0;
uint8  current_temp =0;
uint16 Summation=0 ;
uint8 temp_EEPROM =0;
void temp_inter();
int main(void)
{
	

	// on off button
	Button_init(button_power, PORTD);
	// Heating led
	led_init(led_Heating, PORTD);
	// on_oFF led
	led_init(led_power, PORTD);
	// lcd init dir
	LCD_INIT_DIR();
	
	// lcd init the lcd
	LCD_INIT();

	TWI_voidInitMaster(0);
	//EEPROM send data with location 100
	EEPROM_Write(EEPROM_ideal_temp_location,ideal_temp);
	
	temp_EEPROM = EEPROM_Read(EEPROM_take_temp_location); // read the value that take from user
	// ADC init
	ADC_init();
	// temp sensor int
	// init EXTI0 for the button up temp
	EXT0_Interrupt_enable();
	// init EXTI1 for the button up temp
	EXT1_Interrupt_enable();
	// open Global Interrupt
	global_inti_Enable();
	//timer zero init
	Timer0_init();
	//timer zero interrupt normal mode for cal temp every 100 ms
	timer0_setCallBack(temp_inter);
	
	while(1)
	{
		// get the button state on -off
		if(Button_read(button_power, PORTC)==0)
		{ // if button is off

			//led heating off
			led_off(led_Heating, PORTD);
			// led power off
			led_off(led_power, PORTD);

		}

		// power on
		else if(Button_read(button_power, PORTC)==1)
		{
			// power led on
			led_on(led_power, PORTD);
			//lcd clear
			LCD_Clear();
			// send the temp value to lcd
			LCD_sendString("temp: ");
			// send the temp value to lcd
			LCD_VoidIntgerToString(current_temp);
	
			// if user didn't give him value the temp work at ideal temp
			if (take_temp==0 && (temp_EEPROM==0xFF))
			{
				// the value inside EEProm
				if (current_temp<=ideal_temp-5)  // current < 60-5
				{
					// open the heater ( i.e blinking)
					led_blink(led_Heating, PORTD);
				//	temp_cal(&current_temp);
				
					
				}
				else if (current_temp>ideal_temp+5)
				{
					// open the cooler ( i.e blinking)
					led_on(led_Heating, PORTD);
		//			temp_cal(&current_temp);
				
				}
				else if (current_temp==ideal_temp)
				{
					// the temp go to the right temp
					// NOTE : after reading 10 time and take the (avr) turn off or on
					// cal the avr temp if avr == current temp == the take temp
					for(i=0;i<10;i++)
					{
		//				temp_cal(&current_temp);
						Summation+=current_temp;  // 10 reading

					}
					avr= Summation/10;
					if(avr==take_temp)
					{
						led_off(led_Heating, PORTD);
						Summation=0;
						avr=0;
					}

				}

			}
			// if the button up or down pressed
			else if ( ! (temp_EEPROM==0xFF))   // ie counter == eeprom read
			{
				
				// show the value of the temp that be take from user
					LCD_ChangePOS_XY(0,1);
					LCD_sendString("temp: ");
					LCD_VoidIntgerToString(temp_EEPROM);

				// check the value of the temp that take from user ( i.e button up or down )
				if ((current_temp<(take_temp-5)))
				{
					// open the heater ( i.e blinking)
					led_blink(led_Heating, PORTD);
			//		temp_cal(&current_temp);
					
			
				}
				if (current_temp>take_temp+5)
				{
					// open the cooler ( i.e blinking)
					led_on(led_Heating, PORTD);

				}
				else if (current_temp==take_temp)
				{
					// the temp go to the right temp
					// NOTE : after reading 10 time and take the (avr) turn off or on
					// cal the avr temp if avr == current temp == the take temp
					for(i=0;i<10;i++)
					{
			//			temp_cal(&current_temp);
						Summation+=current_temp;
						
					}
					avr= Summation/10;
					if(avr==take_temp)
					{
						led_off(led_Heating, PORTD);
						Summation=0;
						avr=0;
					}
				}
			}
			
		}
	}
	return 0 ;
}




ISR (EXTI_INT_0)
{
	// count the button up pressed
	button_up(& counter,& max_temp);
		// take the value and put it to take temp var
		take_temp=counter;
		//  save the take temp of user in EEPROM
	EEPROM_Write(EEPROM_take_temp_location,take_temp);
	temp_EEPROM=EEPROM_Read(EEPROM_take_temp_location);
}

ISR (EXTI_INI_1)
{
	// count button down pressed
	button_down(& counter,& min_temp);
	// take the value and put it to take temp var
	take_temp=counter;
	//  save the take temp of user in EEPROM
	EEPROM_Write(EEPROM_take_temp_location,take_temp);
	temp_EEPROM=EEPROM_Read(EEPROM_take_temp_location);
}

// this function used to cal temp every 100 ms 
// i used normal mode timer 0  we cal the value for preload =44
// and the tick number =49 
void temp_inter()
{	
	// local counter to cal the the timer 100ms
	uint8 static local_counter=0;
	local_counter++;
	// if 100ms done cal the temp
	if(local_counter==49)
	{
		uint16 volatile reading , analog;
		// start conversion and save in reading
		reading= ADC_Start_Conversion(pin0);
		//analog = (uint32)(reading)*(5000)/(uint32)(256);
		// temp equation for the simulation 
		analog = (uint32)(reading)* (500.0 / 1023.0*500)/12;
		// the temp after cal
		current_temp=analog/10;
		//reset the counter for new 100 ms
		local_counter=0;
	}
}
