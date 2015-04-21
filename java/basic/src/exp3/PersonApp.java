package exp3;

/**
 * (a) 声明一个Person类，有name(String类型)、age(int类型)、sex(char类型)属性。
 *     通过构造方法进行赋值。
 *     一个show方法，返回String类型，内容如下：
 *          某某 男(女) 年龄
 * (b) 声明一个Student类，继承Person类，增加id(int，学号)属性，通过构造方法，利用
 * super调用父类构造方法来进行变量赋值。Override父类的show方法，返回String类型，
 * 内容如下：
 *          某某 男(女) 年龄 学号
 *     提示：利用super调用父类的show方法得到除学号部分的String，然后加上学号的信息。
 * (c) 声明一个Teacher类，继承Person，增加course(String，所教课程)属性，通过构造
 * 方法，利用super调用父类构造方法来进行变量赋值。Override父类的show方法，返回
 * String类型，内容如下：
 *          某某 男(女) 年龄 所教课程
 *     提示：利用super调用父类的show方法得到除所教课程部分的String，然后加上所教
 * 课程的信息。
 * (d) 声明PersonApp类，在其中的main方法中分别声明Person、Student、Teacher类型
 * 的变量，并通过构造方法初始化，然后显示各自的信息。
 * 
 * @version 2015-3-18
 * @author Kingfree
 */
public class PersonApp {

    public static void main(String[] args) {

        Person nao = new Person("東山奈央", 23, 'f');
        System.out.println(nao.show());

        Student yui = new Student("小倉唯", 20, 'f', 19950815);
        System.out.println(yui.show());

        Teacher nana = new Teacher("水樹奈々", 35, 'f', "国文");
        System.out.println(nana.show());

    }

}
