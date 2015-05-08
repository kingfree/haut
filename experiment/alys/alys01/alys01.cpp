#include <iostream>
#include <iomanip>
using namespace std;

const int MAXN = 128;

class Matrix
{
public:
    int l; // 行（下标1）
    int c; // 列（下标2）
    int A[MAXN][MAXN];

    Matrix(int m, int n)
    {
        c = m;
        l = n;
        for (int i = 0; i < l; i++) {
            for (int j = 0; j < c; j++) {
                A[i][j] = 0;
            }
        }
    }

    void readin()
    {
        for (int i = 0; i < l; i++) {
            for (int j = 0; j < c; j++) {
                cin >> A[i][j];
            }
        }
    }

    void writeout()
    {
        for (int i = 0; i < l; i++) {
            for (int j = 0; j < c; j++) {
                cout << setw(4) << A[i][j];
            }
            cout << endl;
        }
    }

    Matrix operator*(Matrix B)
    {
        Matrix C(B.c, l);
        for (int i = 0; i < l; i++) {
            for (int j = 0; j < B.c; j++) {
                for (int k = 0; k < c; k++) {
                    C.A[i][j] += A[i][k] * B.A[k][j];
                }
            }
        }
        return C;
    }
};

int main(int argc, char **argv)
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n, m;
    cin >> n >> m;
    Matrix A(m, n);
    A.readin();

    cin >> n >> m;
    Matrix B(m, n);
    B.readin();

    if (A.c != B.l) {
        cerr << "不可相乘！" << endl;
        return -1;
    }

    Matrix C = A * B;
    C.writeout();

    return 0;
}
