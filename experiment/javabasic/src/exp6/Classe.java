package exp6;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class Classe {

    private int id;
    private String name;

    public Classe() {
    }

    @Override
    public String toString() {
        return name;
    }

    public Classe(int id, String name) {
        super();
        this.id = id;
        this.name = name;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    private static Connection conn = null;

    static {
        try {
            conn = DBUtils.openConnection();

            List<String> sql = new ArrayList<String>();

            // sql.add("DROP TABLE IF EXISTS classes");
            sql.add("CREATE TABLE IF NOT EXISTS classes(id INTEGER PRIMARY KEY, name STRING)");

            // sql.add("INSERT INTO classes VALUES(1, '软件1301')");
            // sql.add("INSERT INTO classes VALUES(2, '软件1302')");
            // sql.add("INSERT INTO classes VALUES(3, '软件1303')");
            // sql.add("INSERT INTO classes VALUES(4, '软件1304')");
            // sql.add("INSERT INTO classes VALUES(5, '软件1305')");

            DBUtils.executeAsBatch(conn, sql);
        } catch (SQLException e) {
        }
    }

    public static List<Classe> selectAll() {
        String sql = "SELECT * FROM classes";
        try {
            List<Classe> you = DBUtils.queryBeanList(conn, sql,
                    new ClassIResultSetCall());
            return you;
        } catch (Exception e) {
            return null;
        }
    }

    public static Classe selectById(int id) {
        String sql = "SELECT * FROM classes WHERE id = ?";
        try {
            Classe you = DBUtils.queryBean(conn, sql, Classe.class, id);
            return you;
        } catch (Exception e) {
            return null;
        }
    }
    
}

class ClassIResultSetCall implements IResultSetCall<Classe> {
    public Classe invoke(ResultSet rs) throws SQLException {
        Classe e = new Classe(rs.getInt("id"), rs.getString("name"));
        return e;
    }
}
