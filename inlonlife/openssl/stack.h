#ifndef _STACK_H_
#define _STACK_H_

typedef struct {
    void *elems;
    int size;
    int len;
    int alloc;
} stack;

stack *stack_new(int size);
void stack_free(stack *s);
int stack_empty(const stack *s);
void stack_push(stack *s, const void *ele);
void *stack_pop(stack *s, void *ele);

#endif
