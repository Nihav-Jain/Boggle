#ifdef DICTIONARY_CPP
#define extern
#endif

#define TREE_ROOT_CHAR '-'

struct _treenode;
typedef struct _treenode TreeNode;
typedef TreeNode* ChildList;
struct _treenode {
	char character;
	TreeNode *child;
	bool isFinal;
	bool isCounted;
	TreeNode* next;
};

extern TreeNode* treeRoot;

void parseDictionaryFile(char8_t *filename, int32_t *numWords);
void printDictionaryDFS(TreeNode *root);
void freeDictionary(TreeNode *root);

#undef extern