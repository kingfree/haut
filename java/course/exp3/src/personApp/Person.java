package personApp;

/**
 * @version 2015-3-18
 * @author Kingfree
 */
public class Person {

    private String name;
    private int    age;
    private char   sex;

    public void setName(String name) {
        this.name = name;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public void setSex(char sex) {
        this.sex = sex;
    }

    public String getName() {
        return this.name;
    }

    public int getAge() {
        return this.age;
    }

    public String getSex() {
        if (this.sex == 'f' || this.sex == 'F') {
            return "女";
        } else if (this.sex == 'm' || this.sex == 'M') {
            return "男";
        }
        return "";
    }

    public Person(String name, int age, char sex) {
        setName(name);
        setAge(age);
        setSex(sex);
    }

    public String show() {
        return getName() + " " + getSex() + " " + getAge();
    }

}
