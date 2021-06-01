#include "./rope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "utils.h"

#define EMPTY ""

char *strdup(const char *s);


RopeNode* makeRopeNode(const char* str) {
    // Allocate memory for a new node
    RopeNode *new_node = malloc(sizeof(RopeNode));
    DIE(new_node == NULL, "Unable to allocate memory for a new node!\n");

    // Add the data into the node
    new_node->str = str;

    // Initialize properties
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->weight = 0;

    return new_node;
}

RopeTree* makeRopeTree(RopeNode* root) {
    // Allocate memory for the new rope
    RopeTree *tree = malloc(sizeof(RopeTree));
    DIE(tree == NULL, "Unable to allocate memory for the tree!\n");

    // Initialize the root
    tree->root = root;

    return tree;
}


void printRopeNode(RopeNode* rn) {
    if (!rn)
        return;

    if (!(rn->left) && !(rn->right)) {
        printf("%s", rn->str);
        return;
    }

    printRopeNode(rn->left);
   printRopeNode(rn->right);
}

void printRopeTree(RopeTree* rt) {
    if (rt && rt->root) {
        printRopeNode(rt->root);
        printf("%s", "\n");
    }
}

void debugRopeNode(RopeNode* rn, int indent) {
    if (!rn)
        return;

    for (int i = 0; i < indent; ++i)
        printf("%s", " ");

    if (!strcmp(rn->str, EMPTY))
        printf("# %d\n", rn->weight);
    else
        printf("%s %d\n", rn->str, rn->weight);

    debugRopeNode(rn->left, indent+2);
    debugRopeNode(rn->right, indent+2);
}



int getTotalWeight(RopeNode* rt) {
    if (!rt)
        return 0;

    return rt->weight + getTotalWeight(rt->right);
}

int getWeight(RopeNode *rt) {
    if (!rt) {
        return 0;
    }
    
    if (!rt->left && !rt->right) {
        return strlen(rt->str);
    }

    return rt->weight + getWeight(rt->right);
}


RopeTree* concat(RopeTree* rt1, RopeTree* rt2) {
    // Initialize the root node of the new tree
    RopeNode *root = makeRopeNode(strdup(EMPTY));
    root->left = rt1->root;
    root->right = rt2->root;
    root->weight = getTotalWeight(root->left);
    
    // Initialize the new tree
    RopeTree *new_tree = makeRopeTree(root);

    //free la rt1 si rt2?
    
    return new_tree;
}


char __indexRope(RopeNode *node, int idx) {
    if (node->weight <= idx && node->right !=  NULL) {
        return __indexRope(node->right, idx - node->weight);
    } else {
        if (node->left != NULL) {
            return __indexRope(node->left, idx);
        } else {
            return node->str[idx];
        }
    }
}

char indexRope(RopeTree* rt, int idx) {
    return __indexRope(rt->root, idx);
}

char* search(RopeTree* rt, int start, int end) {
    // Allocate memory for the string
    char *str = malloc(sizeof(char) * (end - start + 1));
    DIE(str == NULL, "Unable to allocate memory for the search sting!\n");
    strcpy(str, EMPTY);

    // Fill the string
    for (int index = start; index < end; index++) {
        char c = indexRope(rt, index);
        str[index - start] = c;
    }
    str[end - start] = '\0';

    return str;
}

RopeNode *fixNode(RopeNode *node, int idx) {
    if (node->weight <= idx && node->right !=  NULL) {
        return fixNode(node->right, idx - node->weight);
    } else {
        if (node->left != NULL) {
            return fixNode(node->left, idx);
        } else {
            if (idx > 0) {
                RopeNode *new_right, *new_left;
                char *str_left, *str_right;
                char *first_part;
                first_part = strdup(node->str);
                first_part[idx]= '\0';
                str_left = strdup(first_part);

                str_right = strdup(node->str + idx);
                str_right[strlen(str_right)] = '\0';

                new_left = makeRopeNode(str_left);
                new_right = makeRopeNode(str_right);

                node->left = new_left;
                node->right = new_right;
                node->weight = strlen(node->left->str);

                free(first_part);
                return node;
            }
        }
    }

    return NULL;
}

RopeNode *indexNode(RopeNode *node, int idx) {
    if (node == NULL) {
        return NULL;
    }
    
    if (node->weight <= idx && node->right !=  NULL) {
        return indexNode(node->right, idx - node->weight);
    } else {
        if (node->left != NULL) {
            return indexNode(node->left, idx);
        } else {
            return node;
        }
    }
}

SplitPair split(RopeTree* rt, int idx) {
    SplitPair trees;
    int i;

    if (idx == 0) {
        trees.left = makeRopeNode(strdup(EMPTY));
        trees.right = rt->root;
        return trees;
    }

    int length = getWeight(rt->root);
    
    if (idx == length) {
        trees.right = makeRopeNode(strdup(EMPTY));
        trees.left = rt->root;
        return trees;
    }

    RopeNode *node = fixNode(rt->root, idx);

    RopeNode *node_idx = indexNode(rt->root, 0);
    int isOne = 1;
    trees.left = makeRopeNode(strdup(node_idx->str));

    node_idx = indexNode(rt->root, idx);
    trees.right = makeRopeNode(strdup(node_idx->str));

    i= strlen(trees.left->str);
    while (i < idx) {
        isOne = 0;
        node_idx = indexNode(rt->root, i);
        RopeNode *new_node = makeRopeNode(strdup(node_idx->str));
        RopeNode *new_root = makeRopeNode(strdup(EMPTY));
        new_root->left = trees.left;
        new_root->right = new_node;
        new_root->weight = getWeight(new_root->left);
        trees.left = new_root;
        
        i += strlen(new_node->str);
    }

    if (isOne) {
        RopeNode *new_root = makeRopeNode(strdup(EMPTY));
        new_root->left = trees.left;
        new_root->weight = getWeight(new_root->left);
        trees.left = new_root;
    }

    isOne = 1;
    i = idx + strlen(trees.right->str);
    while (i < length) {
        isOne = 0;
        node_idx = indexNode(rt->root, i);
        
        RopeNode *new_node = makeRopeNode(strdup(node_idx->str));
        RopeNode *new_root = makeRopeNode(strdup(EMPTY));
        new_root->left = trees.right;
        new_root->right = new_node;
        new_root->weight = getWeight(new_root->left);
        trees.right = new_root;
        
        i += strlen(new_node->str);
        
    }

    if (isOne) {
        RopeNode *new_root = makeRopeNode(strdup(EMPTY));
        new_root->left = trees.right;
        new_root->weight = getWeight(new_root->left);
        trees.right = new_root;
    }

    if (node != NULL) {
        free((void *)node->left->str);
        free(node->left);
        free((void *)node->right->str);
        free(node->right);
        node->left = NULL;
        node->right = NULL;
    }

    return trees;
}

RopeTree* insert(RopeTree* rt, int idx, const char* str) {
    // TODO 5. Insert - 5p
    return NULL;
}

RopeTree* delete(RopeTree* rt, int start, int len) {
    // TODO 6. Delete - 5p
    return NULL;
}

// FINAL 10p -> complex test involving all operations
