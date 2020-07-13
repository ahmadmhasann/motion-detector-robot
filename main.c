/*
* main.c
*
* Created: 6/20/2020 1:42:10 AM
* Author : Hassan
*/
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "macros.h"
#include "types.h"
#include "dio.h"
#include "uart.h"
#include "ultrasonic_sensor.h"
#define PIR_PIN 0
#define BUZZER_PIN 1
#define ROBOT_PORT 0

/*Chars sent by NodeMCU*/
enum move {
	STOP = '0',
	FORWARD = '1',
	BACKWARD = '2',
	RIGHT = '3',
	LEFT = '4'
};

int main(void)
{
	u8 moveCommand = STOP;
	u16 distance = 0;
	uart_vid_init();
	dio_vid_set_port_direction(ROBOT_PORT, 0xff);
	dio_vid_set_port_value(ROBOT_PORT, 0x00);
	dio_vid_set_pin_direction(B, PIR_PIN, INPUT);
	dio_vid_set_pin_direction(B, BUZZER_PIN, OUTPUT);
	while(1) {
		
		/*check if PIR sensor detects motion while is robot is not moving*/
		if (dio_u8_read_pin_value(B, PIR_PIN) == 1 && moveCommand== STOP) {
			dio_vid_set_pin_value(B, BUZZER_PIN, HIGH);
		}
		
		/*check if PIR sensor detects motion while is robot is not moving but there is a barrier*/
		else if (dio_u8_read_pin_value(B, PIR_PIN) == 1 && moveCommand== FORWARD && distance<=10) {
			dio_vid_set_pin_value(B, BUZZER_PIN, HIGH);
		}
		else {
			dio_vid_set_pin_value(B, BUZZER_PIN, LOW);
		}
		
		/*Checking UART flag for new data from NodeMCU*/
		if (UCSRA & (1<<RXC)) {
			moveCommand = UDR;
		}
		
		/*getting the distance between the robot and front barrier*/
		if (moveCommand == FORWARD) {
			distance = ultrasonic_u16_get_distance();
		}

		/*Handle moving direction sent from NodeMCU*/
		switch (moveCommand) {
			case STOP:
			dio_vid_set_port_value(A, 0x00);
			break;
			case BACKWARD:
			dio_vid_set_port_value(A, 0x05);
			break;
			case FORWARD:
			if (distance>10)
			dio_vid_set_port_value(A, 0x0A);
			else
			dio_vid_set_port_value(A, 0x00);
			break;
			case RIGHT:
			dio_vid_set_port_value(A, 0x08);
			break;
			case LEFT:
			dio_vid_set_port_value(A, 0x02);
			break;
		}
	}
}

