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
    // Verify if the node is null
    if (!rt) {
        return 0;
    }

    // Adding the length of a leaf node's string
    if (!rt->left && !rt->right) {
        return strlen(rt->str);
    }

    // Returning the weight and calling the function
    // for the right subtree
    return rt->weight + getWeight(rt->right);
}


RopeTree* concat(RopeTree* rt1, RopeTree* rt2) {
    // Initialize the root node of the new tree
    RopeNode *root = makeRopeNode(strdup(EMPTY));
    root->left = rt1->root;
    root->right = rt2->root;
    root->weight = getWeight(root->left);

    // Initialize the new tree
    RopeTree *new_tree = makeRopeTree(root);

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
    /* In the case the split idex is in the centre
    of a node's string, we split the node in two. After
    the split, we return the node to be able to bring the tree
    back to it's original form. */
    if (node->weight <= idx && node->right !=  NULL) {
        // Searching for the node
        return fixNode(node->right, idx - node->weight);
    } else {
        if (node->left != NULL) {
            return fixNode(node->left, idx);
        } else {
            if (idx > 0) {
                // Declaring the new strings and the new nodes
                RopeNode *new_right, *new_left;
                char *str_left, *str_right;
                char *str1;

                // Filling the first node's string
                str1 = strdup(node->str);
                str1[idx]= '\0';
                str_left = strdup(str1);

                // filing the right node's string
                str_right = strdup(node->str + idx);
                str_right[strlen(str_right)] = '\0';

                // Creating the nodes
                new_left = makeRopeNode(str_left);
                new_right = makeRopeNode(str_right);

                // Making the two nodes children
                node->left = new_left;
                node->right = new_right;
                node->weight = strlen(node->left->str);

                free(str1);
                return node;
            }
        }
    }

    // In case the node doesn't exist
    return NULL;
}

RopeNode *indexNode(RopeNode *node, int idx) {
    if (node == NULL) {
        return NULL;
    }

    // Finding the node at a index from a rope tree
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

    // If the split index is 0 or the length of the rope tree
    // than one of the trees is initialized with EMPTY
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

    // Splitting a node in case the index isn't
    // at the beginning of a nodes string
    RopeNode *node = fixNode(rt->root, idx);

    // Finding the node at a certain index and creates the left tree
    RopeNode *node_idx = indexNode(rt->root, 0);
    trees.left = makeRopeNode(strdup(node_idx->str));

    /* If the tree we build contains
    only one node, than we create a root node.This
    variable remembers if there is more than one node. */
    int isOne = 1;

    // Creates the right tree
    node_idx = indexNode(rt->root, idx);
    trees.right = makeRopeNode(strdup(node_idx->str));

    // Starts to add the nodes to the left tree
    i = strlen(trees.left->str);
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
    // Adds a root in case we added only on node
    if (isOne) {
        RopeNode *new_root = makeRopeNode(strdup(EMPTY));
        new_root->left = trees.left;
        new_root->weight = getWeight(new_root->left);
        trees.left = new_root;
    }

    // Does the same for the right tree
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

    // If we split a node into two, we recreate the intial rope tree
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
    RopeNode *node = makeRopeNode(str);

    // Splitting the rope tree from the index and
    // initializing the middle, left and right trees
    // used in the end
    SplitPair sp = split(rt, idx);
    RopeTree *rt_middle = makeRopeTree(node);
    RopeTree *rt_left = makeRopeTree(sp.left);
    RopeTree *rt_right = makeRopeTree(sp.right);

    RopeTree *int_rt, *final_rt;
    int_rt = concat(rt_left, rt_middle);
    final_rt = concat(int_rt, rt_right);

    // Freeing the memory
    free(rt_middle);
    free(rt_left);
    free(rt_right);
    free(int_rt);

    return final_rt;
}

RopeTree* delete(RopeTree* rt, int start, int len) {
    // Splitting the rope tree
    SplitPair sp1 = split(rt, start);
    RopeTree *rtr = makeRopeTree(sp1.right);
    int length = getWeight(rtr->root);

    // Second split
    SplitPair sp2;
    if (len > length) {
        len = length;
    }
    sp2 = split(rtr, len);

    // Initializing the two rope trees to concat them
    RopeTree *rt1 = makeRopeTree(sp1.left);
    RopeTree *rt2 = makeRopeTree(sp2.right);

    // Final tree concat
    RopeTree *final = concat(rt1, rt2);

    // Freeing the memory
    free(rtr);
    free(rt1);
    free(rt2);

    return final;
}
