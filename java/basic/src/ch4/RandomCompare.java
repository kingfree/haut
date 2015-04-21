package ch4;

import java.util.Random;

/**
 * @version 2015-4-3
 * @author Kingfree
 */
public class RandomCompare {

    public static void main(String[] args) {
        int n = 10;
        int[] a = new int[n];
        Random rand = new Random();
        for (int i = 0; i < n; i++) {
            a[i] = rand.nextInt(1000);
        }
        for (int e : a) {
            System.out.print(e + " ");
        }
        System.out.println();

        for (int i = 0; i < n - 1; i++) {
            char c = a[i] < a[i + 1] ? '<' : ((a[i] == a[i + 1]) ? '=' : '>');
            System.out.println(a[i] + " " + c + " " + a[i + 1]);
        }
    }

}
