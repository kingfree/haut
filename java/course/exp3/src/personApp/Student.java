package personApp;

/**
 * @version 2015-3-18
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

    public Student(String name, int age, char sex, int id) {
        super(name, age, sex);
        setId(id);
    }

    public String show() {
        return super.show() + " " + getId();
    }

}
