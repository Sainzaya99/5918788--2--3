#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"

BTree* create_btree(int size) {
    BTree* tree = (BTree*)malloc(sizeof(BTree));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->max_size = size;
    tree->count = 0;
    return tree;
}

int insert_root(BTree* tree, char value) {
    if (!tree) return -1;
    if (tree->root != NULL) return -2;
    if (tree->count >= tree->max_size) return -3;

    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return -4;

    node->data = value;
    node->left = NULL;
    node->right = NULL;

    tree->root = node;
    tree->count++;
    return 0;
}

int insert_child(BTree* tree, Node* parent, char child, char value) {
    if (!tree || !parent) return -1;
    if (child != 'L' && child != 'R') return -2;

    if (parent->left && parent->right) return -3; 

    if (child == 'L') {
        if (parent->left) return -4;
        if (parent->right && parent->right->data == value) return -5;
    }
    else { /* 'R' */
        if (parent->right) return -4;
        if (parent->left && parent->left->data == value) return -5;
    }

    if (tree->count >= tree->max_size) return -6;

    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return -7;

    node->data = value;
    node->left = NULL;
    node->right = NULL;

    if (child == 'L') parent->left = node;
    else parent->right = node;

    tree->count++;
    return 0;
}

int delete_node(BTree* tree, Node* leaf) {
    if (!tree || !leaf) return -1;
    if (leaf->left || leaf->right) return -2; 

    if (leaf == tree->root) {
        free(leaf);
        tree->root = NULL;
        tree->count--;
        return 0;
    }

    Node** stack = (Node**)malloc(sizeof(Node*) * (tree->max_size + 1));
    if (!stack) return -4; 

    int top = 0;
    stack[top++] = tree->root;

    Node* parent = NULL;
    char which = '\0';

    while (top > 0) {
        Node* cur = stack[--top];
        if (cur->left == leaf) { parent = cur; which = 'L'; break; }
        if (cur->right == leaf) { parent = cur; which = 'R'; break; }
        if (cur->left) stack[top++] = cur->left;
        if (cur->right) stack[top++] = cur->right;
    }

    free(stack);

    if (!parent) return -3; 

    if (which == 'L') parent->left = NULL;
    else parent->right = NULL;

    free(leaf);
    tree->count--;
    return 0;
}


int update_value(BTree* tree, Node* node, char value) {
    if (!tree || !node) return -1;

    if (node != tree->root) {
        Node** stack = (Node**)malloc(sizeof(Node*) * (tree->max_size + 1));
        if (stack) {
            int top = 0;
            stack[top++] = tree->root;

            Node* parent = NULL;
            char which = '\0';

            while (top > 0) {
                Node* cur = stack[--top];
                if (cur->left == node) { parent = cur; which = 'L'; break; }
                if (cur->right == node) { parent = cur; which = 'R'; break; }
                if (cur->left) stack[top++] = cur->left;
                if (cur->right) stack[top++] = cur->right;
            }

            free(stack);

            if (parent) {
                Node* sibling = (which == 'L') ? parent->right : parent->left;
                if (sibling && sibling->data == value) return -2;
            }
        }
    }

    node->data = value;
    return 0;
}

void read_child(BTree* tree, Node* parent) {
    (void)tree; 
    if (!parent) return;

    if (!parent->left && !parent->right) {
        printf("No child nodes.\n");
        return;
    }

    int first = 1;
    if (parent->left) {
        printf("%c(L)", parent->left->data);
        first = 0;
    }
    if (parent->right) {
        if (!first) printf(", ");
        printf("%c(R)", parent->right->data);
    }
    printf("\n");
}


typedef struct {
    Node* node;
    int depth;
} PrintItem;

void print_btree(BTree* tree) {
    if (!tree || !tree->root) {
        printf("The tree is empty.\n");
        return;
    }

    PrintItem* stack = (PrintItem*)malloc(sizeof(PrintItem) * (tree->max_size + 1));
    if (!stack) return;

    int top = 0;
    stack[top].node = tree->root;
    stack[top].depth = 0;
    top++;

    while (top > 0) {
        top--;
        Node* node = stack[top].node;
        int depth = stack[top].depth;

        if (depth == 0) {
            printf("%c\n", node->data);
        }
        else {
            int i;
            for (i = 0; i < depth - 1; i++) printf("    ");
            printf("+---%c\n", node->data);
        }

        if (node->right) {
            stack[top].node = node->right;
            stack[top].depth = depth + 1;
            top++;
        }
        if (node->left) {
            stack[top].node = node->left;
            stack[top].depth = depth + 1;
            top++;
        }
    }

    free(stack);
}


void destroy_btree(BTree* tree) {
    if (!tree) return;

    if (tree->root) {
        Node** stack = (Node**)malloc(sizeof(Node*) * (tree->max_size + 1));
        if (stack) {
            int top = 0;
            stack[top++] = tree->root;

            while (top > 0) {
                Node* node = stack[--top];
                if (node->left) stack[top++] = node->left;
                if (node->right) stack[top++] = node->right;
                free(node);
            }

            free(stack);
        }
    }

    tree->root = NULL;
    free(tree);
}