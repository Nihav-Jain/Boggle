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

char8_t gameBoard[NUM_ROWS][NUM_COLS];

/**
*	@method	resetGame
*	@desc	resets game state variables - 
*			calls function to reset dictionary state and frees the current list of found words
*/
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

/**
*	@method	resetDictionaryDFA
*	@desc	resets the isCounted of the DFA tree for the next round
*/
void resetDictionaryDFA(TreeNode* root)
{
	root->isCounted = false;

	if (root->child != NULL)
	{
		resetDictionaryDFA(root->child);
	}
	if (root->nextSibling != NULL)
	{
		resetDictionaryDFA(root->nextSibling);
	}
}

/**
*	@method	initGame
*	@desc	unused, consider removing
*/
void initGame()
{

}						

/**
*	@method	buildRandomBoard
*	@desc	generated the game board from randomly chosen faces of the given set of dice
*/
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
			randomDieNumber = (int16_t) rangedRandom(0, (NUM_ROWS * NUM_COLS));
			// resolving collision - if the generated number dice has already been set on the game board, add 1 until you find the next unused dice
			while (hasDieBeenChosen[randomDieNumber])
			{
				randomDieNumber = (randomDieNumber + 1) % (NUM_ROWS * NUM_COLS);
			}
			hasDieBeenChosen[randomDieNumber] = true;
			randomDieFace = (int16_t) rangedRandom(0, 6);
			gameBoard[i][j] = dice[randomDieNumber][randomDieFace];
		}
	}
}

/**
*	@method	printBoard
*	@desc	print the current game board
*/
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

/**
*	@method	printWords
*	@desc	print the list of words found in the current game board
*/
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

/**
*	@method	searchForWords
*	@desc	searches the game board for all possible words from the dictionary
*/
void searchForWords()
{
	int16_t i, j;
	bool8_t visited[NUM_ROWS][NUM_COLS];
	memset(visited, false, sizeof(visited[0][0]) * NUM_ROWS * NUM_COLS);

	// empty string to form the words searched from the game board
	char8_t* word = (char8_t *)malloc(MAX_CHARS_IN_DICTIONARY_WORD * sizeof(char8_t));
	strcpy(word, "");

	// for every element on the game board, call the function searchDFS()
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

/**
*	@method	searchDFS
*	@desc	runs a Depth First Search to find all words starting with the gameBoard[i][j]
*/
void searchDFS(bool8_t visited[NUM_ROWS][NUM_COLS], int16_t i, int16_t j, char* word, TreeNode* dfaRoot)
{
	visited[i][j] = true;

	// find the node containing current game board character
	dfaRoot = dfaRoot->child;
	while (dfaRoot != NULL)
	{
		if (dfaRoot->character == gameBoard[i][j])
		{
			break;
		}
		dfaRoot = dfaRoot->nextSibling;
	}
	// character is not a part of any word, abort
	if (dfaRoot == NULL)
	{
		visited[i][j] = false;
		return;
	}
	else
	{
		// add character to word
		int16_t len = (int16_t) strlen(word);
		word[len] = dfaRoot->character;

		// adding 'U' as well if the added character was 'Q'
		if (word[len] == 'Q')
		{
			word[len + 1] = 'U';
			len = len + 1;
		}

		word[len + 1] = 0x00;
	}

	// if the recently added character forms a complete word and has not been counted in the game yet
	// add word to word list
	if ((dfaRoot->isFinal) && (!dfaRoot->isCounted))
	{
		dfaRoot->isCounted = true;
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

	// iterting over the 8 possible directions from the given character, looking to form a new word
	int16_t a, b;
	for (a = i - 1; (a <= i + 1) && (a < NUM_ROWS); a++)
	{
		for (b = j - 1; (b <= j + 1) && (j < NUM_COLS); b++)
		{
			// a, b is positive and the cell has not been visited yet
			if (a >= 0 && b >= 0 && !visited[a][b])
			{
				searchDFS(visited, a, b, word, dfaRoot);
			}
		}
	}

	// remove the recently added characters from the word string to reset the state of the DFS
	int16_t len = (int16_t) strlen(word);
	if (len >= 2 && word[len - 1] == 'U' && word[len - 2] == 'Q')
		word[len - 2] = 0x00;
	else
		word[len - 1] = 0x00;
	visited[i][j] = false;
}

/**
*	@method	rangedRandom
*	@desc	geenrates a random number in the range [min, max) (excludes max)
*/
int32_t rangedRandom(int32_t min, int32_t max)
{
	return min + rand() % (max - min);
}