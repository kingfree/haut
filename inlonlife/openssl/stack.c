#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define ALLOCSIZE 4

stack *stack_new(int size)
{
    stack *s = malloc(sizeof(stack));
    s->size = size;
    s->len = 0;
    s->alloc = ALLOCSIZE;
    s->elems = malloc(ALLOCSIZE);
    return s;
}

void stack_free(stack *s) { free(s->elems); }

int stack_empty(const stack *s) { return s->len == 0; }

void stack_push(stack *s, const void *ele)
{
    void *dst;
    if (s->len == s->alloc) {
        s->alloc += ALLOCSIZE + 2;
        s->elems = realloc(s->elems, s->alloc * s->size);
    }
    dst = (char *)s->elems + s->len * s->size;
    memcpy(dst, ele, s->size);
    s->len++;
}

void *stack_pop(stack *s)
{
    void *ele;
    s->len--;
    ele = (char *)s->elems + s->len * s->size;
    return ele;
}
