/*
 * SharpProximitySensors.h
 *
 *  Created on: 21.06.2019
 *      Author: Micha³ Kita
 */
#ifndef LIBRARIES_SHARPPROXIMITYSENSORS_H_
#define LIBRARIES_SHARPPROXIMITYSENSORS_H_

/* --------------------------------------- Includes ---------------------------------------- */
#include "main.h"

/* ---------------------------------------- Defines ---------------------------------------- */
#define V_REF 3.3


/* --------------------------------------- Functions --------------------------------------- */
uint16_t front_sensor_distance(uint16_t ADC_value);
uint16_t side_sensor_distance(uint16_t ADC_value);

#endif /* LIBRARIES_SHARPPROXIMITYSENSORS_H_ */
