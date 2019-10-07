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

#include <limits.h>
#include <stdlib.h>

/* --------------------------------------- DEFINES ---------------------------------------- */
#define MAZE_DIM 8

#define NORTH 0b1000
#define EAST 0b0100
#define SOUTH 0b0010
#define WEST 0b0001

/* --------------------------------------- FREERTOS --------------------------------------- */

/* --------------------------------------- Variables --------------------------------------- */
/* ENCODERS */
uint32_t encoderFL;
uint32_t encoderRL;
uint32_t encoderFR;
uint32_t encoderRR;

/* DRIVE VARIABLES */
uint16_t maxSpeed = 0;

/* FLOOD_FILL */
typedef struct ff_cell{
	uint8_t walls:5;		// bits in order (from MSB to LSB) NESW
	uint8_t visited:1;		// 1 - visited, 0 - not visited
	uint8_t value;			// flood-fill value
}ff_cell_t;

ff_cell_t ff_maze[MAZE_DIM][MAZE_DIM];

/* MOUSE */
typedef struct position{
	uint8_t direction:5;	// bits in order (from MSB to LSB) NESW
	uint8_t x, y;			// Y higher values -> to NORTH, x higher values -> to EAST
} position_t;

position_t mouse = {NORTH, 0, 0};

/* --------------------------------------- Functions --------------------------------------- */

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

/* -------------- flood-fill stack -------------- */
// A structure to represent a stack
typedef struct ff_Stack {
	int top;
	unsigned capacity;
	ff_cell_t** array;
}ff_Stack_t;

// function to create a stack of given capacity. It initializes size of stack as 0
struct ff_Stack* create_ff_Stack(unsigned capacity)
{
	struct ff_Stack* ff_stack = (struct ff_Stack*)malloc(sizeof(struct ff_Stack));
	ff_stack->capacity = capacity;
	ff_stack->top = -1;
	ff_stack->array = (ff_cell_t**)malloc(ff_stack->capacity * sizeof(ff_cell_t*));
	return ff_stack;
}

// Stack is full when top is equal to the last index
int isFull(struct ff_Stack* ff_stack)
{
	return ff_stack->top == ff_stack->capacity - 1;
}

// Stack is empty when top is equal to -1
int isEmpty(struct ff_Stack* ff_stack)
{
	return ff_stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1
void push(struct ff_Stack* ff_stack, ff_cell_t* item)
{
	if (isFull(ff_stack))
		return;
	ff_stack->array[++ff_stack->top] = item;
}

// Function to remove an item from stack.  It decreases top by 1
ff_cell_t* pull(struct ff_Stack* ff_stack)
{
//		if (isEmpty(ff_stack))
//			return INT_MIN;
	return ff_stack->array[ff_stack->top--];
}

// Function to return the top from stack without removing it
ff_cell_t* peek(struct ff_Stack* ff_stack)
{
//		if (isEmpty(ff_stack))
//			return INT_MIN;
	return ff_stack->array[ff_stack->top];
}

uint8_t minOnStack(struct ff_Stack* ff_stack){
	uint8_t tmpMin = (pull(ff_stack)->value);

	while(!isEmpty(ff_stack)){
		if(tmpMin >= (peek(ff_stack)->value) && 0 != (peek(ff_stack)->value) ){
			tmpMin = (pull(ff_stack)->value);
		}else{
			pull(ff_stack);
		}
	}
	return tmpMin;
}

void push_all_neighbours(ff_Stack_t * ff_stack){


	if(mouse.x > 0){
		if((ff_maze[(mouse.x)-1][mouse.y]).value != 0)
			push(ff_stack, &ff_maze[(mouse.x)-1][mouse.y]);
	}

	if(mouse.y > 0){
		if(ff_maze[mouse.x][(mouse.y)-1].value != 0)
			push(ff_stack, &ff_maze[mouse.x][(mouse.y)-1]);
	}

	if(mouse.x < MAZE_DIM-1){
		if(ff_maze[(mouse.x)+1][mouse.y].value != 0)
			push(ff_stack, &ff_maze[(mouse.x)+1][mouse.y]);
	}

	if(mouse.y < MAZE_DIM-1){
		if(ff_maze[mouse.x][(mouse.y)+1].value != 0)
			push(ff_stack, &ff_maze[mouse.x][(mouse.y)+1]);
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
						push_all_neighbours(ff_stack);
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
