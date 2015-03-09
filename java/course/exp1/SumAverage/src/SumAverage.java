import java.util.Scanner;

/**
 * 求数组的和、平均值
 * 
 * @version 2015-3-9
 * @author Kingfree
 */

public class SumAverage {

    public static final int MAX_RANDOM = 1000;

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        in.close();

        int[] a = new int[n];
        for (int i = 0; i < a.length; i++) {
            a[i] = (int) (Math.random() * MAX_RANDOM);
            System.out.print(a[i] + " ");
        }

        int sum = 0;
        for (int element : a) {
            sum += element;
        }
        double average = (double) sum / n;

        System.out.println();
        System.out.println("数组和：" + sum);
        System.out.println("平均值：" + average);
    }

}
