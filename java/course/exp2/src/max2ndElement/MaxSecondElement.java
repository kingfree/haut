/**
 * 在一个方法中实现从一个数组中找到该数组的最大值和次大值并返回。
 */
package max2ndElement;

/**
 * @version 2015-3-17
 * @author Kingfree
 */
public class MaxSecondElement {

    /**
     * 计算数组中的最大值和次大值
     * 
     * @param a
     *            待查找的数组
     * @return 数组的最大值和次大值组成的含有两个元素的数组
     */
    public static double[] max2ndelement(double[] a) {
        double[] c = { 0, 0 };
        for (double b : a) {
            if (b > c[0]) {
                c[1] = c[0];
                c[0] = b;
            } else if (b > c[1]) {
                c[1] = b;
            }
        }
        return c;
    }

    public static void main(String[] args) {
        int n = 10;
        double a[] = new double[n];
        for (int i = 0; i < n; i++) {
            a[i] = ((int) (Math.random() * 10000)) / 100.00;
            System.out.print(a[i] + " ");
        }
        System.out.println();
        
        double[] result = max2ndelement(a);
        System.out.println("最大值: " + result[0]);
        System.out.println("次大值: " + result[1]);
    }

}
