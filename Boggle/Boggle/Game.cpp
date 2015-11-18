
#define GAME_CPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "types.h"
#include "Boggle.h"
#include "Dictionary.h"
#include "dice.h"
#include "game.h"

char8_t gameBoard[NUM_ROWS][NUM_COLS];

void resetGame()
{

}

void initGame()
{

}						

void buildRandomBoard()
{
	int i, j;
	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLS; j++)
		{
			gameBoard[i][j] = (char8_t)rangedRandom('A', 'Z' + 1);
		}
	}
}

void printBoard()
{
#if DEBUG_PRINTING_ON
	int i, j;
	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLS; j++)
		{
			printf("%c", gameBoard[i][j]);
		}
		printf("\n");
	}
#endif
}

void printWords()
{
#if DEBUG_PRINTING_ON

#endif
}

void searchForWords()
{
	//just taking some time here to make sure the timer is working properly
	/*float j=0.0f;
	for(int i=0;i<100000;++i)
		j+=((float)rand())/100000.0f;

	printf("j=%f",j);*/


}

// this includes min but exclues max i.e. [min, max)
int32_t rangedRandom(int32_t min, int32_t max)
{
	return min + rand() % (max - min);
}