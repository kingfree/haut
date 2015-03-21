public class WhileTest {
  public static void main(String[] args) {

     java.awt.Toolkit.getDefaultToolkit().beep();
     try {
       Thread.currentThread().sleep(500);
     } catch (Exception e) {
       ;
     }
     java.awt.Toolkit.getDefaultToolkit().beep();
     try {
       Thread.currentThread().sleep(500);
     } catch (Exception e) {
       ;
     }
     java.awt.Toolkit.getDefaultToolkit().beep();

     int i = 0;
     while (i < 3) {
       System.out.println(i++);
     }

     int j = 0;
     while (j < 3) {
       java.awt.Toolkit.getDefaultToolkit().beep();
       try {
         Thread.currentThread().sleep(500);
       } catch (Exception e) {
         ;
       }
       j++;
     }

     int k = 0;
     while (true) {
       System.out.println(k++);
       if (k > 4) {
         break;
       }
     }

  }
}
