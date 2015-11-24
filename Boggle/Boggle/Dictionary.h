#ifdef DICTIONARY_CPP
#define extern
#endif

#define TREE_ROOT_CHAR '-'

struct _treenode;
typedef struct _treenode TreeNode;
struct _treenode {
	TreeNode *child;		// first child of this node (NULL if no child)
	TreeNode* nextSibling;	// next sibling of this node (NULL if no sibling)
	bool isFinal;			// true if the path to this node's character forms a valid word from the dictionary
	bool isCounted;			// set to true when word ending with this node's character is counted in the game board
	char character;
};

extern TreeNode* treeRoot;

void parseDictionaryFile(char8_t *filename, int32_t *numWords);
void initializeTreeNode(TreeNode *treeNode, char8_t character);
void printDictionaryDFS(TreeNode *root);
void freeDictionary(TreeNode *root);

#undef extern