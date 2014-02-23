import java.util.Date;
public class DateTest {
  public static void main(String[] args) {
  Date 日期1 = new Date(1000);
  Date 日期2 = new Date(1001);
  if (日期1.before(日期2))
    System.out.println("日期1 在 日期2 之前");
  else
    System.out.println("日期1 大于等于 日期2");
  System.out.println(new Date(0)); 
  }
}
