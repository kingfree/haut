#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int N = 1010;
const int M = 1010;
struct Matrix {
	int n, m;
	int a[N][M];
	void clear() {
		n = m = 0;
		memset(a, 0, sizeof(a));
	}
	Matrix operator + (const Matrix &b) const {
		Matrix t;
		t.n = n, t.m = m;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
				t.a[i][j] = a[i][j] + b.a[i][j];
		return t;
	}
	Matrix operator - (const Matrix &b) const {
		Matrix t;
		t.n = n, t.m = m;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
				t.a[i][j] = a[i][j] - b.a[i][j];
		return t;
	}
	Matrix operator * (const Matrix &b) const {
		Matrix t;
		t.clear();
		t.n = n, t.m = b.m;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < b.m; j++)
				for(int k = 0; k < m; k++)
				t.a[i][j] = a[i][j] * b.a[i][j];
		return t;
	}
};
int main() {
	int n, m;
	while(scanf("%d%d", &n, &m), n || m) {
	}
	return 0;
}