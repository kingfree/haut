import java.text.NumberFormat;
import java.util.Locale;
public class NumberFormatTest {
  public static void main(String[] args) {
    NumberFormat nf = NumberFormat.getInstance(Locale.US);
    System.out.println(nf.getClass().getName());
    System.out.println(nf.format(123445));
  }
}
