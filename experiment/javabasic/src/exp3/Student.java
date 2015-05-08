package exp3;

/**
 * @version 2015-4-30
 * @author Kingfree
 */
public class Student extends Person {

    private int id;

    public void setId(int id) {
        this.id = id;
    }

    public int getId() {
        return this.id;
    }

    public Student(String name, int age, char sex, int id) throws Exception {
        super(name, age, sex);
        setId(id);
    }

    public String show() {
        return super.show() + " " + getId();
    }

    public String toString() {
        return "学生[学号=" + getId() + ",姓名=" + getName() + ",年龄=" + getAge()
                + "]";
    }

}
