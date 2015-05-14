package exp6;

import java.io.Serializable;

/**
 * 学生类
 * 
 * @version 2015-5-14
 * @author Kingfree
 */
public class Student implements Serializable {

    private static final long serialVersionUID = 1L;

    private int id;
    private String name;
    private int os, math, java;

    public Student() {

    }

    public Student(int id, String name, int os, int math, int java) {
        setId(id);
        setName(name);
        setOs(os);
        setMath(math);
        setJava(java);
    }

    public int getOs() {
        return os;
    }

    public void setOs(int os) {
        this.os = os;
    }

    public int getMath() {
        return math;
    }

    public void setMath(int math) {
        this.math = math;
    }

    public int getJava() {
        return java;
    }

    public void setJava(int java) {
        this.java = java;
    }

    private void setId(int id) {
        this.id = id;
    }

    private void setName(String name) {
        this.name = name;
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    @Override
    public String toString() {
        return getName() + " " + getId() + " os:" + getOs() + " math:"
                + getMath() + " java:" + getJava();
    }
}
