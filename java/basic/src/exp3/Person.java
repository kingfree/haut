package exp3;

/**
 * @version 2015-3-26
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
        this.age = Math.abs(age) % 180;
    }

    public void setSex(char sex) {
        this.sex = (sex == 'f' || sex == 'F') ? 'f' : 'm';
    }

    public String getName() {
        return this.name;
    }

    public int getAge() {
        return this.age;
    }

    public String getSex() {
        return this.sex == 'f' ? "女" : "男";
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
