/*
 * stack.c
 *
 *  Created on: 10.10.2019
 *      Author: Micha³ Kita
 */

/* --------------------------------------- Includes --------------------------------------- */
#include "main.h"
#include "Libraries/stack.h"

/* --------------------------------------- Functions --------------------------------------- */
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

void push_all_neighbours(ff_Stack_t * ff_stack, ff_cell_t ff_maze[MAZE_DIM][MAZE_DIM], position_t * mouse){

	if(mouse->x > 0){
		if((ff_maze[(mouse->x)-1][mouse->y]).value != 0)
			push(ff_stack, &ff_maze[(mouse->x)-1][mouse->y]);
	}

	if(mouse->y > 0){
		if(ff_maze[mouse->x][(mouse->y)-1].value != 0)
			push(ff_stack, &ff_maze[mouse->x][(mouse->y)-1]);
	}

	if(mouse->x < MAZE_DIM-1){
		if(ff_maze[(mouse->x)+1][mouse->y].value != 0)
			push(ff_stack, &ff_maze[(mouse->x)+1][mouse->y]);
	}

	if(mouse->y < MAZE_DIM-1){
		if(ff_maze[mouse->x][(mouse->y)+1].value != 0)
			push(ff_stack, &ff_maze[mouse->x][(mouse->y)+1]);
	}
}
