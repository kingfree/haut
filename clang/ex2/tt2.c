#include <stdio.h>
#include <stdlib.h>

typedef struct list {
    int num;
    struct list *next;
} list;

int main()
{
    list *head, *tail, *p;
    int num;
    int size = sizeof(list);
    head = tail = NULL;
    while (scanf("%d", &num), num != -1) {
        p = malloc(size);
        p->num = num;
        if (tail == NULL)
            tail = p;
        p->next = head;
        head = p;
        // fprintf(stderr, "%p->%p\n", p, p->next);
    }
    for (p = head; p != NULL; p = p->next)
        printf("%d ", p->num);
    printf("\n");
    return 0;
}
