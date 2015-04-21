package exp3;

/**
 * 圆
 * 
 * @version 2015-3-19
 * @author Kingfree
 */
public class Circle implements Shape {

    private double radius;

    public void setRadius(double radius) {
        this.radius = radius;
    }

    public double getRadius() {
        return this.radius;
    }

    public Circle(double radius) {
        setRadius(radius);
    }

    /**
     * @see exp3.Shape#area()
     */
    @Override
    public double area() {
        return Math.PI * Math.pow(getRadius(), 2);
    }

    /**
     * @see exp3.Shape#perimeter()
     */
    @Override
    public double perimeter() {
        return 2 * Math.PI * getRadius();
    }

    public String toString() {
        return "圆[半径=" + getRadius() + "]";
    }

}
