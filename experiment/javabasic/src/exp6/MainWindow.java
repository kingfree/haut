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

import javax.swing.DefaultListModel;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.UIManager;
import javax.swing.plaf.FontUIResource;

/**
 * 主窗口
 * 
 * @version 2015-6-6
 * @author Kingfree
 */
public class MainWindow implements ItemListener {
    private static Connection conn = null;
    JFrame frame;
    DefaultListModel<Student> model = new DefaultListModel<>();

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
        frame.setSize(300, 200);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null); // 居中
        frame.setLayout(new FlowLayout());

        JLabel label1 = new JLabel("班级：");
        frame.add(label1);

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
        }

        JList<Student> list = new JList<>(model);
        JScrollPane listPane = new JScrollPane(list);
        frame.add(listPane);

        changeList(classes.elementAt(0));
        frame.setVisible(true);
    }

    private void changeList(Classe cls) {
        String sql = "SELECT * FROM students WHERE class_id = " + cls.getId();
        try {
            model.clear();
            DBUtils.queryBeanList(conn, sql, new IResultSetCall<String>() {
                public String invoke(ResultSet rs) throws SQLException {
                    Student stu = new Student(rs.getInt("id"), rs
                            .getString("name"), rs.getInt("os"), rs
                            .getInt("math"), rs.getInt("java"));
                    model.addElement(stu);
                    return stu.toString();
                }
            });
        } catch (Exception e) {
        }
    }

    public void itemStateChanged(ItemEvent ie) {
        if (ie.getStateChange() == ItemEvent.SELECTED) {
            this.changeList((Classe) ie.getItem());
        }
    }

    private static void init() {
        try {
            conn = DBUtils.openConnection();

            List<String> sql = new ArrayList<String>();

            sql.add("CREATE TABLE IF NOT EXISTS students(id INTEGER PRIMARY KEY, class_id INTEGER"
                    + ", name STRING, os INTEGER, math INTEGER, java INTEGER)");
            sql.add("CREATE TABLE IF NOT EXISTS classes(id INTEGER PRIMARY KEY, name STRING)");

            DBUtils.executeAsBatch(conn, sql);
        } catch (SQLException e) {
        }
    }
}
