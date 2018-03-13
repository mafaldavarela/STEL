// C program for linked list implementation of stack
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "stack.h"


struct StackNode* newNode(double data){
    struct StackNode* stackNode = (struct StackNode*) malloc(sizeof(struct StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

int isEmpty(struct StackNode *root){
    return !root;
}

void push(struct StackNode** root, double data){
    struct StackNode* stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
}

double pop(struct StackNode** root){
    if (isEmpty(*root))
        return INT_MIN;
    struct StackNode* temp = *root;
    *root = (*root)->next;
    double popped = temp->data;
    free(temp);

    return popped;
}
