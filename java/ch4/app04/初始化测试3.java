package app04;

public class 初始化测试3 {
  int x = 3;
  int y;
  // 实例初始化代码
  public 初始化测试3() {
    初始化();
  }
  public 初始化测试3(int x) {
    this.x = x;
    初始化();
  }
  private void 初始化() {
    y = x * 2;
    System.out.println(y);
  }
  // 静态初始化代码
  static {
    System.out.println("静态初始化");
  }
  public static void main(String[] args) {
    初始化测试3 测试 = new 初始化测试3();
    初始化测试3 測試 = new 初始化测试3();
  }

}
