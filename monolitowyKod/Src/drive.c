/*
 * drive.c
 *
 *  Created on: 17.10.2019
 *      Author: mkimk
 */
/* --------------------- INCLUDES --------------------- */
#include "Libraries/drive.h"

/* ---------------- FUNCTION PROTOTYPES ---------------- */
// initialization
void init_FL_motor(int16_t power);
void init_RL_motor(int16_t power);
void init_FR_motor(int16_t power);
void init_RR_motor(int16_t power);

int16_t prc_to_int(float prc){
	if(abs(prc) > 100){
		_Error_Handler(__FILE__, __LINE__);
	}
	return prc*(MAX_POWER/1000)*10;
}

/* -------------------- FUNCTIONS -------------------- */
void drive_FL(float power){
	init_FL_motor(power);
	power = prc_to_int(power);
	htim1.Instance->CCR1 = abs(power);		// FRONT LEFT wheel pulse duty - max = 1000
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);		// FRONT LEFT start
}

void drive_RL(float power){
	init_RL_motor(power);
	power = prc_to_int(power);
	htim1.Instance->CCR2 = abs(power);		// REAR LEFT wheel pulse duty - max = 1000
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);		// FRONT RIGHT start
}

void drive_FR(float power){
	init_FR_motor(power);
	power = prc_to_int(power);
	htim1.Instance->CCR3 = abs(power);		// FRONT RIGHT wheel pulse duty - max = 1000
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);		// REAR LEFT start
}

void drive_RR(float power){
	init_RR_motor(power);
	power = prc_to_int(power);
	htim1.Instance->CCR4 = abs(power);		// REAR RIGHT wheel pulse duty - max = 1000
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);		// REAR RIGHT start
}

// initialization functions
void init_FL_motor(int16_t power){
	if(power >= 0){
		HAL_GPIO_WritePin(FL_PORT, FL_PIN, 1);
	}else{
		HAL_GPIO_WritePin(FL_PORT, FL_PIN, 0);
	}
}

void init_RL_motor(int16_t power){
	if(power >= 0){
		HAL_GPIO_WritePin(RL_PORT, RL_PIN, 1);
	}else{
		HAL_GPIO_WritePin(RL_PORT, RL_PIN, 0);
	}
}

void init_FR_motor(int16_t power){
	if(power >= 0){
		HAL_GPIO_WritePin(FR_PORT, FR_PIN, 0);
	}else{
		HAL_GPIO_WritePin(FR_PORT, FR_PIN, 1);
	}
}

void init_RR_motor(int16_t power){
	if(power >= 0){
		HAL_GPIO_WritePin(RR_PORT, RR_PIN, 0);
	}else{
		HAL_GPIO_WritePin(RR_PORT, RR_PIN, 1);
	}
}
