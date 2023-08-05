
/*
 * led.c
 *
 * Created: 1/25/2023 9:47:02 PM
 *  Author: Ahmed Nour
 */ 
#include "../../MCAL/DIO/DIO.h"
#include "led.h"
#include <avr/delay.h>

void led_init(uint8 pin_id , uint8 port_id)
{
	DIO_set_pin_direction(port_id,pin_id,output);
}

void led_on(uint8 pin_id , uint8 port_id)
{
	DIO_set_pin_value(port_id,pin_id,HIGH);
}

void led_off(uint8 pin_id , uint8 port_id)
{
	DIO_set_pin_value(port_id,pin_id,LOW);
}

void led_blink(uint8 pin_id , uint8 port_id)
{
	led_on(pin_id,port_id);
	_delay_ms(500);
	led_off(pin_id,port_id);
	_delay_ms(500);
	
}