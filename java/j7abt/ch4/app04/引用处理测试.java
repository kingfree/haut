package app04;

class Point {
  public int x;
  public int y;
}

public class 引用处理测试 {
  static {
    System.out.println("静态");
  }
  public static void 递增(int x) {
    x++;
  }
  public static void 重置(Point point) {
    point.x = 0;
    point.y = 0;
  }
  public static void main(String[] args) {
    int a = 9;
    递增(a);
    System.out.println(a); // => 9
    Point p = new Point();
    p.x = 400;
    p.y = 600;
    重置(p);
    System.out.println(p.x); // => 0
  }
  static {
    System.out.println("静态2");
  }

}
