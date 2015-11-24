#define DICTIONARY_CPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "Dictionary.h"
#include "Boggle.h"
#include "game.h"


/**
*	@method	parseDictionaryFile
*	@desc	reads the dictionary file, and adds the valid words to a Deterministic Finite Automata (DFA),
*			removes invalid words (words with length less than 4 or containing special symbols)
*/
void parseDictionaryFile(char8_t *filename, int32_t *numWords)
{
	FILE* dictionaryFile;
	fopen_s(&dictionaryFile, filename, "r");
	
	*numWords = 0;
	if (dictionaryFile == NULL)
	{
		printf("Invalid path %s\n", filename);
		return;
	}

	char8_t *inputWord = (char8_t *)malloc(MAX_CHARS_IN_DICTIONARY_WORD * sizeof(char8_t));

	// initializing the root of the DFA
	treeRoot = (TreeNode *)malloc(sizeof(TreeNode));
	initializeTreeNode(treeRoot, TREE_ROOT_CHAR);

	TreeNode* nextTreeNode;
	TreeNode* parentRef;

	int16_t i, j, len;
	while (fscanf(dictionaryFile, "%s", inputWord) != EOF)
	{
		// length validation
		len = (int16_t)strlen(inputWord);
		if (len < MIN_WORD_LEN)
		{
			continue;
		}

		// strip out any 'U' which appears right after a 'Q' since the dice face 'Q' is considered a 'QU'
		for (i = 0; i < len - 1; i++)
		{
			if (inputWord[i] == 'Q' && inputWord[i + 1] == 'U')
			{
				for (j = i + 1; j < len; j++)
				{
					inputWord[j] = inputWord[j + 1];
				}
				len--;
			}
		}

		(*numWords)++;
		
		nextTreeNode = treeRoot->child;
		parentRef = treeRoot;
		for (i = 0; i < len; i++)
		{
			// if this is the first child of its parent
			if (nextTreeNode == NULL)
			{
				nextTreeNode = (TreeNode *)malloc(sizeof(TreeNode));
				initializeTreeNode(nextTreeNode, inputWord[i]);
				parentRef->child = nextTreeNode;

			}
			else
			{
				// find the sibling node which has the same character as inputWord[i]
				while (nextTreeNode->character != inputWord[i])
				{
					if (nextTreeNode->nextSibling == NULL)
						break;
					nextTreeNode = nextTreeNode->nextSibling;
				}

				// if the character inputWord[i] already exists in the tree at the current level then proceed to next character in the word
				// else add a new sibling with inputWord[i] as its character
				if (nextTreeNode->character != inputWord[i])
				{
					nextTreeNode->nextSibling = (TreeNode *)malloc(sizeof(TreeNode));
					nextTreeNode = nextTreeNode->nextSibling;
					initializeTreeNode(nextTreeNode, inputWord[i]);
				}
			}

			// this node will now become the parent since the next character to be encountered will go in the next level of the tree
			parentRef = nextTreeNode;
			nextTreeNode = nextTreeNode->child;
			
			// if this is the last character then mark this node as the final node
			if (i == len - 1)
			{
				parentRef->isFinal = true;
			}
		}
	}

	free(inputWord);
	fclose(dictionaryFile);
}

/**
*	@method	initializeTreeNode
*	@desc	initializes this given tree node with the given character
*/
void initializeTreeNode(TreeNode *treeNode, char8_t character)
{
	treeNode->character = character;
	treeNode->child = NULL;
	treeNode->isFinal = false;
	treeNode->isCounted = false;
	treeNode->nextSibling = NULL;
}

/**
*	@method	printDictionaryDFS
*	@desc	prints the DFA tree in Pre order traversal
*/
void printDictionaryDFS(TreeNode *root)
{
	printf("%c", root->character);

	TreeNode* childList = root->child;
	while (childList != NULL)
	{
		printDictionaryDFS(childList);
		childList = childList->nextSibling;
	}
}

/**
*	@method	freeDictionary
*	@desc	releases the heap memory occupied by the DFA tree
*/
void freeDictionary(TreeNode *root)
{
	if (root->child != NULL)
	{
		freeDictionary(root->child);
	}
	if (root->nextSibling != NULL)
	{
		freeDictionary(root->nextSibling);
	}

	free(root);
}
