#include <iostream>
#include <vector>
#include <climits>
using namespace std;

void Merge(vector<int> &A, int p, int q, int r)
{
    int n_1 = q - p + 1;
    int n_2 = r - q;
    vector<int> L(n_1 + 2), R(n_2 + 2);
    for (int i = 1; i <= n_1; i++)
        L[i] = A[p + i -1];
    for (int j = 1; j <= n_2; j++)
        R[j] = A[q + j];
    L[n_1 + 1] = R[n_2 + 1] = INT_MAX;
    int i = 1, j = 1;
    for (int k = p; k <= r; k++) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
        } else {
            A[k] = R[j];
            j++;
        }
    }
}

void MergeSort(vector<int> &A, int p, int r)
{
    if (p < r) {
        int q = (p + r) / 2;
        MergeSort(A, p, q);
        MergeSort(A, q + 1, r);
        Merge(A, p, q, r);
    }
}

int main()
{
    int n;
    cin >> n;

    vector<int> a(n + 1);
    
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    MergeSort(a, 1, n);

    for (int i = 1; i <= n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;

    return 0;
}
