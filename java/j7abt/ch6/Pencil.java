class Tool {
  public String toString() {
    return "通用工具";
  }
}

public class Pencil extends Tool {
  public String toString() {
    return "我是一支铅笔";
  }

  public void write() {
    System.out.println(super.toString());
    System.out.println(toString());
  }

  public static void main(String[] args) {
    Pencil pencil = new Pencil();
    pencil.write();
  }
}
