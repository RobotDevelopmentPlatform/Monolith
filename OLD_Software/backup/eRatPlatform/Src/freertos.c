/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "tim.h"
#include "Interface.h"
#include "WheelDriveSystem.h"
#include "WheelDrive.h"
#include "spi.h"
#include <string.h>

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
osThreadId driveTestTaskHandle;

extern int8_t user_button_1;

extern int16_t encoderFL; //TIM2
extern int16_t encoderRL; //TIM3
extern int16_t encoderFR; //TIM4
extern int16_t encoderRR; //TIM5

extern SPI_HandleTypeDef hspi1;
char message[] = "hello";

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void StartDriveTestTask(void const * argument);

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  osThreadDef(driveTestTask, StartDriveTestTask, osPriorityNormal, 0, 128);
  driveTestTaskHandle = osThreadCreate(osThread(driveTestTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */
void StartDriveTestTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	for(;;)
	  {
		  encoderFL = -TIM2->CNT;
		  encoderRL = -TIM3->CNT;
		  encoderFR = TIM4->CNT;
		  encoderRR = TIM5->CNT;

		  if(user_button_1 == 1){
	//		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
	//		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);
	//		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	//		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);

			  //Forward driving Test
			  WheelDriveFrontLeft_Run(100);
			  WheelDriveRearLeft_Run(100);
			  WheelDriveFrontRight_Run(100);
			  WheelDriveRearRight_Run(100);

			  osDelay(100);
		  }else if(user_button_1 == 2){
			  //Reverse driving Test

			  WheelDriveFrontLeft_Run(-100);
			  WheelDriveRearLeft_Run(-100);
			  WheelDriveFrontRight_Run(-100);
			  WheelDriveRearRight_Run(-100);

			  osDelay(100);
		  }else if(user_button_1 == 3){
			  //Hard stop Test

			  WheelDriveFrontLeft_Run(0);
			  WheelDriveRearLeft_Run(0);
			  WheelDriveFrontRight_Run(0);
			  WheelDriveRearRight_Run(0);

			  osDelay(100);
		  }else if(user_button_1 == -1 || user_button_1 == 4){
			  //Soft stop Test

			  WheelDriveFrontLeft_Run(-1);
			  WheelDriveRearLeft_Run(-1);
			  WheelDriveFrontRight_Run(-1);
			  WheelDriveRearRight_Run(-1);

			  osDelay(100);
		  }

		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
		  HAL_SPI_Transmit(&hspi1, (uint8_t *)message, strlen(message), 1);
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
	    osDelay(10);
	  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
