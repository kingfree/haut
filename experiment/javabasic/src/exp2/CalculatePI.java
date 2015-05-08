package exp2;

/**
 * 用$\frac{\pi}{4}\approx 1-\frac{1}{3}+\frac{1}{5}-\frac{1}{7}$公式
 * 求$\pi$的近似值，直到最后一项绝对值小于$10^{-6}$。
 *
 * @version 2015-3-17
 * @author Kingfree
 */
public class CalculatePI {

    /**
     * 依公式计算 π
     * 
     * @param e 计算的精度
     * @return 按精度计算的 π 值
     */
    public static double calcpi(double e) {
        double pi = 0.0;
        double sign = 1;
        for (int i = 1; 1.0 / i > e; i += 2) {
            pi += sign / i;
            sign = -sign;
        }
        pi *= 4;
        return pi;
    }

    public static void main(String[] args) {
        double e = 1e-6;
        System.out.println(calcpi(e));
    }

}
