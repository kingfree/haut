public class 数组克隆测试 {
  public static void main(String[] args) {
    int[] 数组1 = {1, 2};
    int[] 数组2 = 数组1.clone();
    数组1[0] = 10;
    for (int i : 数组1) {
      System.out.println(i);
    }
    for (int i : 数组2) {
      System.out.println(i);
    }
  }
}
