
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
	int16_t i, j;
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
	int16_t i, j;
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

	int16_t i, j, k;
	bool8_t visited[NUM_ROWS][NUM_COLS];
	memset(visited, false, sizeof(visited[0][0]) * NUM_ROWS * NUM_COLS);

	char8_t* word = (char8_t *)malloc(MAX_CHARS_IN_DICTIONARY_WORD * sizeof(char8_t));

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLS; j++)
		{
			searchDFS(visited, i, j, word);
		}
	}
}

void searchDFS(bool8_t visited[NUM_ROWS][NUM_COLS], int16_t i, int16_t j, char* word)
{

}

// this includes min but exclues max i.e. [min, max)
int32_t rangedRandom(int32_t min, int32_t max)
{
	return min + rand() % (max - min);
}