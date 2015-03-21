class Base {
  public Base() {
    System.out.println("Base");
  }
  public Base(String s) {
    System.out.println("Base." + s);
  }
}
public class Sub extends Base {
  public Sub(String s) {
    super(s);
    System.out.println(s);
  }
  public static void main(String[] args) {
    Sub sub = new Sub("开始");
  }
}
