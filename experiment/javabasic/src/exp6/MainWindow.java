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
import javax.swing.JButton;
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
 * @version 2015-6-10
 * @author Kingfree
 */
public class MainWindow {
    JFrame 主窗口;

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
        
        主窗口 = new JFrame("学生信息管理系统");
        主窗口.setSize(200, 400);
        主窗口.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        主窗口.setLocationRelativeTo(null); // 居中
        主窗口.setLayout(new FlowLayout());

        JButton 班级和学生 = new JButton("班级和学生");
        班级和学生.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                new ClassWindow();
            }
        });
        
        主窗口.add(班级和学生);
        
        主窗口.setVisible(true);
    }

}
