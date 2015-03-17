/**
 * 求Fibonacci数列的前40个数。即F1 = 1，F2 = 1，F[n] = F[n−1] +F[n−2] (n ≥ 3)。
 */
package fibonacci;

/**
 * @version 2015-3-17
 * @author Kingfree
 */
public class Fibonacci {

    public static void main(String[] args) {
        int n = 40;
        int[] f = new int[n + 1];
        f[1] = 1;
        f[2] = 1;
        for (int i = 3; i <= n; i++) {
            f[i] = f[i - 1] + f[i - 2];
        }
        
        for (int i = 1; i <= n; i++) {
            System.out.printf("%10d ", f[i]);
            if (i % 8 == 0) {
                System.out.println();
            }
        }
        System.out.println();
    }

}
