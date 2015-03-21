import java.util.Scanner;
public class ScannerTest {
  public static void main(String[] args) {
    Scanner scanner = new Scanner(System.in);
    String s = scanner.next();
    System.out.println("你输入了: " + s);
  }
}
