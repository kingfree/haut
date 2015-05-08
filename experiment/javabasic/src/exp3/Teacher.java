package exp3;

/**
 * @version 2015-4-30
 * @author Kingfree
 */
public class Teacher extends Person {

    private String course;

    public void setCourse(String course) {
        this.course = course;
    }

    public String getCourse() {
        return this.course;
    }

    public Teacher(String name, int age, char sex, String course) throws Exception {
        super(name, age, sex);
        setCourse(course);
    }

    public String show() {
        return super.show() + " " + getCourse();
    }

}
