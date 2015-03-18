package shape;

/**
 * Shape 接口
 * 
 * @version 2015-3-18
 * @author Kingfree
 */
public interface Shape {

    /**
     * 返回该图形的属性信息
     * 
     * @return 描述属性的字符串
     */
    String info();

    /**
     * 计算图形的面积
     * 
     * @return 面积
     */
    double area();

    /**
     * 计算图形的周长
     * 
     * @return 周长
     */
    double perimeter();

}
