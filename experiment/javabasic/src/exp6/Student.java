package exp6;

import java.io.Serializable;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * 学生类
 * 
 * @version 2015-6-10
 * @author Kingfree
 */
public class Student implements Serializable {

    private static final long serialVersionUID = 1L;

    private int id;
    private String name;
    private int os, math, java;
    private int classId;

    public Student() {

    }

    public Student(int id, int classId, String name, int os, int math, int java) {
        setId(id);
        setClassId(classId);
        setName(name);
        setOs(os);
        setMath(math);
        setJava(java);
    }

    private static int parseScore(int score) {
        return Math.abs(score) % 100;
    }

    public int getOs() {
        return os;
    }

    public void setOs(int os) {
        this.os = parseScore(os);
    }

    public int getMath() {
        return math;
    }

    public void setMath(int math) {
        this.math = parseScore(math);
    }

    public int getJava() {
        return java;
    }

    public void setJava(int java) {
        this.java = parseScore(java);
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

    private static Connection conn = null;

    static {
        try {
            conn = DBUtils.openConnection();

            List<String> sql = new ArrayList<String>();

            // sql.add("DROP TABLE IF EXISTS students");
            sql.add("CREATE TABLE IF NOT EXISTS students(id INTEGER PRIMARY KEY, class_id INTEGER"
                    + ", name STRING, os INTEGER, math INTEGER, java INTEGER)");

            // sql.add("INSERT INTO students VALUES(1, 1, '水树', 90, 90, 90)");
            // sql.add("INSERT INTO students VALUES(2, 1, '田村', 89, 72, 88)");
            // sql.add("INSERT INTO students VALUES(3, 2, '堀江', 80, 70, 91)");
            // sql.add("INSERT INTO students VALUES(4, 3, '小仓', 78, 67, 70)");
            // sql.add("INSERT INTO students VALUES(5, 5, '东山', 60, 88, 70)");
            // sql.add("INSERT INTO students VALUES(6, 5, '种田', 77, 89, 70)");
            // sql.add("INSERT INTO students VALUES(7, 4, '钉宫', 98, 78, 83)");
            // sql.add("INSERT INTO students VALUES(8, 4, '喜多村', 84, 67, 71)");
            // sql.add("INSERT INTO students VALUES(9, 4, '阿澄', 87, 68, 92)");
            // sql.add("INSERT INTO students VALUES(10, 4, '花泽', 69, 67, 82)");

            DBUtils.executeAsBatch(conn, sql);
        } catch (SQLException e) {
        }
    }

    public static List<Student> selectAll() {
        String sql = "SELECT * FROM students";
        try {
            List<Student> you = DBUtils.queryBeanList(conn, sql,
                    new StudentIResultSetCall());
            return you;
        } catch (Exception e) {
            return null;
        }
    }

    public static List<Student> selectByClass(Classe cls) {
        String sql = "SELECT * FROM students WHERE class_id = ?";
        try {
            List<Student> you = DBUtils.queryBeanList(conn, sql, Student.class,
                    cls.getId());
            return you;
        } catch (Exception e) {
            return null;
        }
    }

    public static Student selectById(int id) {
        String sql = "SELECT * FROM students WHERE id = ?";
        try {
            Student you = DBUtils.queryBean(conn, sql, Student.class, id);
            return you;
        } catch (Exception e) {
            return null;
        }
    }

    public static List<Student> selectByName(String name) {
        String sql = "SELECT * FROM students WHERE name LIKE ?";
        try {
            List<Student> you = DBUtils.queryBeanList(conn, sql,
                    new StudentIResultSetCall(), "%" + name + "%");
            return you;
        } catch (Exception e) {
            return null;
        }
    }

    public static boolean deleteById(int id) {
        String sql = "DELETE FROM students WHERE id = ?";
        try {
            int res = DBUtils.execute(conn, sql, id);
            if (res == 1) {
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    public static List<Student> selectOrberBy(String col) {
        String sql = new String("SELECT * FROM students ORDER BY `" + col
                + "` DESC");
        try {
            List<Student> you = DBUtils.queryBeanList(conn, sql,
                    new StudentIResultSetCall());
            return you;
        } catch (Exception e) {
            return null;
        }
    }

    public static boolean insert(Student stu) {
        String sql = "INSERT INTO students (id, class_id, name, os, math, java) VALUES (?, ?, ?, ?, ?, ?)";
        try {
            int res = DBUtils.execute(conn, sql, stu.getId(), stu.getClassId(),
                    stu.getName(), stu.getOs(), stu.getMath(), stu.getJava());
            if (res == 1) {
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
    }

    public int getClassId() {
        return classId;
    }

    public void setClassId(int classId) {
        this.classId = classId;
    }
}

class StudentIResultSetCall implements IResultSetCall<Student> {
    public Student invoke(ResultSet rs) throws SQLException {
        Student e = new Student(rs.getInt("id"), rs.getInt("class_id"),
                rs.getString("name"), rs.getInt("os"), rs.getInt("math"),
                rs.getInt("java"));
        return e;
    }
}
