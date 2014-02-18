public class 字符串测试 {
  public static void main(String[] args) {
    System.out.println("Java is cool".charAt(0)); // => 'J'
    System.out.println("Java ".concat("is cool")); // => "Java is cool"
    System.out.println("Java is cool".indexOf("cool")); // => 8
    System.out.println("Java is cool".lastIndexOf("a")); // => 3
    System.out.println("Java is cool".substring(8)); // => "cool"
    System.out.println("Java is cool".substring(5, 7)); // => "is"
    System.out.println("Java is cool".length()); // => 12
    System.out.println("  Java ".trim()); // => "Java"
    System.out.println("dingdong".replace('d', 'k')); // => "kingkong"
    System.out.println("Java is cool".toLowerCase()); // => "java is cool"
    System.out.println("Java is cool".toUpperCase()); // => "JAVA IS COOL"
    String[] 元素 = "Java is cool".split(" ");
    for (String 元 : 元素) {
      System.out.println(元);
    }  
  }
}
