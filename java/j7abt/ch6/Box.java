public class Box {
  public int length;
  public int width;
  public int height;

  public Box(int length, int width, int height) {
    this.length = length;
    this.width = width;
    this.height = height;
  }

  public String toString() {
    return "我是个盒子";
  }

  public Object clone() {
    return new Box(1, 1, 1);
  }
}
