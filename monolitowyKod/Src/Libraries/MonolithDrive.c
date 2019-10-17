/*
 * MonolithDrive.c
 *
 *  Created on: 27.06.2019
 *      Author: Micha³ Kita
 */


/* --------------------------------------- Includes --------------------------------------- */
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include "adc.h"

#include "Libraries/MonolithDrive.h"
#include "Libraries/SharpProximitySensors.h"
#include "Libraries/stack.h"


/* --------------------------------------- FREERTOS --------------------------------------- */
/* THREADS */
osThreadId drive_straight_task_handle = NULL;

/* Semaphores */
osSemaphoreId (adc_semaphore_id);

/* Messages queue */


/* --------------------------------------- Variables --------------------------------------- */
/* ENCODERS */

/* SENSORS */
uint16_t Sensors[4];		// 0, 1 - FRONT, 2 - RIGHT, 3- LEFT

uint32_t encoderFL;		// htim2
uint32_t encoderRL;		// htim3
uint32_t encoderFR;		// htim4
uint32_t encoderRR;		// htim5

/* DRIVE VARIABLES */
uint16_t maxSpeed = 300;

/* FLOOD_FILL */
ff_cell_t ff_maze[MAZE_DIM][MAZE_DIM];
position_t mouse = {NORTH, 0, 0};

/* --------------------------------------- Functions --------------------------------------- */

/* PID ALGORITHM */
/*
 * Simple PID algorithm based on PID structure
 */
void PID(PID_t * value, uint8_t sensor_type){
	if(sensor_type == ENCODER){
		value->error =((htim2.Instance->CNT + htim3.Instance->CNT)/2 - (htim4.Instance->CNT + htim5.Instance->CNT)/2); // left minus right
	}else if(sensor_type == DISTANCE_SENSOR){
		value->error = side_sensor_distance(Sensors[3]) - side_sensor_distance(Sensors[2]); // left minus right
	}
	value->I += value->error;
	value->D = value->error - value->last_error;
	value->correction = (value->Kp*value->error) + (value->Kd*value->D) + (value->Ki*value->I);
	value->last_error = value->error;
}

/* -------------- FLOOD-FILL ALGORITHM -------------- */
/* flood-fill init */
void ff_init(void){

// flood-fill most optimistic values (works only for mazes with even number MAZE_DIM)
	for(uint8_t x = 0; x < MAZE_DIM; x++){
		for(uint8_t y = 0; y < MAZE_DIM; y++){
			if(x < (MAZE_DIM/2) && y < (MAZE_DIM/2)){
				ff_maze[x][y].value = (MAZE_DIM - 2) - (x + y);
			}else if(x < (MAZE_DIM/2) && y >= (MAZE_DIM/2)){
				ff_maze[x][y].value = (MAZE_DIM - 2) - (x + (MAZE_DIM - 1) - y);
			}else if(x >= (MAZE_DIM/2) && y >= (MAZE_DIM/2)){
				ff_maze[x][y].value = (MAZE_DIM - 2) - ((MAZE_DIM - 1) - x + (MAZE_DIM - 1) - y);
			}else if(x >= (MAZE_DIM/2) && y < (MAZE_DIM/2)){
				ff_maze[x][y].value = (MAZE_DIM - 2) - ((MAZE_DIM - 1) - x + y);
			}
		}
	}

// all cells not visited
	for(uint8_t x = 0; x < MAZE_DIM; x++)
			for(uint8_t y = 0; y < MAZE_DIM; y++)
				ff_maze[x][y].visited = 0;

// set border walls for whole maze
	for(uint8_t x = 0; x < MAZE_DIM; x++){
		ff_maze[x][0].walls |= SOUTH;
		ff_maze[x][MAZE_DIM - 1].walls |= NORTH;
	}
	for(uint8_t y = 0; y < MAZE_DIM; y++){
			ff_maze[0][y].walls |= WEST;
			ff_maze[MAZE_DIM - 1][y].walls |= EAST;
	}

}

