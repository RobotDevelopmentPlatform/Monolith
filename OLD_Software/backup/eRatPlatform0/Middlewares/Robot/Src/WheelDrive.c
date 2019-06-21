/*
 * WheelDrive.c
 *
 *  Created on: Nov 29, 2018
 *      Author: Oskar
 */

#include "WheelDrive.h"

#include "tim.h"

void WheelDriveFrontLeft_Run(int16_t power){
	TIM_OC_InitTypeDef sConfigOC;

	if(power >= 0){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);

		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = power;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

		HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);


	}else if(power < -1){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);

		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = power*-1;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

		HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	}else{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	}
}

void WheelDriveRearLeft_Run(int16_t power){
	TIM_OC_InitTypeDef sConfigOC;

	if(power >= 0){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);

		sConfigOC.OCMode = TIM_OCMODE_PWM2; //ToDo
		sConfigOC.Pulse = power;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);
	    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);


	}else if(power < -1){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 0);

		sConfigOC.OCMode = TIM_OCMODE_PWM2; //ToDo
		sConfigOC.Pulse = power*-1;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2);
	    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	}else{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	}
}

void WheelDriveFrontRight_Run(int16_t power){
	TIM_OC_InitTypeDef sConfigOC;

	if(power >= 0){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);

		sConfigOC.OCMode = TIM_OCMODE_PWM2; //ToDo
		sConfigOC.Pulse = power;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

		HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);


	}else if(power < -1){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 1);

		sConfigOC.OCMode = TIM_OCMODE_PWM2; //ToDo
		sConfigOC.Pulse = power*-1;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

		HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	}else{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
	}
}

void WheelDriveRearRight_Run(int16_t power){
	TIM_OC_InitTypeDef sConfigOC;

	if(power >= 0){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);

		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = power;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

		HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);


	}else if(power < -1){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 1);

		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = power*-1;
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

		HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	}else{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
	}
}
