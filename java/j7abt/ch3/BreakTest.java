public class BreakTest {
  public static void main(String[] args) {
    int i = 0;
    while (true) {
      System.out.println(i);
      i++;
      if (i > 3) {
        break;
      }
    }

    int m = 0;
    for (; ; ) {
      System.out.println(m);
      m++;
      if (m > 4) {
        break;
      }
    }

  }
}
