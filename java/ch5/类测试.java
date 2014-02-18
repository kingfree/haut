public class 类测试 {
  public static void main(String[] args) {
    String 国家 = "中国";
    Class 新类 = 国家.getClass();
    System.out.println(新类.getName());
    Class 类 = null;
    try {
      类 = Class.forName("测试");
      // 从类名称创建一个类对象
    } catch (ClassNotFoundException e) {
      // 类找不到异常
    }
    if (类 != null) {
      try {
        测试 测 = (测试) 类.newInstance();
        // 创建这个“测试”类的实例
        测.print();
      } catch (IllegalAccessException e) {
        // 非法访问异常
      } catch (InstantiationException e) {
        // 实例化异常
      }
    }
  }
}
