#ifdef GAME_CPP
#define extern 
#endif


struct wordList
{
	char8_t *word;
	struct wordList *nextWord;
};
typedef struct wordList wordListT;

extern wordListT *topOfWordList;
extern wordListT *bottomOfWordList;


void initGame();
void buildRandomBoard();
int32_t rangedRandom(int32_t min, int32_t max);
void searchForWords();
void searchDFS(bool8_t visited[NUM_ROWS][NUM_COLS], int16_t i, int16_t j, char* word, TreeNode* dfaRoot);
void printBoard();
void printWords();
void resetGame();
void resetDictionaryDFA(TreeNode* root);

#undef extern