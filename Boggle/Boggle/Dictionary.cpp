#define DICTIONARY_CPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "Dictionary.h"
#include "Boggle.h"
#include "game.h"


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

	treeRoot = (TreeNode *)malloc(sizeof(TreeNode));
	treeRoot->character = TREE_ROOT_CHAR;
	treeRoot->isFinal = false;
	treeRoot->isCounted = false;
	treeRoot->child = NULL;
	treeRoot->next = NULL;

	TreeNode* nextTreeNode;
	TreeNode* parentRef;

	int16_t i, len;
	while (fscanf(dictionaryFile, "%s", inputWord) != EOF)
	{
		//printf("%s %d\n", inputWord, strlen(inputWord));
		if (strlen(inputWord) < MIN_WORD_LEN)
		{
			continue;
		}
		(*numWords)++;
		
		len = strlen(inputWord);
		nextTreeNode = treeRoot->child;
		parentRef = treeRoot;
		for (i = 0; i < len; i++)
		{
			//printf("%c", inputWord[i]);
			if (nextTreeNode == NULL)
			{
				nextTreeNode = (TreeNode *)malloc(sizeof(TreeNode));
				nextTreeNode->character = inputWord[i];
				nextTreeNode->child = NULL;
				nextTreeNode->isFinal = false;		// check later
				nextTreeNode->isCounted = false;
				nextTreeNode->next = NULL;
				parentRef->child = nextTreeNode;

				parentRef = nextTreeNode;
				nextTreeNode = nextTreeNode->child;
			}
			else
			{
				while (nextTreeNode->character != inputWord[i])
				{
					if (nextTreeNode->next == NULL)
						break;
					nextTreeNode = nextTreeNode->next;
				}
				if (nextTreeNode->character == inputWord[i])
				{
					parentRef = nextTreeNode;
					nextTreeNode = nextTreeNode->child;
				}
				else
				{
					nextTreeNode->next = (TreeNode *)malloc(sizeof(TreeNode));
					nextTreeNode = nextTreeNode->next;
					nextTreeNode->character = inputWord[i];
					nextTreeNode->child = NULL;
					nextTreeNode->isFinal = false;		// check later
					nextTreeNode->isCounted = false;
					nextTreeNode->next = NULL;

					parentRef = nextTreeNode;
					nextTreeNode = nextTreeNode->child;
				}
			}
			if (i == len - 1)
			{
				parentRef->isFinal = true;
			}
		}
		//printf("\n");
	}
	//printDictionaryDFS(treeRoot);
	//printf("\n");
	free(inputWord);
	fclose(dictionaryFile);
}

void printDictionaryDFS(TreeNode *root)
{
	printf("%c", root->character);

	TreeNode* childList = root->child;
	while (childList != NULL)
	{
		printDictionaryDFS(childList);
		childList = childList->next;
	}
}
