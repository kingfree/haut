class Parent {
  public Parent(String s) {
    System.out.println("Parent(String)");
  }
}

public class Child extends Parent {
  public Child() {
    super(null);
  }
}
