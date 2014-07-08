/* score.c
* ≥…º®¿‡
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "score.h"

Score *score_new(User *u, Paper *p)
{
    assert(u);
    assert(p);
    Score *s = (Score *)malloc(sizeof(Score));
    assert(s);
    s->id = 0;
    s->user_id = u->id;
    memset(s->username, 0, sizeof(s->username));
    strcpy(s->username, u->username);
    s->paper_id = p->id;
    s->paper_count = p->length;
    s->right = 0;
    time(&s->date);
    memset(s->answer, 0, sizeof(s->answer));
    return s;
}

int score_read_list(List *list)
{
    return 0;
}

int score_write_file(List *list)
{
    return 0;
}

int score_did(Score *s, char c)
{
    return 0;
}

int score_restore(Score *s)
{
    return 0;
}
