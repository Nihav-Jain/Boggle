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


void initGame();
void buildRandomBoard();
int32_t rangedRandom(int32_t min, int32_t max);
void searchForWords();
void printBoard();
void printWords();
void resetGame();

#undef extern