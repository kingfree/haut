package exp6;

import java.awt.FlowLayout;
import java.awt.Font;
import java.util.Enumeration;

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
 * @version 2015-3-27
 * @author Kingfree
 */
public class MainWindow {
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

        JFrame frame = new JFrame("学生信息管理系统");
        frame.setSize(400, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new FlowLayout());

        JLabel label = new JLabel("班级：");
        frame.add(label);

        String[] classes = { "软件1301班", "软件1302班", "软件1303班", "软件1304班",
                "软件1305班" };
        JComboBox<String> combobox = new JComboBox<String>(classes);
        frame.add(combobox);

        JList<String> list = new JList<>(classes);
        JScrollPane listPane = new JScrollPane(list);
        frame.add(listPane);

        frame.setVisible(true);
    }
}
