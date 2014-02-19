public class P {
  public void publicMethod() {
  }
  protected void protectedMethod() {
  }
  void defaultMethod() {
  }
}

class C extends P {
  public void testMethods() {
    publicMethod();
    protectedMethod();
    defaultMethod();
  }
  public C(String s) {
    System.out.println(s);
  }
}
