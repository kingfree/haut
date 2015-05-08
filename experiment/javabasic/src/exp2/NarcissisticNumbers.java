package exp2;

/**
 * 输出100~999之间所有的“水仙花数”。
 *
 * @version 2015-3-19
 * @author Kingfree
 */
public class NarcissisticNumbers {

    public static boolean isNarciss(int n) {
        int m = n;
        int[] a = new int[32];
        int i = 0;
        while (n != 0) {
            a[i++] = n % 10;
            n /= 10;
        }

        int s = 0;
        for (int j = 0; j < i; j++) {
            s += Math.pow(a[j], i);
        }

        return m == s;
    }

    public static void main(String[] args) {
        int a = 100, b = 999;
        for (int i = a; i <= b; i++) {
            if (isNarciss(i)) {
                System.out.print(i + " ");
            }
        }
    }

}
