/*
 * MonolithDrive.c
 *
 *  Created on: 27.06.2019
 *      Author: Micha³ Kita
 */


/* --------------------------------------- Includes --------------------------------------- */
#include "Libraries/MonolithDrive.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
/* --------------------------------------- FREERTOS --------------------------------------- */
void driveForward(void const * argument);
osThreadId driveForwardTaskHandle;

osThreadDef(driveForwardTask, driveForward, osPriorityHigh, 0, 128);

osMessageQDef(runDistance, 1, float);	// run distance in cm
osMessageQId (runDistanceID);

/* --------------------------------------- Variables --------------------------------------- */
uint32_t encoderFL;
uint32_t encoderRL;
uint32_t encoderFR;
uint32_t encoderRR;

/* --------------------------------------- Functions --------------------------------------- */
void initMonolithDrive(void){
	MX_TIM1_Init();		// INIT TIMER1 with PWM for wheel motor drivers

	htim1.Instance->CCR1 = 0;		// FRONT LEFT wheel pulse duty - max = 999
	htim1.Instance->CCR2 = 0;		// REAR LEFT wheel pulse duty - max = 999
	htim1.Instance->CCR3 = 0;		// FRONT RIGHT wheel pulse duty - max = 999
	htim1.Instance->CCR4 = 0;		// REAR RIGHT wheel pulse duty - max = 999

	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);		// FRONT LEFT stop
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);		// REAR LEFT stop
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);		// FRONT RIGHT stop
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);		// REAR RIGHT stop

	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

	htim2.Instance->CNT = 0;
	htim3.Instance->CNT = 0;
	htim4.Instance->CNT = 0;
	htim5.Instance->CNT = 0;
}

void wheelDriveFrontLeft(int16_t power){
	if(power >= 0){
		HAL_GPIO_WritePin(FL_PORT, FL_PIN, 1);
		htim1.Instance->CCR1 = power;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	}else if(power < -1){
		HAL_GPIO_WritePin(FL_PORT, FL_PIN, 0);
		htim1.Instance->CCR1 = power*-1;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	}else{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	}
}

void wheelDriveRearLeft(int16_t power){
	if(power >= 0){
		HAL_GPIO_WritePin(RL_PORT, RL_PIN, 1);
		htim1.Instance->CCR2 = power;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	}else if(power < -1){
		HAL_GPIO_WritePin(RL_PORT, RL_PIN, 0);
		htim1.Instance->CCR2 = power*-1;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	}else{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	}
}

void wheelDriveFrontRight(int16_t power){
	if(power >= 0){
		HAL_GPIO_WritePin(FR_PORT, FR_PIN, 0);
		htim1.Instance->CCR3 = power;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	}else if(power < -1){
		HAL_GPIO_WritePin(FR_PORT, FR_PIN, 1);
		htim1.Instance->CCR3 = power*-1;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	}else{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
	}
}

void wheelDriveRearRight(int16_t power){
	if(power >= 0){
		HAL_GPIO_WritePin(RR_PORT, RR_PIN, 0);
		htim1.Instance->CCR4 = power;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	}else if(power < -1){
		HAL_GPIO_WritePin(RR_PORT, RR_PIN, 1);
		htim1.Instance->CCR4 = power*-1;
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	}else{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
	}
}

/* --------------------------------------- FREERTOS API --------------------------------------- */
void driveForward(void const * argument){
	initMonolithDrive();

	uint32_t encoderR, encoderL;
	int32_t encError, encLastError, encI, encD, power;
	float encKp = 0, encKd = 0, encKi = 0;

	osEvent distance;

	for(;;) {

//		distance = osMessageGet(runDistanceID, osWaitForever);
//		if(distance.status == osEventMessage){
//			distance.value.v;
			encLastError = encError;

			encoderFL = htim2.Instance->CNT;
			encoderRL = htim3.Instance->CNT;
			encoderFR = htim4.Instance->CNT;
			encoderRR = htim5.Instance->CNT;
			encoderR=(encoderFR+encoderRR)/2;
			encoderL=(encoderFL+encoderRL)/2;

			encError = encoderR-encoderL;	// >0 -> skreca w lewo, <0 -> skreca w prawo
			encI += encError;
			encD = encError - encLastError;
			power = encKp*encError + encKi*encI + encKd*encD;

//			wheelDriveFrontLeft(speed+(power)/2);
//			wheelDriveRearLeft(speed+(power)/2);
//			wheelDriveFrontRight(speed-(power)/2);
//			wheelDriveRearRight(speed-(power)/2);
//		}
		osDelay(1);
	}
}
