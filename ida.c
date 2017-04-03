/*
 * ida.c
 *
 *  Created on: Oct 20, 2015
 *      Author: kavenp
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define ARRAY_SIZE 16
#define SIDE_LEN 4
 #define MAX_MOVES 4
#define INFINITY INT_MAX

typedef struct {
	int cost;
	int threshold;
	int *state;
	int *moves;
}node_t;

int heuristic(int state[]);
node_t* idaControlLoop(int state[]);
node_t* idaStar(node_t* node, int bound, int *tempBound);
void possibleMoves(int* state, int* moves);
void nextMoveState(int index, int* state, int* nextState);
int min(int val1, int val2);

static long generatedNodes = 0;
static long expandedNodes = 0;

int main(int argc, char* argv[]) {
	int h,i;

	node_t* result;
	int state[ARRAY_SIZE] = {5,0,2,3,1,4,6,7,8,9,10,11,12,13,14,15};
	h = heuristic(state);
	/*node_t* start;*/
	printf("h = %d\n", h);
	result = idaControlLoop(state);
	printf("moves = %d\n", result->cost);
	/*printf("final result\n");*/
	printf("\n");
	free(result);
	return 0;
}

/* Manhattan Distance Heuristic used */
int heuristic(int state[ARRAY_SIZE]) {
	int sum = 0;
	int i;
	int currX, currY, shouldX, shouldY;
	for(i = 0; i < ARRAY_SIZE; i++) {
		if(state[i] != 0) {
			currX = i % SIDE_LEN;
			currY = i / SIDE_LEN;
			shouldX = state[i] % SIDE_LEN;
			shouldY = state[i] / SIDE_LEN;
			sum += abs(currX - shouldX) + abs(currY - shouldY);
		}
	}
	return sum;
}

node_t
*idaControlLoop(int* state) {
	int bound;
	int i;
	int tempBound[1];
	node_t *initNode = malloc(sizeof(node_t)); 
	node_t *result = NULL;
	bound = heuristic(state);
	do {
		*tempBound = INFINITY;
		initNode->state = state;
		initNode->cost = 0;
		printf("bound: %d\n", bound);
		result = idaStar(initNode, bound, tempBound);
		if(result == NULL) {
			bound = *tempBound;
		}
	} while(result == NULL);
		free(initNode);
		return result;
}

node_t
*idaStar(node_t* node, int bound, int *tempBound) {
	int i;
	int moves[MAX_MOVES];
	int nextState[ARRAY_SIZE];
	node_t *tempNode = malloc(sizeof(node_t));
	node_t *result;
	possibleMoves(node->state,moves);
	/* creates all possible moves and places in moves list*/
	node->moves = moves;
	printf("generated = %ld\n", generatedNodes);
	printf("expanded = %ld\n", expandedNodes);
	printf("\nbound = %d\n", bound);
	for(i = 0; i < MAX_MOVES; i++) {
		generatedNodes++;
		printf("iiiiiiiiiiiii = %d\n", i);
		if(node->moves[i] == -1) {
			continue;
		}else{
			nextMoveState(node->moves[i], node->state, nextState);
			/* creates new state from move */
			tempNode->state = nextState;
			printf("\nnextState\n");
			for(i=0;i<ARRAY_SIZE;i++) {
				printf("%d ",tempNode->state[i]);
			}
			tempNode->cost = node->cost + 1;
			tempNode->threshold = tempNode->cost + heuristic(tempNode->state);
			printf("\nthreshold = %d\n", tempNode->threshold);
			if(tempNode->threshold > bound) {
				*tempBound = min(tempNode->threshold, *tempBound);
				printf("tempBound: %d\n", *tempBound);
			}else{
				expandedNodes++;
				if(heuristic(tempNode->state) == 0) {
					return tempNode;
				}
				result = idaStar(tempNode, bound, tempBound);
				if(result != NULL) {
					return result;
				}
			}
		}
	}
	return NULL;
}

int min(int val1, int val2) {
	if(val1 < val2) {
		return val1;
	}else{
		return val2;
	}
}

void possibleMoves(int* state, int* moves) {
	int i;
	int blankX, blankY;
	int movedX, movedY;
	int xMove[] = {1,0,-1,0};
	int yMove[] = {0,1,0,-1};
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		/* Find blank space x,y coordinate */
		if(state[i] == 0) {
			blankX = i % SIDE_LEN;
			blankY = i / SIDE_LEN;
			break;
		}
	}
	for(i = 0; i < MAX_MOVES; i++)
	{
		/* Try each move and store the x,y coordinates
		 * after each move */
		movedX = blankX + xMove[i];
		movedY = blankY + yMove[i];
		if((movedX < 0 || movedX > MAX_MOVES-1) || 
			(movedY < 0  || movedY > MAX_MOVES-1))
		{
			/* If move out of bounds, -1 in array means move is
			 * not valid */
			moves[i] = -1;
		}else{
			/* Otherwise add the index of the possible move 
			 * into the moveset array*/
			moves[i] = movedY * SIDE_LEN + movedX;
			/*printf("moves: %d\n", moves[i]);*/
		}
	}
}

void nextMoveState(int index, int* state, int* nextState) {
	int i;
	for(i = 0; i < ARRAY_SIZE; i++) {	
		if(state[i] == 0) {
			/* Swap the index value into blank index */
			nextState[i] = state[index];
		}else{
			/* Copy other values over */
			nextState[i] = state[i];
		}
	}
	/* Swap blank value into index value */
	nextState[index] = 0;
	/*printf("nextState\n");
	for(i = 0; i < ARRAY_SIZE; i++) {
		printf("%d ", nextState[i]);*/
	
}

