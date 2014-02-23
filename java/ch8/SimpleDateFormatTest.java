import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
public class SimpleDateFormatTest {
  public static void main(String[] args) {
    String pattern = "MM/dd/yyyy";
    SimpleDateFormat format = new SimpleDateFormat(pattern);
    try {
      Date date = format.parse("01/21/2014");
    } catch (ParseException e) {
      e.printStackTrace();
    }
    System.out.println(format.format(new Date()));
  }
}
