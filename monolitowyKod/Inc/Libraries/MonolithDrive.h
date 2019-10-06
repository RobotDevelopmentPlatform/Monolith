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

/* --------------------------------------- Defines ---------------------------------------- */
#define FL_PORT GPIOE
#define FL_PIN GPIO_PIN_8
#define RL_PORT GPIOE
#define RL_PIN GPIO_PIN_10
#define FR_PORT GPIOE
#define FR_PIN GPIO_PIN_12
#define RR_PORT GPIOE
#define RR_PIN GPIO_PIN_15

/* ---------------------------------- Exported Functions ---------------------------------- */
/* INIT */
void initMonolithDrive(void);


#endif /* LIBRARIES_MONOLITHDRIVE_H_ */
