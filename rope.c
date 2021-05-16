#include "./rope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define EMPTY ""


RopeNode* makeRopeNode(const char* str) {
    // TODO
}

RopeTree* makeRopeTree(RopeNode* root) {
    // TODO
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
    // TODO 1. Concat - 10p
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
