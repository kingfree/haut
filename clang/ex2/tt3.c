#include <stdio.h>
#include <stdlib.h>

typedef struct list {
    int num;
    struct list *next;
} list;

int main()
{
    list *head, *tail, *p, *q;
    int num;
    int size = sizeof(list);
    head = tail = NULL;
    while (scanf("%d", &num), num != -1) {
        p = malloc(size);
        p->num = num;
        p->next = NULL;
        if (head == NULL)
            head = p;
        else
            tail->next = p;
        tail = p;
    }
    for (p = head; p && p->next; p = p->next) {
        if (p->next->num % 2 == 0) {
            q = p->next;
            p->next = p->next->next;
            if (p->next == NULL)
                tail = p;
            free(q);
        }
    }
    if (head->num % 2 == 0) {
        q = head;
        head = head->next;
        free(q);
    }
    for (p = head; p != NULL; p = p->next)
        printf("%d ", p->num);
    printf("\n");
    return 0;
}
