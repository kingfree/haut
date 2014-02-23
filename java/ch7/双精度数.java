import java.util.Scanner;
public class 双精度数 {
  public static void main(String[] args) {
    Scanner scanner = new Scanner(System.in);
    String input = scanner.next();
    try {
      double 数 = Double.parseDouble(input);
      System.out.printf("结果: %s", 数);
    } catch (NumberFormatException e) {
      System.out.println("无效输入。");
    }
  }
}
