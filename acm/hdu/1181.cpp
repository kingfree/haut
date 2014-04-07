#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

const char B = 'b', M = 'm';

struct node {
    char s, t;
    bool v;
};
vector<node> c;
bool ok;
int l;

void dfs(char ch)
{
    if (ch == M)
        ok = true;
    if (ok)
        return;
    for (int i = 0; i < l; i++) {
        if (c[i].v)
            continue;
        if (c[i].s == ch) {
            c[i].v = true;
            dfs(c[i].t);
            c[i].v = false;
        }
    }
}

int main()
{
    char s[1 << 10];
    while (scanf("%s", s) > 0) {
        if (s[0] == '0')
            continue;
        c.clear();
        do {
            // c.push_back(node{s[0], s[strlen(s) - 1]}); // C++ 11
            node n = {s[0], s[strlen(s) - 1]};
            c.push_back(n);
        } while (scanf("%s", s), s[0] != '0');
        ok = false;
        l = c.size();
        for (int i = 0; i < l; i++) {
            if (c[i].s == B) {
                for (int j = 0; j < l; j++)
                    c[j].v = false;
                c[i].v = true;
                dfs(c[i].s);
            }
            if (ok)
                break;
        }
        printf("%s.\n", ok ? "Yes" : "No");
    }
    return 0;
}

