package shape;

/**
 * 圆
 * @version 2015-3-18
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
     * @see shape.Shape#area()
     */
    @Override
    public double area() {
        return Math.PI * Math.pow(getRadius(), 2);
    }

    /**
     * @see shape.Shape#perimeter()
     */
    @Override
    public double perimeter() {
        return 2 * Math.PI * getRadius();
    }

    /**
     * @see shape.Shape#info()
     */
    @Override
    public String info() {
        return "圆[半径=" + getRadius() + "]";
    }

}
