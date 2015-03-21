public class 系统属性测试 {
  public static void main(String[] args) {
    java.util.Properties 属性 = System.getProperties();
    属性.list(System.out);
  }
}
