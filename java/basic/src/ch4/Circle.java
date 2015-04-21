package ch4;

/**
 * @version 2015-4-2
 * @author Kingfree
 */
public class Circle {
    public static void main(String[] args) {
        double r;
        try {
            r = Double.parseDouble(args[0]);
        } catch (Exception e) {
            System.out.println("用法: java prob12.Circle <圆的半径>");
            return;
        }

        Circle c = new Circle(r);
        System.out.println(c);
        System.out.println("周长=" + c.perimeter());
    }

    private double radius;

    public Circle(double radius) {
        setRadius(radius);
    }

    public double getRadius() {
        return radius;
    }

    public double perimeter() {
        return 2 * Math.PI * getRadius();
    }

    public void setRadius(double radius) {
        this.radius = Math.abs(radius);
    }

    @Override
    public String toString() {
        return "圆[半径=" + radius + "]";
    }

}
