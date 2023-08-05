/*
 * led.h
 *
 * Created: 1/25/2023 9:47:13 PM
 *  Author: Ahmed Nour
 */ 


#ifndef LED_H_
#define LED_H_


#include "../../MCAL/DIO/dio.h"

void led_init(uint8 pin_id , uint8 port_id);
void led_on(uint8 pin_id , uint8 port_id);
void led_blink(uint8 pin_id , uint8 port_id);
void led_off(uint8 pin_id , uint8 port_id);



#endif /* LED_H_ */