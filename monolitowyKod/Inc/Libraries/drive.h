/*
 * drive.h
 *
 *  Created on: 17.10.2019
 *      Author: mkimk
 */

#ifndef LIBRARIES_DRIVE_H_
#define LIBRARIES_DRIVE_H_

/* ------------------ Includes ------------------- */
#include "main.h"
#include "tim.h"
#include "FreeRTOS.h"

#include <stdlib.h>

/* ------------------- Defines ------------------- */
// Motor drivers pins
#define FL_PORT GPIOE
#define FL_PIN GPIO_PIN_8
#define RL_PORT GPIOE
#define RL_PIN GPIO_PIN_10
#define FR_PORT GPIOE
#define FR_PIN GPIO_PIN_12
#define RR_PORT GPIOE
#define RR_PIN GPIO_PIN_15

#define MAX_POWER 500

/* --------------- Exported Functions --------------- */
void drive_FL(float power);
void drive_RL(float power);
void drive_FR(float power);
void drive_RR(float power);

#endif /* LIBRARIES_DRIVE_H_ */
