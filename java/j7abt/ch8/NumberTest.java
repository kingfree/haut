import java.util.Scanner;
public class NumberTest {
  public static void main(String[] args) {
    Scanner scanner = new Scanner(System.in);
    String userInput = scanner.next();
    try {
      int i = Integer.parseInt(userInput);
      System.out.println("输入的数字: " + i);
    } catch (NumberFormatException e) {
      System.out.println("用户非法输入");
    }
  }
}
