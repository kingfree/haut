package exp3;

/**
 * 矩形类
 * 
 * @version 2015-3-19
 * @author Kingfree
 */
public class Rectangle implements Shape {

    private double height;
    private double width;

    public void setHeight(double height) {
        this.height = height;
    }

    public void setWidth(double width) {
        this.width = width;
    }

    public double getHeight() {
        return this.height;
    }

    public double getWidth() {
        return this.width;
    }

    public Rectangle(double height, double width) {
        setHeight(height);
        setWidth(width);
    }

    /**
     * @see exp3.Shape#area()
     */
    @Override
    public double area() {
        return getHeight() * getWidth();
    }

    /**
     * @see exp3.Shape#perimeter()
     */
    @Override
    public double perimeter() {
        return 2 * (getHeight() + getWidth());
    }

    public String toString() {
        return "矩形[长=" + getHeight() + ",宽=" + getWidth() + "]";
    }

}
