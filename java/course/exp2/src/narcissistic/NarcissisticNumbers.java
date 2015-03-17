/**
 * 输出100~999之间所有的“水仙花数”。
 */
package narcissistic;

/**
 * @version 2015-3-17
 * @author Kingfree
 */
public class NarcissisticNumbers {

    public static void main(String[] args) {
        int a = 100, b = 999;
        for (int i = a; i <= b; i++) {
            int n = i;
            
            int s = 0;
            while (n > 0) {
                int r = n % 10;
                n /= 10;
                s += Math.pow(r, 3);
            }

            if (i == s) {
                System.out.print(i + " ");
            }
        }
    }

}
