package exp1;

import java.util.Scanner;

/**
 * 判断水仙花数 <br>
 * 摘录几个水仙花数： 153 370 371 407 1634 8208 9474 54748 92727 93084
 * 
 * @version 2015-3-19
 * @author Kingfree
 */

public class NarcissisticNumber {

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
        Scanner in = new Scanner(System.in);
        System.out.println("请输入一个整数：");
        int n = in.nextInt();
        in.close();

        if (isNarciss(n)) {
            System.out.println(n + " 是水仙花数。");
        } else {
            System.out.println(n + " 不是水仙花数。");
        }
    }

}
