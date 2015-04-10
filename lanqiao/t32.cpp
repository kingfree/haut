#include <cstdio>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

// cost = 10 * n + n * (n + 1) / 2;
// 深度优先遍历。
// 本题实际上是要求树中最远的两个点，可以使用两遍深度优先遍历解决。第一遍从1号点开始，找到距1号点最远的点a，如果有多个任取一个即可。第二遍从a号点开始，找到距a最远的点b，则a和b就是最远的两个点。

const int N = 1024 * 8;
const int oo = 0x7ffffff;
int n;
vector<int> v[N];
vector<int> w[N];
int d[N];
bool visited[N];
// int f[N][N];

void dfs(int u)
{
	// fprintf(stderr, "%d:%d\n", u, d[u]);
	visited[u] = true;
	int l = v[u].size();
	for (int i = 0; i < l; i++) {
		int vi = v[u][i];
		if (!visited[vi]) {
			d[vi] = d[u] + w[u][i];
			// fprintf(stderr, "%d(\%d) = %d(%d) + %d\n", d[vi], vi, d[u], u, w[u][i]);
			dfs(vi);
		}
	}
}

void init()
{
	/* for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			f[i][j] = oo;
		}
		f[i][i] = 0;
	} */
	fill_n(d, N, 0);
	fill_n(visited, N, false);
}

/* void floyd()
{
	for (int k = 1; k <= n; k++)
	for (int i = 1; i <= n; i++)
	for (int j = 1; j <= n; j++)
		f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
} */

inline int calc(int n)
{
	return 10 * n + n * (n + 1) / 2;	
}

int main()
{
	scanf("%d", &n);
	init();
	for (int i = 1; i < n; i++) {
		int p, q, d;
		scanf("%d %d %d", &p, &q, &d);
		// f[p][q] = f[q][p] = d;
		v[p].push_back(q);
		w[p].push_back(d);
		v[q].push_back(p);
		w[q].push_back(d);
	}

	// floyd();

	/* int s = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			// fprintf(stderr, "%2d ", f[i][j]);
			if (f[i][j] < oo)
				s = max(s, f[i][j]);
		}
		// fprintf(stderr, "\n");
	} */
	init();
	dfs(1);
	int a = max_element(d + 1, d + n + 1) - d;
	// fprintf(stderr, "%d\n", k);
	init();
	dfs(a);
	int s = *max_element(d + 1, d + n + 1);
	// fprintf(stderr, "%d\n", s);
	printf("%d\n", calc(s));

	return 0;
}
