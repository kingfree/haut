package shape;

/**
 * 三角形类，继承于矩形类
 * 
 * @version 2015-3-18
 * @author Kingfree
 */
public class Triangle extends Rectangle {

    private double third;

    public void setThird(double third) {
        this.third = third;
    }

    public double getThird() {
        return this.third;
    }

    public Triangle(double height, double width, double third) {
        super(height, width);
        setThird(third);
    }

    /**
     * @see shape.Shape#area()
     */
    @Override
    public double area() {
        double a = getHeight(), b = getWidth(), c = getThird();
        double p = (a + b + c) / 2;
        double s = p * (p - a) * (p - b) * (p - c);
        return Math.sqrt(s);
    }

    /**
     * @see shape.Shape#perimeter()
     */
    @Override
    public double perimeter() {
        return getHeight() + getWidth() + getThird();
    }

    /**
     * @see shape.Shape#info()
     */
    @Override
    public String info() {
        return "三角形[a=" + getHeight()
                + ",b=" + getWidth()
                + ",c=" + getThird() + "]";
    }

}
