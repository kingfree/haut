/* score.c
* ≥…º®¿‡
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "score.h"

static char *score_db_name = "score.db";

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
    s->now = 0;
    time(&s->date);
    memset(s->answer, 0, sizeof(s->answer));
    return s;
}

int score_read_list(List *list)
{
    return read_file_to_list(score_db_name, list, sizeof(Score));
}

int score_write_file(List *list)
{
    return write_list_to_file(score_db_name, list, sizeof(Score));
}

int score_did(Score *s, char c, char ans)
{
    assert(s->now <= s->paper_count);
    s->answer[s->now++] = c;
    s->right += (c == ans);
    return s->now;
}
