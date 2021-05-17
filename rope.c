#include "./rope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "utils.h"

#define EMPTY ""


RopeNode* makeRopeNode(const char* str) {
    // Allocate memory for a new node
    RopeNode *new_node = malloc(sizeof(RopeNode));
    DIE(new_node == NULL, "Unable to allocate memory for a new node!\n");

    // Add the data into the node
    new_node->str = malloc(sizeof(char) * (strlen(str) + 1));
    DIE(new_node->str == NULL, "Unable to allocate memory for the node's string!\n");
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
    tree->root= malloc(sizeof(RopeNode));
    DIE(tree->root == NULL, "Unable to allocate memory for the root!\n");
    memcpy(tree->root, root, sizeof(RopeNode));

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


RopeTree* concat(RopeTree* rt1, RopeTree* rt2) {
    // Initialize the root node of the new tree
    RopeNode *root = makeRopeNode(EMPTY);
    root->left = rt1->root;
    root->right = rt2->root;
    root->weight = getTotalWeight(root->left);
    
    // Free old trees' memory
    free(rt1);
    free(rt2);

    // Initialize the new tree
    RopeTree *new_tree = makeRopeTree(root);
    return new_tree;
}


char indexRope(RopeTree* rt, int idx) {
    // TODO 2. Index - 10p
}


char* search(RopeTree* rt, int start, int end) {
    // TODO 3. Search - 20p
}

SplitPair split(RopeTree* rt, int idx) {
    // TODO 4. Split - 20p
}

RopeTree* insert(RopeTree* rt, int idx, const char* str) {
    // TODO 5. Insert - 5p
}

RopeTree* delete(RopeTree* rt, int start, int len) {
    // TODO 6. Delete - 5p
}

// FINAL 10p -> complex test involving all operations
