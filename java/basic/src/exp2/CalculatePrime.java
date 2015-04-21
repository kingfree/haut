package exp2;

/**
 * 求100至200间的全部素数。
 * 
 * @version 2015-3-17
 * @author Kingfree
 */
public class CalculatePrime {

    public static void main(String[] args) {
        int n = 2048;
        int[] primes = new int[n];
        for (int i = 2; i < n; i++) {
            for (int j = i + i; j < n; j += i) {
                primes[j] = 1;
            }
        }

        int a = 100, b = 200;
        for (int i = a; i <= b; i++) {
            if (primes[i] == 0) {
                System.out.print(i + " ");
            }
        }
    }

}