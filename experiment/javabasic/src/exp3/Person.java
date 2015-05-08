package exp3;

/**
 * @version 2015-4-30
 * @author Kingfree
 */
public class Person {

    private String name;
    private int    age;
    private char   sex;

    public void setName(String name) {
        this.name = name;
    }

    public void setAge(int age) throws Exception {
    	if (age < 0 || age > 180) {
    		throw new Exception("年龄不正确");
    	}
        this.age = age;
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

    public Person(String name, int age, char sex) throws Exception {
        setName(name);
        setAge(age);
        setSex(sex);
    }

    public String show() {
        return getName() + " " + getSex() + " " + getAge();
    }

}