/* -------------- DRIVE -------------- */
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

/*
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
*/

void drive_straight_init(void const * power){
// SENSORS INIT
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)Sensors, 4);

	// start encoder compare match interrupts
	HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_4);
	HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_4);
	HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_4);
	HAL_TIM_OC_Start_IT(&htim5, TIM_CHANNEL_4);

	// start encoders
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

	// reset encoders value
	htim2.Instance->CNT = 0;	// FL encoder
	htim3.Instance->CNT = 0;	// RL encoder
	htim4.Instance->CNT = 0;	// FR encoder
	htim5.Instance->CNT = 0;	// RR encoder

	// set encoder compare match value
	htim2.Instance->CCR4 = 1000;	// FL encoder
	htim3.Instance->CCR4 = 1000;	// RL encoder
	htim4.Instance->CCR4 = 1000;	// FR encoder
	htim5.Instance->CCR4 = 1000;	// RR encoder

	// PWM motor driver direction select
	if(power >= 0){
		HAL_GPIO_WritePin(FL_PORT, FL_PIN, 1);
		HAL_GPIO_WritePin(RL_PORT, RL_PIN, 1);
		HAL_GPIO_WritePin(FR_PORT, FR_PIN, 0);
		HAL_GPIO_WritePin(RR_PORT, RR_PIN, 0);
	}else if(power < (void*)-1){
		HAL_GPIO_WritePin(FL_PORT, FL_PIN, 0);
		HAL_GPIO_WritePin(RL_PORT, RL_PIN, 0);
		HAL_GPIO_WritePin(FR_PORT, FR_PIN, 1);
		HAL_GPIO_WritePin(RR_PORT, RR_PIN, 1);

	}
}

void drive_straight(int16_t power){
	if(!drive_straight_task_handle){
		osThreadDef(drive_straight, drive_straight_task, osPriorityHigh, 0, 256);
		drive_straight_task_handle = osThreadCreate(osThread(drive_straight), power);
	}
}

void drive_hard_stop(void){
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);		// FRONT LEFT start
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);		// FRONT RIGHT start
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);		// REAR LEFT start
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);		// REAR RIGHT start

	if(drive_straight_task_handle){
		osThreadTerminate(drive_straight_task_handle);
		drive_straight_task_handle = NULL;
	}
}

void set_left_speed(uint16_t power){
	htim1.Instance->CCR1 = power;		// FRONT LEFT wheel pulse duty - max = 999
	htim1.Instance->CCR2 = power;		// REAR LEFT wheel pulse duty - max = 999
}

void set_right_speed(uint16_t power){
	htim1.Instance->CCR3 = power;		// FRONT LEFT wheel pulse duty - max = 999
	htim1.Instance->CCR4 = power;		// REAR LEFT wheel pulse duty - max = 999
}

/* --------------------------------------- FREERTOS API --------------------------------------- */
void floodFillTask(void const * argument){
	ff_init();			// init maze array, and fill it with most optimistic distance values

	ff_Stack_t * ff_stack = create_ff_Stack(MAZE_DIM*MAZE_DIM*2);		// create stack for modified flood-fill algorithm
	ff_Stack_t * ff_neighbours = create_ff_Stack(4);
	uint8_t neighboursCount = 0, minVal = 0;

	ff_cell_t* tmpCell;


	initMonolithDrive();

	for(;;){
		if(ff_maze[mouse.x][mouse.y].value != 0){							// is current cell a finish?
			push(ff_stack, &ff_maze[mouse.x][mouse.y]);						// begin flood-fill algorithm
//			updateWalls();  todo
			while(!isEmpty(ff_stack)){
				tmpCell = pull(ff_stack);

				for(uint8_t i = 0; i<4; i++){								// find open neighbours
					if(!(tmpCell->walls & (1<<i))){
						++neighboursCount;
						switch(1<<i){
							case NORTH:
								push(ff_neighbours, &ff_maze[mouse.x][mouse.y+1]);
								break;
							case EAST:
								push(ff_neighbours, &ff_maze[mouse.x+1][mouse.y]);
								break;
							case SOUTH:
								push(ff_neighbours, &ff_maze[mouse.x][mouse.y-1]);
								break;
							case WEST:
								push(ff_neighbours, &ff_maze[mouse.x-1][mouse.y]);
								break;
						}
					}
				}

				if(neighboursCount>1){										// if not dead end
					neighboursCount = 0;
					minVal = minOnStack(ff_neighbours);
					if(!((ff_maze[mouse.x][mouse.y].value - 1) == minVal)){
						ff_maze[mouse.x][mouse.y].value = minVal+1;
						push_all_neighbours(ff_stack, ff_maze, &mouse);
					}
				}else{
					neighboursCount = 0;
//					deadEND(); todo
				}
			}
//				move_to_next_cell(); todo
		}
		neighboursCount = 0;
		osDelay(1);
	}
}


