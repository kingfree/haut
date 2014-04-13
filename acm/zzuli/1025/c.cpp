#include <cstdio>
#include <vector>
using namespace std;

vector<char> v['Z' + 1];
bool dfs(char c, char d) { // 在c链中找d
    int l = v[c].size();
    // fprintf(stderr, "\n%c: ", c);
    for(int i = 0; i < l; i++) {
        // fprintf(stderr, "%c ", v[c][i]);
        if (v[c][i] == d)
            return true;
        else if (dfs(v[c][i], d))
            return true;
    }
    return false;
}

int main() {
    int m;
    char a, b, c;
    bool ok;

    while(scanf("%d\n", &m) > 0) {
        for (int i = 'A'; i <= 'Z'; i++)
            v[i].clear();
        ok = true;
        for (int i = 0; i < m; i++) {
            scanf("%c%c%c\n", &a, &c, &b);
            if (!ok) continue;
            if (c == '<') {
                if (!dfs(b, a))
                    v[a].push_back(b);
                else
                    ok = false;
            } else if (c == '>') {
                if (!dfs(a, b))
                    v[b].push_back(a);
                else
                    ok = false;
            }
        }
        printf("%s\n", ok ? "YES" : "NO");
    }
    return 0;
}

