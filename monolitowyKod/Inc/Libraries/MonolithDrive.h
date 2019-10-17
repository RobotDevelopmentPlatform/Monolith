/*
 * MonolithDrive.h
 *
 *  Created on: 27.06.2019
 *      Author: Micha³ Kita
 */

#ifndef LIBRARIES_MONOLITHDRIVE_H_
#define LIBRARIES_MONOLITHDRIVE_H_

/* --------------------------------------- Includes --------------------------------------- */
#include "main.h"
#include <limits.h>
#include <stdlib.h>

/* --------------------------------------- Defines ---------------------------------------- */
#define FL_PORT GPIOE
#define FL_PIN GPIO_PIN_8
#define RL_PORT GPIOE
#define RL_PIN GPIO_PIN_10
#define FR_PORT GPIOE
#define FR_PIN GPIO_PIN_12
#define RR_PORT GPIOE
#define RR_PIN GPIO_PIN_15

#define MAZE_DIM 8

#define NORTH 0b1000
#define EAST 0b0100
#define SOUTH 0b0010
#define WEST 0b0001

#define ADC_SIGNAL_FLAG 1

#define ENCODER 1
#define DISTANCE_SENSOR 2

/* ---------------------------------------- Types ----------------------------------------- */

/* Single cell in maze structure */
typedef struct ff_cell{
	uint8_t walls:5;		// bits in order (from MSB to LSB) NESW
	uint8_t visited:1;		// 1 - visited, 0 - not visited
	uint8_t value;			// flood-fill value
}ff_cell_t;

/* Robot position in maze structure */
typedef struct position{
	uint8_t direction:5;	// bits in order (from MSB to LSB) NESW
	uint8_t x, y;			// Y higher values -> to NORTH, x higher values -> to EAST
} position_t;

typedef struct PID{
	int16_t P;
	int16_t I;
	int16_t D;
	double Kp;
	double Kd;
	double Ki;
	int16_t error;
	int16_t last_error;
	int16_t correction;
	int16_t set_point;
}PID_t;


/* ---------------------------------- Exported Functions ---------------------------------- */
/* INIT */
void initMonolithDrive(void);

void drive_straight(int16_t power);
void drive_hard_stop(void);

/* FREE-RTOS */
void floodFillTask(void const * argument);
void drive_straight_task(void const * argument);


#endif /* LIBRARIES_MONOLITHDRIVE_H_ */
