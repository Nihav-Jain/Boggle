
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

void resetGame()
{

}
void initGame()
{

}						
void buildRandomBoard()
{

}

void printBoard()
{
#if DEBUG_PRINTING_ON

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
	float j=0.0f;
	for(int i=0;i<100000;++i)
		j+=((float)rand())/100000.0f;

	printf("j=%f",j);
}

int32_t rangedRandom(int32_t min, int32_t max)
{
	return 0;
}


