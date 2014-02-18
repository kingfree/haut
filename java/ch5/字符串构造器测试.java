public class 字符串构造器测试 {
  public static void main(String[] args) {
    StringBuilder sb = new StringBuilder(100);
    System.out.println(sb.capacity()); // => 100
    System.out.println(sb.length()); // => 0
    sb.append("中");
    sb.append(2); // => 中2
    System.out.println(sb);
    StringBuilder sb2 = new StringBuilder(100);
    sb2.append("和");
    sb2.insert(0, '小'); // => 小和
    System.out.println(sb2);
  }
}
