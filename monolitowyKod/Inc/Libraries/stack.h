/*
 * stack.h
 *
 *  Created on: 10.10.2019
 *      Author: Micha³ Kita
 */

#ifndef LIBRARIES_STACK_H_
#define LIBRARIES_STACK_H_

/* --------------------------------------- Includes --------------------------------------- */
#include "main.h"
#include "Libraries/MonolithDrive.h"

/* --------------------------------------- Defines ---------------------------------------- */

/* ---------------------------------------- Types ----------------------------------------- */
typedef struct ff_Stack {
	int top;
	unsigned capacity;
	ff_cell_t** array;
}ff_Stack_t;

extern struct position;

/* ---------------------------------- Exported Functions ---------------------------------- */
struct ff_Stack* create_ff_Stack(unsigned capacity);
int isFull(struct ff_Stack* ff_stack);
int isEmpty(struct ff_Stack* ff_stack);
void push(struct ff_Stack* ff_stack, ff_cell_t* item);
ff_cell_t* pull(struct ff_Stack* ff_stack);
ff_cell_t* peek(struct ff_Stack* ff_stack);
uint8_t minOnStack(struct ff_Stack* ff_stack);
void push_all_neighbours(ff_Stack_t * ff_stack, ff_cell_t ff_maze[MAZE_DIM][MAZE_DIM], position_t * mouse);

#endif /* LIBRARIES_STACK_H_ */
