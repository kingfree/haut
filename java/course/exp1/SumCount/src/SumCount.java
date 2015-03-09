import java.util.Scanner;

/**
 * 计算整数各位数字之和
 * 
 * @version 2015-3-9
 * @author Kingfree
 */

public class SumCount {

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        in.close();

        int s = 0;
        while (n > 0) {
            s += n % 10;
            n /= 10;
        }
        System.out.println(s);
    }

}
