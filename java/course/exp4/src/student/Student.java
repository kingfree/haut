package student;

/**
 * @date 2015-3-18
 * @version 2015-3-26
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

	public Student(int age, String name, int id) {
		super(name, age);
		setId(id);
	}

	public String toString() {
		return "学生[学号=" + getId() + ",姓名=" + getName() + ",年龄=" + getAge()
				+ "]";
	}

}
