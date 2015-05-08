package exp2;

/**
 * 编程求出1000之内的所有完数。
 *
 * @version 2015-3-17
 * @author Kingfree
 */
public class PerfectNumber {

    /**
     * 判断是否为完美数
     * 
     * @param n 待判断的数
     * @return 是否为完美数
     */
    public static boolean isPerfectNumber(int n) {
        int s = 0;
        for (int i = 1; i * 2 <= n; i++) {
            if (n % i == 0) {
                s += i;
            }
        }
        return s == n;
    }

    public static void main(String[] args) {
        int n = 1000;
        for (int i = 1; i <= n; i++) {
            if (isPerfectNumber(i)) {
                System.out.println(i);
            }
        }
    }

}
