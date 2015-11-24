
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

#if BIG_BOGGLE
char8_t* dice[NUM_ROWS * NUM_COLS] = { DIE1, DIE2, DIE3, DIE4, DIE5, DIE6, DIE7, DIE8, DIE9, DIE10, DIE11, DIE12, DIE13, DIE14, DIE15, DIE16, DIE17, DIE18, DIE19, DIE20, DIE21, DIE22, DIE23, DIE24, DIE25};
#else
char8_t* dice[NUM_ROWS * NUM_COLS] = { DIE1, DIE2, DIE3, DIE4, DIE5, DIE6, DIE7, DIE8, DIE9, DIE10, DIE11, DIE12, DIE13, DIE14, DIE15, DIE16};
#endif

char8_t gameBoard[NUM_ROWS][NUM_COLS]; // = { { 'E', 'W', 'R', 'X', 'X' }, { 'V', 'R', 'S', 'G', 'H' }, { 'A', 'W', 'D', 'L', 'H' }, { 'F', 'N', 'V', 'U', 'S' }, { 'Q', 'M', 'U', 'R', 'O' } };

void resetGame()
{
	// reset the isCounted of the DFA
	TreeNode* root = treeRoot;
	resetDictionaryDFA(root);

	// free the word list
	if (topOfWordList != NULL)
	{
		wordListT *wordPtrToFree = topOfWordList;
		wordListT *tempHolder;
		while (wordPtrToFree != NULL)
		{
			free(wordPtrToFree->word);
			tempHolder = wordPtrToFree;
			wordPtrToFree = wordPtrToFree->nextWord;
			free(tempHolder);
		}
		topOfWordList = NULL;
		bottomOfWordList = NULL;
	}
}

void resetDictionaryDFA(TreeNode* root)
{
	root->isCounted = false;

	if (root->child != NULL)
	{
		resetDictionaryDFA(root->child);
	}
	if (root->next != NULL)
	{
		resetDictionaryDFA(root->next);
	}
}

void initGame()
{

}						

void buildRandomBoard()
{
	int16_t i, j;
	int16_t randomDieNumber, randomDieFace;

	bool hasDieBeenChosen[NUM_ROWS * NUM_COLS];
	memset(hasDieBeenChosen, false, sizeof(hasDieBeenChosen[0]) * NUM_ROWS * NUM_COLS);

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLS; j++)
		{
			randomDieNumber = (int16_t) rangedRandom(0, (NUM_ROWS * NUM_COLS)-1);
			while (hasDieBeenChosen[randomDieNumber])
			{
				randomDieNumber = (randomDieNumber + 1) % (NUM_ROWS * NUM_COLS);
			}
			hasDieBeenChosen[randomDieNumber] = true;
			randomDieFace = (int16_t) rangedRandom(0, 5);
			//gameBoard[i][j] = (char8_t)rangedRandom('A', 'Z');
			gameBoard[i][j] = dice[randomDieNumber][randomDieFace];
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
	printf("\n");
#endif
}

void printWords()
{
#if DEBUG_PRINTING_ON
	int wordCount = 0;
	printf("\nWords found in game board = \n");
	wordListT *top = topOfWordList;
	while (top != NULL)
	{
		printf("%s\n", top->word);
		top = top->nextWord;
		wordCount++;
	}
	printf("Word count = %d\n", wordCount);
#endif
}

void searchForWords()
{
	//just taking some time here to make sure the timer is working properly
	/*float j=0.0f;
	for(int i=0;i<100000;++i)
		j+=((float)rand())/100000.0f;

	printf("j=%f",j);*/

	int16_t i, j;
	bool8_t visited[NUM_ROWS][NUM_COLS];
	memset(visited, false, sizeof(visited[0][0]) * NUM_ROWS * NUM_COLS);

	char8_t* word = (char8_t *)malloc(MAX_CHARS_IN_DICTIONARY_WORD * sizeof(char8_t));
	strcpy(word, "");

	for (i = 0; i < NUM_ROWS; i++)
	{
		for (j = 0; j < NUM_COLS; j++)
		{
			searchDFS(visited, i, j, word, treeRoot);
		}
	}
	free(word);
	
	// call reset
}

void searchDFS(bool8_t visited[NUM_ROWS][NUM_COLS], int16_t i, int16_t j, char* word, TreeNode* dfaRoot)
{
	visited[i][j] = true;

	/*int16_t initStringLen = strlen(word);
	if (initStringLen > 0 && word[initStringLen - 1] == 'Q')
	{
		word[initStringLen] = 'U';
		word[initStringLen + 1] = 0x00;
	}*/

	dfaRoot = dfaRoot->child;
	while (dfaRoot != NULL)
	{
		if (dfaRoot->character == gameBoard[i][j])
		{
			break;
		}
		dfaRoot = dfaRoot->next;
	}
	if (dfaRoot == NULL)
	{
		// character is not a part of any word, abort
		visited[i][j] = false;
		return;
	}
	else
	{
		// add character to word
		int16_t len = (int16_t) strlen(word);
		word[len] = dfaRoot->character;
		if (word[len] == 'Q')
		{
			word[len + 1] = 'U';
			len = len + 1;
		}
		word[len + 1] = 0x00;
	}

	if ((dfaRoot->isFinal) && (!dfaRoot->isCounted))
	{
		dfaRoot->isCounted = true;
		// add word to word list
		if (topOfWordList == NULL)
		{
			topOfWordList = (wordListT *)malloc(sizeof(wordListT));
			topOfWordList->nextWord = NULL;
			topOfWordList->word = (char8_t *)malloc(sizeof(char8_t)* (strlen(word) + 1));
			strcpy(topOfWordList->word, word);
			bottomOfWordList = topOfWordList;
		}
		else
		{
			wordListT *temp = (wordListT *)malloc(sizeof(wordListT));
			temp->nextWord = NULL;
			temp->word = (char8_t *)malloc(sizeof(char8_t)* (strlen(word) + 1));
			strcpy(temp->word, word);
			bottomOfWordList->nextWord = temp;
			bottomOfWordList = bottomOfWordList->nextWord;
		}
	}

	int16_t a, b;
	for (a = i - 1; a <= i + 1 && a < NUM_ROWS; a++)
	{
		for (b = j - 1; b <= j + 1 && j < NUM_COLS; b++)
		{
			if (a >= 0 && b >= 0 && !visited[a][b])
			{
				searchDFS(visited, a, b, word, dfaRoot);
			}
		}
	}

	int16_t len = (int16_t) strlen(word);
	if (len >= 2 && word[len - 1] == 'U' && word[len - 2] == 'Q')
		word[len - 2] = 0x00;
	else
		word[len - 1] = 0x00;
	visited[i][j] = false;
}

// this includes min but exclues max i.e. [min, max)
int32_t rangedRandom(int32_t min, int32_t max)
{
	return min + rand() % (max - min);
}