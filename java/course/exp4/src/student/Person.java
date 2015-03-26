package student;

/**
 * @date 2015-3-18
 * @version 2015-3-26
 * @author Kingfree
 */
public class Person {

    private String name;
    private int    age;

    public void setName(String name) {
        this.name = name;
    }

    public void setAge(int age) {
        this.age = Math.abs(age) % 180;
    }

    public String getName() {
        return this.name;
    }

    public int getAge() {
        return this.age;
    }

    public Person(String name, int age) {
        setName(name);
        setAge(age);
    }

    public String toString() {
        return getName() + " " + getAge();
    }

}
