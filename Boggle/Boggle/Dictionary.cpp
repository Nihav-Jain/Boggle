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
	treeRoot->next = NULL;

	while (fscanf(dictionaryFile, "%s", inputWord) != EOF)
	{
		(*numWords)++;
		printf("%s\n", inputWord);
	}

	fclose(dictionaryFile);
}

