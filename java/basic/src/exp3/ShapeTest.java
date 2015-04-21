package exp3;

/**
 * 声明一个Shape接口，其中有计算面积(area)、周长(perimeter)的方法，有以下几个实现：
 * Circle(圆)，Rectangle(矩形)，Triangle(三角形)，都有计算面积、周长的方法。
 *
 * @version 2015-3-19
 * @author Kingfree
 */
public class ShapeTest {

    public static void main(String[] args) {

        Circle c = new Circle(1.5);
        System.out.println(c + " 面积: " + c.area() + " 周长: " + c.perimeter());

        Rectangle r = new Rectangle(3, 4);
        System.out.println(r + " 面积: " + r.area() + " 周长: " + r.perimeter());

        Triangle t = new Triangle(3, 4, 5);
        System.out.println(t + " 面积: " + t.area() + " 周长: " + t.perimeter());

    }

}
