public class ForTest {
  public static void main(String[] args) {
    System.out.println("ex1");
    for (int i = 0; i < 3; i++) {
      System.out.print(i + " ");
    }

    System.out.println("\nex2");
    for (int i = 0; i < 3; i++) {
      if (i % 2 == 0) {
        System.out.print(i + " ");
      }
    }

    System.out.println("\nex3");
    for (int i = 0; i < 3; i += 2) {
      System.out.print(i + " ");
    }

    System.out.println("\nex4");
    for (int i = 0; i < 3; i++) {
      System.out.print(i + " ");
    }

    System.out.println("\nex5");
    int j = 0;
    for (; j < 3; j++) {
      System.out.print(j + " ");
    }

    System.out.println("\nex6");
    int k = 0;
    for (; k < 3; ) {
      System.out.print(k + " ");
      k++;
    }

    System.out.println("\nex7");
    int m = 0;
    for (; ; ) {
      System.out.print(m + " ");
      m++;
      if (m > 2) {
        break;
      }
    }

  }
}
