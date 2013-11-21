#include <stdio.h>
#include <stdlib.h>
void swap(int *a, int *b) {
    int t;
    t = *a, *a = *b, *b = t;
}
void q_sort(int a[], int l, int r) {
    int i = l + 1, j = r, k = a[l];
    if(l >= r) return;
    while(1) {
        while(a[j] > k) j--;
        while(a[i] < k && i < j) i++;
        if(i >= j) break;
        swap(&a[i], &a[j]);
        if(a[i] == k) j--; else i++;
    } swap(&a[l], &a[j]);
    if(l < i - 1) q_sort(a, l, i - 1);
    if(j + 1 < r) q_sort(a, j + 1, r);
}
int main() {
    int n, a[100], i, j;
    scanf("%d", &n);
    for(i = 0; i < n; i++)
        printf("%d ", a[i] = rand() % 1000);
    printf("\n");

    q_sort(a, 0, n - 1);

    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}
