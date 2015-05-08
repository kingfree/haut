#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student {
    int id;
    char name[20];
    int score;
    struct student *next;
} student;

int main()
{
    student *head, *tail, *p;
    int id, score;
    char name[20];
    int size = sizeof(student);
    head = tail = NULL;
    scanf("%d", &id);
    while (id != 0) {
        p = malloc(size);
        scanf("%s%d", name, &score);
        p->id = id;
        strcpy(p->name, name);
        p->score = score;
        p->next = NULL;
        if (head == NULL)
            head = p;
        else
            tail->next = p;
        tail = p;
        scanf("%d", &id);
    }
    scanf("%d", &score);                     /* 输入成绩 */
    for (p = head; p != NULL; p = p->next)
        if (p->score >= score)               /* 判断输出 */
            printf("%d %s %d\n", p->id, p->name, p->score);
    return 0;
}
