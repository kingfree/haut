#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stud_node {
    int num;
    char name[20];
    int score;
    struct stud_node *next;
};

int main()
{
    struct stud_node *head, *tail, *p;
    int num, score;
    char name[20];
    int size = sizeof(struct stud_node);
    head = tail = NULL;
    printf("Input num, name and score:\n");
    scanf("%d", &num);
    while (num != 0) {
        p = malloc(size);
        scanf("%s%d", name, &score);
        p->num = num;
        strcpy(p->name, name);
        p->score = score;
        p->next = NULL;
        if (head == NULL)                   /* 首指针 */
            head = p;
        else
            tail->next = p;
        tail = p;
        scanf("%d", &num);
    }
    for (p = head; p != NULL; p = p->next)  /* 走到头 */
        printf("%d %s %d\n", p->num, p->name, p->score);
    return 0;
}
