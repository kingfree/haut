package app04;

public class 初始化测试1 {
  int x = 3;
  int y;
  // 实例初始化代码
  {
    y = x * 2;
    System.out.println(y);
  }
  // 静态初始化代码
  static {
    System.out.println("静态初始化");
  }
  public static void main(String[] args) {
    初始化测试1 测试 = new 初始化测试1();
    初始化测试1 測試 = new 初始化测试1();
  }

}
