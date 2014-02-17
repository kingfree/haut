package app04;

public class 静态初始化测试 {
    public static int a = 5;
    public static int b = a * 2;
    static {
        System.out.println("静态");
        System.out.println(b);
    }
    public static void main(String[] args) {
        System.out.println("主方法");
    }
}
