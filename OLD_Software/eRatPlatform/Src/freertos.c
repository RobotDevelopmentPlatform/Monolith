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
#include "usart.h"
#include "i2c.h"
#include <string.h>

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */

extern uint8_t user_button_1;

/* DRIVE BEGIN */
	osThreadId driveTestTaskHandle;

	extern int16_t encoderFL; //TIM2
	extern int16_t encoderRL; //TIM3
	extern int16_t encoderFR; //TIM4
	extern int16_t encoderRR; //TIM5
/* DRIVE END */

/* UART BEGIN */
	osThreadId uart2TaskHandle;
	osThreadId uart2TestTaskHandle;

	extern __IO ITStatus Uart2Ready;

	extern UART_Comm_State uart2_comm_state;
	extern uint8_t uart2_tx_buffer[];
	extern uint8_t uart2_rx_buffer[];
/* UART END */

/* I2C BEGIN */
	osThreadId i2c2TestTaskHandle;
	osThreadId i2c2TaskHandle;

	extern __IO ITStatus I2C2Ready;

	extern I2C_Comm_State i2c2_comm_state;
	extern uint8_t i2c2_tx_buffer[];
	extern uint8_t i2c2_rx_buffer[];
/* I2C END */

osThreadId menuTaskHandle;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void StartDriveTestTask(void const * argument);

void StartUart2Task(void const * argument);

void StartI2C2Task(void const * argument);

void StartUart2TestTask(void const * argument);

void StartI2C2TestTask(void const * argument);

void StartMenuTask(void const * argument);

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
  vTaskSuspend(defaultTaskHandle);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  if(PLATFORM_MODE == DRIVE_TEST){
	osThreadDef(driveTestTask, StartDriveTestTask, osPriorityNormal, 0, 128);
	driveTestTaskHandle = osThreadCreate(osThread(driveTestTask), NULL);
  }else if(PLATFORM_MODE == UART_SHIELD_TEST){
	osThreadDef(uart2TestTask, StartUart2TestTask, osPriorityNormal, 0, 128);
	uart2TestTaskHandle = osThreadCreate(osThread(uart2TestTask), NULL);
	uart2_comm_state = UART_START;
  }else if(PLATFORM_MODE == I2C_SHIELD_TEST){
	osThreadDef(i2c2TestTask, StartI2C2TestTask, osPriorityNormal, 0, 128);
	i2c2TestTaskHandle = osThreadCreate(osThread(i2c2TestTask), NULL);
	uart2_comm_state = I2C_START;
  }else if(PLATFORM_MODE == BASIC_DEBUG){
	  osThreadDef(menuTask, StartMenuTask, osPriorityNormal, 0, 128);
	  menuTaskHandle = osThreadCreate(osThread(menuTask), NULL);
  }


//  osThreadDef(uart2Task, StartUart2Task, osPriorityNormal, 0, 128);
//  uart2TaskHandle = osThreadCreate(osThread(uart2Task), NULL);
//  vTaskSuspend(uart2TaskHandle);
//
//  osThreadDef(i2c2Task, StartI2C2Task, osPriorityNormal, 0, 128);
//  i2c2TaskHandle = osThreadCreate(osThread(i2c2Task), NULL);
//  vTaskSuspend(i2c2TaskHandle);
//
//

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

			  uart2_comm_state = UART_START;

			  osDelay(100);
		  }else if(user_button_1 == -1 || user_button_1 == 4){
			  //Soft stop Test

			  WheelDriveFrontLeft_Run(-1);
			  WheelDriveRearLeft_Run(-1);
			  WheelDriveFrontRight_Run(-1);
			  WheelDriveRearRight_Run(-1);

			  uart2_comm_state = UART_STOP;

			  osDelay(100);
		  }

	    osDelay(1);
	  }
  /* USER CODE END StartDefaultTask */
}

void StartUart2Task(void const * argument){
	for(;;)
	{

		osDelay(100);
	}
}

void StartI2CTask(void const * argument){
	for(;;){

		osDelay(100);
	}
}

void StartUart2TestTask(void const * argument){
	uint8_t i=0;
	for(;;)
	{
		if(user_button_1 == 1){ //test receive
			uint8_t number = 5;
			sprintf((char*)uart2_tx_buffer, "\n\rTesting receive. \n\r\tEnter '%d' character word:\n\r\t", number);

			if(HAL_UART_Transmit_IT(&huart2, (uint8_t*)uart2_tx_buffer, strlen((char*)uart2_tx_buffer))!= HAL_OK)
			{
				Error_Handler();
			}

			while (Uart2Ready != SET)
			{
			}

			Uart2Ready = RESET;

			/* Put UART peripheral in reception process */
			if(HAL_UART_Receive_IT(&huart2, (uint8_t *)uart2_rx_buffer, number) != HAL_OK)
			{
				Error_Handler();
			}

			/*##-5- Wait for the end of the transfer ###################################*/
			while (Uart2Ready != SET)
			{
			}

			/* Reset transmission flag */
			Uart2Ready = RESET;

			sprintf((char*)uart2_tx_buffer, "\n\r\tYou have entered '%s'.\n\r", uart2_rx_buffer);

			if(HAL_UART_Transmit_IT(&huart2, (uint8_t*)uart2_tx_buffer, strlen((char*)uart2_tx_buffer))!= HAL_OK)
			{
				Error_Handler();
			}

			while (Uart2Ready != SET)
			{
			}

			Uart2Ready = RESET;

			user_button_1 = 2;
		}else if(user_button_1 == 2){ //test transmit
			if(i == 0){
				sprintf((char*)uart2_tx_buffer, "\rUART Transmit Test %s", "|");
				i++;
			}
			else if(i == 1){
				sprintf((char*)uart2_tx_buffer, "\rUART Transmit Test %s", "/");
				i++;
			}
			else if(i == 2){
				sprintf((char*)uart2_tx_buffer, "\rUART Transmit Test %s", "-");
				i++;
			}
			else if(i == 3){
				sprintf((char*)uart2_tx_buffer, "\rUART Transmit Test %s", "\\");
				i = 0;
			}

			if(HAL_UART_Transmit_IT(&huart2, (uint8_t*)uart2_tx_buffer, strlen((char*)uart2_tx_buffer))!= HAL_OK)
			{
				Error_Handler();
			}

			while (Uart2Ready != SET)
			{
			}

			Uart2Ready = RESET;
		} else if (user_button_1 != 0){
			sprintf((char*)uart2_tx_buffer, "\n\r");

			if(HAL_UART_Transmit_IT(&huart2, (uint8_t*)uart2_tx_buffer, strlen((char*)uart2_tx_buffer))!= HAL_OK)
			{
				Error_Handler();
			}

			while (Uart2Ready != SET)
			{
			}

			Uart2Ready = RESET;

			user_button_1 = 0;
		}

		osDelay(100);
	}
}

void StartI2C2TestTask(void const * argument){
	for(;;)
		{
			if(user_button_1 == 1){ //test receive
				sprintf((char*)i2c2_tx_buffer, "T2C Transmit Test '%d'", 200);
//				HAL_I2C_Slave_Transmit_DMA(&hi2c2, (uint8_t*)i2c2_tx_buffer, strlen((char*)i2c2_tx_buffer));
				HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t*)i2c2_tx_buffer, strlen((char*)i2c2_tx_buffer));


				//user_button_1 = 0;
			}else if(user_button_1 == 2){ //test transmit

				user_button_1 = 0;
			} else{

			}

			osDelay(100);
		}
}

void StartMenuTask(void const * argument){
	for(;;){

		osDelay(100);
	}
}



/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
