public class 格式测试 {
  public static void main(String[] args) {
    String 姓 = "東山";
    String 名 = "奈央";
    System.out.printf("尊姓: %s，大名: %s\n", 姓, 名);
    System.out.println("尊姓: " + 姓 + "，大名: " + 名);
    int a = 65;
    int b = 66;
    System.out.format("尊姓: %x，大名: %x", a, b);
    // printf 是 format 的别名
  }
}
