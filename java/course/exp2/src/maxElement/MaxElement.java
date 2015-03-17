/**
 * 声明一个类，定义一个方法以计算一维数组中的最大值并返回该值，参数为int[]或double[]。
 * 在main方法中调用该方法，传递不同长度的数组，得到返回值并输出。
 */
package maxElement;

/**
 * @version 2015-3-17
 * @author Kingfree
 */
public class MaxElement {

    /**
     * 计算数组中的最大值
     * 
     * @param a
     *            待查找的数组
     * @return 数组的最大值
     */
    public static double max_element(double[] a) {
        double c = a[0];
        for (double b : a) {
            c = b > c ? b : c;
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
        System.out.println("最大值: " + max_element(a));
    }

}
