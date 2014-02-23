import java.text.DateFormat;
import java.text.ParseException;
import java.util.Date;
public class DateFormatTest {
  public static void main(String[] args) {
    DateFormat 短 = DateFormat.getDateInstance(DateFormat.SHORT);
    DateFormat 中 = DateFormat.getDateInstance(DateFormat.MEDIUM);
    DateFormat 长 = DateFormat.getDateInstance(DateFormat.LONG);
    DateFormat 全 = DateFormat.getDateInstance(DateFormat.FULL);
    System.out.println(短.format(new Date()));
    System.out.println(中.format(new Date()));
    System.out.println(长.format(new Date()));
    System.out.println(全.format(new Date()));
    try {
      Date date = 短.parse("12/12/2012");
    } catch (ParseException e) {
    }
  }
}