/* DRIVE FORWARD TASK */
void drive_straight_task(void const * argument){

/* variables */
	PID_t encoder = {0,0,0,0,0,0,0,0,0,0};
	PID_t distance_sensor = {0,0,0,0,0,0,0,0,0,0};
	int16_t correction = 0, power;
	osEvent sig;

/* begin */
	drive_straight_init(argument);

	if(abs((int)argument) <= maxSpeed){
		power = abs((int)argument);
	}else{
		power = maxSpeed;
	}

	htim1.Instance->CCR1 = power;		// FRONT LEFT wheel pulse duty - max = 999
	htim1.Instance->CCR2 = power;		// REAR LEFT wheel pulse duty - max = 999
	htim1.Instance->CCR3 = power;		// FRONT RIGHT wheel pulse duty - max = 999
	htim1.Instance->CCR4 = power;		// REAR RIGHT wheel pulse duty - max = 999

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);		// FRONT LEFT start
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);		// FRONT RIGHT start
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);		// REAR LEFT start
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);		// REAR RIGHT start

	for(;;){
//		if(Sensors[0] > 2000 || Sensors[1] > 2000){
//			driveHardStop();
//		}
		sig = osSignalWait(ADC_SIGNAL_FLAG, 5);
		if(sig.status == osEventSignal){

			PID(&encoder, ENCODER);
			PID(&distance_sensor, DISTANCE_SENSOR);

			correction = distance_sensor.correction + encoder.correction;

			if(power + abs(correction) <= maxSpeed){
				set_left_speed(power - correction/2);
				set_right_speed(power + correction/2);
			}else{
				if(correction < 0){
					set_left_speed(maxSpeed);
					set_right_speed(power + correction);
				}else{
					set_left_speed(power - correction);
					set_right_speed(maxSpeed);
				}
			}
		}else{
			drive_hard_stop();
		}
		osDelay(1);
	}
}

/* --------------------------------------- INTERRUPTS --------------------------------------- */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
		osSignalSet(drive_straight_task_handle, ADC_SIGNAL_FLAG);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim2){
		if((htim->Instance->CR1 & TIM_CR1_DIR) == 0){
			encoderFL += 1000;
		}else{
			encoderFL -= 1000;
		}
	}else if(htim == &htim3){
		if((htim->Instance->CR1 & TIM_CR1_DIR) == 0){
			encoderRL += 1000;
		}else{
			encoderRL -= 1000;
		}
	}else if(htim == &htim4){
		if((htim->Instance->CR1 & TIM_CR1_DIR) == 0){
			encoderFR += 1000;
		}else{
			encoderFR -= 1000;
		}
	}else if(htim == &htim5){
		if((htim->Instance->CR1 & TIM_CR1_DIR) == 0){
			encoderRR += 1000;
		}else{
			encoderRR -= 1000;
		}
	}
	// clear CNT register (clear encoder)
	htim->Instance->CNT = 0;
}
