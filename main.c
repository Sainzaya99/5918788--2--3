#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "btree.h"

#define MAX_LINE 256
#define MAX_TOKENS 10
#define MAX_TREE_SIZE 1000

static int str_equal_ignore_case(const char* a, const char* b) {
    while (*a != '\0' && *b != '\0') {
        if (toupper((unsigned char)*a) != toupper((unsigned char)*b)) return 0;
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

static int match_cmd(const char* tok, const char* full, char short_c) {
    if (tok[0] != '\0' && tok[1] == '\0' && toupper((unsigned char)tok[0]) == short_c) return 1;
    if (str_equal_ignore_case(tok, full)) return 1;
    return 0;
}

static int is_upper_letter(const char* tok) {
    return tok[0] != '\0' && tok[1] == '\0' && isupper((unsigned char)tok[0]);
}

static Node* find_node(BTree* tree, const char* path) {
    if (!tree || !path || path[0] != '/') return NULL;
    if (tree->root == NULL) return NULL;

    char buf[MAX_PATH];
    strncpy(buf, path, MAX_PATH - 1);
    buf[MAX_PATH - 1] = '\0';

    char* token = strtok(buf, "/");
    if (token == NULL) return NULL; 

    if (token[1] != '\0' || token[0] != tree->root->data) return NULL;

    Node* cur = tree->root;

    token = strtok(NULL, "/");
    while (token != NULL) {
        if (token[0] == '\0' || token[1] != '\0') return NULL; 
        char c = token[0];

        if (cur->left && cur->left->data == c) {
            cur = cur->left;
        }
        else if (cur->right && cur->right->data == c) {
            cur = cur->right;
        }
        else {
            return NULL; 
        }

        token = strtok(NULL, "/");
    }

    return cur;
}

int main(void) {
    BTree* tree = create_btree(MAX_TREE_SIZE);
    if (!tree) {
        fprintf(stderr, "Failed to create tree\n");
        return 1;
    }

    char line[MAX_LINE];

    printf("=== Binary Tree Manipulation Program ===\n");
    printf("Commands: Insert(I), Delete(D), Update(U), Read(R), Print(P)\n\n");

    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;

        char* tokens[MAX_TOKENS];
        int ntok = 0;
        char* tok = strtok(line, " \t");
        while (tok && ntok < MAX_TOKENS) {
            tokens[ntok++] = tok;
            tok = strtok(NULL, " \t");
        }
        if (ntok == 0) continue;

        char* cmd = tokens[0];

        if (match_cmd(cmd, "Insert", 'I')) {
            if (ntok == 3) {
                if (strcmp(tokens[1], "/") != 0 || !is_upper_letter(tokens[2])) {
                    printf("Error: invalid Insert (root) format. Example: Insert / A\n");
                    continue;
                }
                int ret = insert_root(tree, tokens[2][0]);
                if (ret == 0)
                    printf("Created root node %c.\n", tokens[2][0]);
                else if (ret == -2)
                    printf("Error: a root node already exists.\n");
                else
                    printf("Error: could not create the root node.\n");
            }
            else if (ntok == 4) {
                char* path = tokens[1];
                char childc = (char)toupper((unsigned char)tokens[2][0]);

                if (!(tokens[2][1] == '\0' && (childc == 'L' || childc == 'R'))) {
                    printf("Error: the child argument must be L or R.\n");
                    continue;
                }
                if (!is_upper_letter(tokens[3])) {
                    printf("Error: new-data must be a single uppercase letter.\n");
                    continue;
                }

                Node* parent = find_node(tree, path);
                if (!parent) {
                    printf("Error: parent node %s not found.\n", path);
                    continue;
                }

                int ret = insert_child(tree, parent, childc, tokens[3][0]);
                if (ret == 0)
                    printf("Added node %c as the %c child of %s.\n", tokens[3][0], childc, path);
                else if (ret == -3)
                    printf("Error: the parent node already has 2 children.\n");
                else if (ret == -4)
                    printf("Error: that position already has a child node.\n");
                else if (ret == -5)
                    printf("Error: cannot have the same data as the sibling node.\n");
                else
                    printf("Error: could not add the node.\n");
            }
            else {
                printf("Error: wrong number of arguments for Insert.\n");
            }
        }

        else if (match_cmd(cmd, "Delete", 'D')) {
            if (ntok != 2) {
                printf("Error: wrong number of arguments for Delete.\n");
                continue;
            }
            Node* node = find_node(tree, tokens[1]);
            if (!node) {
                printf("Error: node %s not found.\n", tokens[1]);
                continue;
            }
            int ret = delete_node(tree, node);
            if (ret == 0)
                printf("Deleted node %s.\n", tokens[1]);
            else if (ret == -2)
                printf("Error: not a leaf node, cannot delete.\n");
            else
                printf("Error: could not delete the node.\n");
        }

        else if (match_cmd(cmd, "Update", 'U')) {
            if (ntok != 3) {
                printf("Error: wrong number of arguments for Update.\n");
                continue;
            }
            if (!is_upper_letter(tokens[2])) {
                printf("Error: new-data must be a single uppercase letter.\n");
                continue;
            }
            Node* node = find_node(tree, tokens[1]);
            if (!node) {
                printf("Error: node %s not found.\n", tokens[1]);
                continue;
            }
            int ret = update_value(tree, node, tokens[2][0]);
            if (ret == 0)
                printf("Changed the data of node %s to %c.\n", tokens[1], tokens[2][0]);
            else if (ret == -2)
                printf("Error: cannot have the same data as the sibling node.\n");
            else
                printf("Error: could not change the value.\n");
        }

        else if (match_cmd(cmd, "Read", 'R')) {
            if (ntok != 2) {
                printf("Error: wrong number of arguments for Read.\n");
                continue;
            }
            Node* node = find_node(tree, tokens[1]);
            if (!node) {
                printf("Error: node %s not found.\n", tokens[1]);
                continue;
            }
            read_child(tree, node);
        }

        else if (match_cmd(cmd, "Print", 'P')) {
            if (ntok != 1) {
                printf("Error: Print takes no extra arguments.\n");
                continue;
            }
            print_btree(tree);
        }

        else {
            printf("Error: unknown command: %s\n", cmd);
        }
    }

    destroy_btree(tree);
    printf("\nExiting the program.\n");
    return 0;
}

