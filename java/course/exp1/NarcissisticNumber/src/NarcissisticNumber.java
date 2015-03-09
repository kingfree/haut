import java.util.Scanner;

/**
 * 判断水仙花数
 * 
 * @version 2015-3-9
 * @author Kingfree
 */

public class NarcissisticNumber {

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        in.close();

        int m = n;
        int s = 0;
        while (n > 0) {
            int a = n % 10;
            n /= 10;
            s += Math.pow(a, 3);
        }

        if (m == s) {
            System.out.println(m + " 是水仙花数。");
        } else {
            System.out.println(m + " 不是水仙花数。");
        }
    }

}
