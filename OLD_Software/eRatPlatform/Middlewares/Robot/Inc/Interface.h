/*
 * Interface.h
 *
 *  Created on: Nov 29, 2018
 *      Author: Oskar
 */

#ifndef ROBOT_INC_INTERFACE_H_
#define ROBOT_INC_INTERFACE_H_

enum modes{
	BASIC_DEBUG,

	I2C_SHIELD_DEBUG,
	SPI_SHIELD_DEBUG,
	UART_SHIELD_DEBUG,

	DRIVE_TEST,
	I2C_SHIELD_TEST,
	SPI_SHIELD_TEST,
	UART_SHIELD_TEST
};

uint8_t user_button_1;

typedef enum
{
	UART_START,
	UART_STOP,
	UART_RESET
} UART_Comm_State;

typedef enum
{
	I2C_START,
	I2C_STOP,
	I2C_RESET
} I2C_Comm_State;

UART_Comm_State uart2_comm_state;
uint8_t uart2_tx_buffer[100];
uint8_t uart2_rx_buffer[100];

I2C_Comm_State i2c2_comm_state;
uint8_t i2c2_tx_buffer[100];
uint8_t i2c2_rx_buffer[100];

#endif /* ROBOT_INC_INTERFACE_H_ */
