/*
 * WheelDrive.h
 *
 *  Created on: Nov 25, 2018
 *      Author: Oskar
 */

#ifndef ROBOT_WHEELDRIVE_H_
#define ROBOT_WHEELDRIVE_H_

#include "stm32f7xx_hal.h"

//power:
//		{0} 		- 	Hard Stop
//		{-1} 		- 	soft stop
//		(0 - X> 	- 	positive power is forward
//		<-X, -1) 	- 	negative power is reverse

void WheelDriveFrontLeft_Run(int16_t power);
void WheelDriveRearLeft_Run(int16_t power);
void WheelDriveFrontRight_Run(int16_t power);
void WheelDriveRearRight_Run(int16_t power);

#endif /* ROBOT_WHEELDRIVE_H_ */
