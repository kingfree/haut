public class 数组测试 {
  public static void main(String[] args) {
    int[] 数组 = new int[4];
    System.out.println(数组[2]);
    System.out.println("长度: " + 数组.length);
    System.out.println("类型: " + 数组.getClass().getName());
    System.out.println("是数组: " + 数组.getClass().isArray());

    long 数们[] = new long[2];
    System.out.println(数们[1]);

    String[] 名字 = {"田村", "水樹", "堀江"};
    for (String name : 名字) {
      System.out.println(name);
    }
    System.out.println(名字[1]);

    int[] 矩阵 = { 1, 2, 3, 10 };
    System.out.println(矩阵[2]);
  }
}
