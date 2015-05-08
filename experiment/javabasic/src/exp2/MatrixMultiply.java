package exp2;

import java.util.Scanner;

/**
 * 计算两个3×3矩阵的和，int[][]或double[][]都可以。 方法接受参数，并返回计算的结果。
 * 
 * @version 2015-3-17
 * @author Kingfree
 */
public class MatrixMultiply {

    /**
     * 计算矩阵乘法（叉积）
     * 
     * @param a 矩阵 A
     * @param b 矩阵 B
     * @return 乘积的二维数组 C。无法相乘则返回一个0×0的数组
     */
    public static double[][] multiply(double[][] a, double[][] b) {
        int m = a.length;
        int n = b[0].length;
        int l = a[0].length;
        if (l != b.length) {
            return new double[0][0];
        }
        double[][] c = new double[m][n];
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < l; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return c;
    }

    /**
     * 输入一个二维数组
     * 
     * @param in 输入来源
     * @param m 高度
     * @param n 宽度
     * @return 读入的二维数组
     */
    public static double[][] inputArray(Scanner in, int m, int n) {
        double[][] a = new double[m][n];
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = in.nextDouble();
            }
        }
        return a;
    }

    /**
     * 输出一个二维数组
     * 
     * @param c 要输出的数组
     */
    public static void outputArray(double[][] c) {
        for (double[] a : c) {
            for (double b : a) {
                System.out.print(b + " ");
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        int n = 3;

        Scanner in = new Scanner(System.in);

        System.out.println("请输入矩阵 A:");
        double[][] a = inputArray(in, n, n);

        System.out.println("请输入矩阵 B:");
        double[][] b = inputArray(in, n, n);

        in.close();

        System.out.println("相乘结果 C:");
        outputArray(multiply(a, b));
    }

}
