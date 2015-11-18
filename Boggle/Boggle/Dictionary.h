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
	TreeNode* next;
};

extern TreeNode* treeRoot;

void parseDictionaryFile(char8_t *filename, int32_t *numWords);

#undef extern