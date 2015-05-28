package exp6;

import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.Vector;

import javax.swing.ComboBoxModel;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.UIManager;
import javax.swing.plaf.FontUIResource;

/**
 * 主窗口
 * 
 * @version 2015-3-27
 * @author Kingfree
 */
public class MainWindow implements ItemListener {
    private static Connection conn = null;
    JFrame frame;

    private static void InitGlobalFont(Font font) {
        FontUIResource fontRes = new FontUIResource(font);
        for (Enumeration<Object> keys = UIManager.getDefaults().keys(); keys
                .hasMoreElements();) {
            Object key = keys.nextElement();
            Object value = UIManager.get(key);
            if (value instanceof FontUIResource) {
                UIManager.put(key, fontRes);
            }
        }
    }

    MainWindow() {

        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            InitGlobalFont(new Font("微软雅黑", Font.PLAIN, 12));
        } catch (Exception e) {
        }

        init();

        frame = new JFrame("学生信息管理系统");
        frame.setSize(400, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null); // 居中
        frame.setLayout(new FlowLayout());

        JLabel label = new JLabel("班级：");
        frame.add(label);

        JTextArea textarea = new JTextArea();
        Vector<Classe> classes = new Vector<>();

        String sql = new String("SELECT * FROM classes");
        try {
            DBUtils.queryBeanList(conn, sql, new IResultSetCall<Classe>() {
                public Classe invoke(ResultSet rs) throws SQLException {
                    Classe cls = new Classe(rs.getInt("id"), rs
                            .getString("name"));
                    classes.add(cls);
                    return cls;
                }
            });
            JComboBox<Classe> combobox = new JComboBox<>(classes);
            combobox.addItemListener(this);
            frame.add(combobox);
        } catch (Exception e) {
            e.printStackTrace();
        }

        frame.add(textarea);

        frame.setVisible(true);
    }

    public void itemStateChanged(ItemEvent e) {
        if (e.getStateChange() == ItemEvent.SELECTED) {
            Classe cls = (Classe) ((JComboBox<Classe>) e.getSource()).getSelectedItem();
            Vector<Student> students = new Vector<>();
            String sql = new String("SELECT * FROM students WHERE class_id = " + cls.getId());
            try {
                DBUtils.queryBeanList(conn, sql, new IResultSetCall<String>() {
                    public String invoke(ResultSet rs) throws SQLException {
                        Student stu = new Student(rs.getInt("id"), rs
                                .getString("name"), rs.getInt("os"), rs
                                .getInt("math"), rs.getInt("java"));
                        students.add(stu);
                        return stu.toString();
                    }
                });
                JList<Student> list = new JList<>(students);
                JScrollPane listPane = new JScrollPane(list);
                frame.add(listPane);
            } catch (Exception e1) {
                e1.printStackTrace();
            }
        }
    }

    private static void init() {
        try {
            conn = DBUtils.openConnection();

            List<String> sql = new ArrayList<String>();

            // sql.add("DROP TABLE IF EXISTS students");
            sql.add("CREATE TABLE IF NOT EXISTS students(id INTEGER PRIMARY KEY, class_id INTEGER"
                    + ", name STRING, os INTEGER, math INTEGER, java INTEGER)");

            // sql.add("INSERT INTO students VALUES(1, 1, '张三', 90, 90, 90)");
            // sql.add("INSERT INTO students VALUES(4, 1, '小明', 89, 72, 88)");
            // sql.add("INSERT INTO students VALUES(2, 2, '李四', 80, 70, 91)");
            // sql.add("INSERT INTO students VALUES(3, 3, '王五', 60, 67, 70)");

            // sql.add("DROP TABLE IF EXISTS classes");
            sql.add("CREATE TABLE IF NOT EXISTS classes(id INTEGER PRIMARY KEY, name STRING)");

            // sql.add("INSERT INTO classes VALUES(1, '软件1301')");
            // sql.add("INSERT INTO classes VALUES(2, '软件1302')");
            // sql.add("INSERT INTO classes VALUES(3, '软件1303')");
            // sql.add("INSERT INTO classes VALUES(4, '软件1304')");
            // sql.add("INSERT INTO classes VALUES(5, '软件1305')");

            DBUtils.executeAsBatch(conn, sql);
        } catch (SQLException e) {
            System.err.println(e.getMessage());
        }
    }
}
