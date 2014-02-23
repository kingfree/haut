public class 字串 {
  public String 到大写(String s) 
      throws NullPointerException, 已大写异常 {
    if (s == null) {
      throw new NullPointerException("Your passed a null argument");
    }
    Character 首 = s.charAt(0);
    if (Character.isUpperCase(首)) {
      throw new 已大写异常();
    }
    String 尾 = s.substring(1);
    return 首.toString().toUpperCase() + 尾;
  }

  public static void main(String[] args) {
    字串 单 = new 字串();
    String 输入 = "Index";
    try {
      String 大写 = 单.到大写(输入);
      System.out.println(大写);
    } catch (NullPointerException e) {
      System.out.println(e.toString());
    } catch (已大写异常 e) {
      e.printStackTrace();
    }

  }

}
