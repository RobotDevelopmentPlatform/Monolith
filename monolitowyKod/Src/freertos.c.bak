/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum AdcSignal {ADC_ON, ADC_OFF};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* UART VARIABLES BEGIN */

/* UART VARIABLES END */

/* FREERTOS VARIABLES BEGIN */
/* THREADS */
osThreadId MenuTaskHandle;
osThreadId SensorsTaskHandle;

/* SEMAPHORES */
osSemaphoreId uartRxSemaphoreHandle;
osSemaphoreId uartTxSemaphoreHandle;
osSemaphoreId adcSemaphoreHandle;

/* MESSAGE QUEUES */

/* FREERTOS VARIABLES END */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void menu(void const * argument);
void sensors(void const * argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* UART */
  osSemaphoreDef(uartRxSemaphore);
  uartRxSemaphoreHandle = osSemaphoreCreate(osSemaphore(uartRxSemaphore), 1);
  osSemaphoreDef(uartTxSemaphore);
  uartTxSemaphoreHandle = osSemaphoreCreate(osSemaphore(uartTxSemaphore), 1);

  /* ADC */
  osSemaphoreDef(adcSemaphore);
  adcSemaphoreHandle = osSemaphoreCreate(osSemaphore(adcSemaphore), 1);
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  osThreadDef(menuTask, menu, osPriorityRealtime, 0, 128);
  MenuTaskHandle = osThreadCreate(osThread(menuTask), NULL);

  osThreadDef(sensorsTask, sensors, osPriorityHigh, 0, 128);
  SensorsTaskHandle = osThreadCreate(osThread(sensorsTask), NULL);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
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

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void menu(void const * argument)
{
	extern UART_HandleTypeDef huart3;
	uint8_t uartRxBuffer = 0;
	uint8_t uartRxFlag = 0;

	HAL_UART_Receive_DMA(&huart3, &uartRxBuffer, 1);
  for(;;) {

	  uartRxFlag = osSemaphoreWait(uartRxSemaphoreHandle, 1);
	  if(osOK == uartRxFlag){			// wait for complete UART receive, if received, then proceed

		  switch(uartRxBuffer){										// simple switch menu - possible values - from 0 to 255 (uint8_t min to max value)
		  	  case 0:		// hard stop
		  		  //HardFault_Handler();
				  break;
			  case 1:
				  osSignalSet(SensorsTaskHandle, ADC_ON);
				  break;
			  case 2:
				  osSignalSet(SensorsTaskHandle, ADC_OFF);
				  break;
			  /*
			   * case 3:
			   * case 4:
			   * .
			   * .
			   * .
			   */
			  default:
				  break;
		  }

		  HAL_UART_Receive_DMA(&huart3, &uartRxBuffer, 1);			// enable UART receive
	  }else if (osErrorOS == uartRxFlag){
		  // TODO When can't receive data from uart in time do something
	  }
    osDelay(1);
  }
}

void sensors(void const * argument){

	extern ADC_HandleTypeDef hadc1;
	uint16_t sensorTab[4] = {0};
	osEvent adcSignalFlag;

	for(;;) {
		adcSignalFlag = osSignalWait(0, osWaitForever);
			if(osEventSignal == adcSignalFlag.status){

				switch(adcSignalFlag.value.v){
				case ADC_ON:
					HAL_ADC_Start_DMA(&hadc1, (uint32_t*)sensorTab, 4);
					break;
				case ADC_OFF:
					HAL_ADC_Stop_DMA(&hadc1);
					break;
				default:
					break;
				}

			}
		osDelay(1);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
