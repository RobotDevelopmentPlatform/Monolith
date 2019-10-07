/*
 * SharpProximitySensors.c
 *
 *  Created on: 21.06.2019
 *      Author: Micha³ Kita
 */

/* --------------------------------------- Includes --------------------------------------- */
#include "Libraries/SharpProximitySensors.h"
#include "adc.h"
#include "math.h"


/* --------------------------------------- Variables --------------------------------------- */
// GP2Y0A51SK
static double GP2Y0A51SK_a = 5.0943;
static double GP2Y0A51SK_pow = -1.168;

// GP2Y0A41SK
static double GP2Y0A41SK_a = 12.211;
static double GP2Y0A41SK_pow = -1.086;

/* --------------------------------------- Functions --------------------------------------- */

/*
 * 	@brief	Calculate distance measured by sensor. Equation: distance = a*(Voltage^b), Voltage = ADC*V_REF/(2^12 - 1)
 * 	@param	ADC_value - Value from ADC
 * 	@param	a - constant factor
 * 	@param	b - constant power (pow)
 * 	@retval	Distance in centimeters truncated to 1 decimal point
 */
double adcToCm(uint16_t ADC_value, double a, double b){
	return (double) (trunc(10 * a * pow((ADC_value * V_REF/((1<<12) - 1)), b)))/10.0;
}
