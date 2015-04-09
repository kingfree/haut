package ssys;

import java.sql.*;

/**
 * @version 2015-4-1
 * @author Kingfree
 */
public class UnitTest {

    /**
     * @param args
     * @throws ClassNotFoundException
     */
    public static void main(String[] args) throws ClassNotFoundException {
        Class.forName("org.sqlite.JDBC");

        Connection connection = null;
        try {
            connection = DriverManager.getConnection("jdbc:sqlite:sample.db");
            Statement sql = connection.createStatement();
            sql.setQueryTimeout(30);

            sql.executeUpdate("DROP TABLE IF EXISTS students");
            sql.executeUpdate("CREATE TABLE students(id INTEGER PRIMARY KEY, name STRING)");
            sql.executeUpdate("DROP TABLE IF EXISTS subjects");
            sql.executeUpdate("CREATE TABLE subjects (id INTEGER PRIMARY KEY, name STRING"
                    + ", memo STRING)");
            sql.executeUpdate("DROP TABLE IF EXISTS scores");
            sql.executeUpdate("CREATE TABLE scores(" + "student_id INTEGER"
                    + ", subject_id INTEGER" + ", score INTEGER"
                    + ", FOREIGN KEY(student_id) REFERENCES students(id)"
                    + ", FOREIGN KEY(subject_id) REFERENCES subjects(id)" + ")");

            sql.executeUpdate("INSERT INTO subjects VALUES(1, 'os', '操作系统')");
            sql.executeUpdate("INSERT INTO subjects VALUES(2, 'math', '离散数学')");
            sql.executeUpdate("INSERT INTO subjects VALUES(3, 'java', '程序设计')");

            sql.executeUpdate("INSERT INTO students VALUES(12, '张三')");
            sql.executeUpdate("INSERT INTO scores VALUES(12, 1, 90)");
            sql.executeUpdate("INSERT INTO scores VALUES(12, 2, 90)");
            sql.executeUpdate("INSERT INTO scores VALUES(12, 3, 90)");

            ResultSet rs = sql.executeQuery("SELECT"
                    + " students.name, subjects.name as subject_name"
                    + ", subjects.memo, scores.score"
                    + " FROM students, subjects, scores"
                    + " WHERE students.id = scores.student_id"
                    + " AND subjects.id = scores.subject_id");
            while (rs.next()) {
                System.out.printf("%s %s[%s]:%d\n", rs.getString("name"),
                        rs.getString("subject_name"), rs.getString("memo"),
                        rs.getInt("score"));
            }
        } catch (SQLException e) {
            System.err.println(e.getMessage());
        } finally {
            try {
                if (connection != null)
                    connection.close();
            } catch (SQLException e) {
                System.err.println(e);
            }
        }
    }

    public UnitTest() {
        super();
    }

    @Override
    public String toString() {
        return "UnitTest []";
    }

}
