#ifndef STACK_H_   /* Include guard */
#define STACK_H_

// A structure to represent a stack
struct StackNode
{
    double data;
    struct StackNode* next;
};

struct StackNode* newNode(double data);

int isEmpty(struct StackNode *root);

void push(struct StackNode** root, double data);

double pop(struct StackNode** root);

double peek(struct StackNode* root);


#endif // STACK_H_
