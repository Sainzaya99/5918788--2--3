#ifndef BTREE_H
#define BTREE_H

#define MAX_PATH 200

typedef struct Node {
	char data;
	struct Node* left;
	struct Node* right;
} Node;

typedef struct {
	Node* root; 
	int max_size;
	int count;
} BTree;

BTree *create_btree(int size);
int insert_root(BTree* tree, char value);
int insert_child(BTree* tree, Node* parent, char child, char value);
int delete_node(BTree* tree, Node* leaf);
int update_value(BTree* tree, Node* node, char value);
void read_child(BTree *tree, Node* parent);
void print_btree(BTree* tree);
void destroy_btree(BTree* btree);

#endif
