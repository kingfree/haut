package exp6;

import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Insets;
import java.util.Enumeration;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JLayeredPane;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.UIManager;
import javax.swing.plaf.FontUIResource;

/**
 * 主窗口
 * 
 * @version 2015-6-10
 * @author Kingfree
 */
public class MainWindow extends JFrame {

    private static final long serialVersionUID = 1L;
    MainWindow 主窗口;

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

    @Override
    public Insets getInsets() {
        Insets squeeze = new Insets(40, 20, 20, 20); // 上左下右
        return squeeze;
    }

    MainWindow() {
        主窗口 = this;

        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            InitGlobalFont(new Font("微软雅黑", Font.PLAIN, 12));
        } catch (Exception e) {
        }

        主窗口.setTitle("学生信息管理系统");
        主窗口.setSize(190, 400);
        主窗口.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        主窗口.setLocationRelativeTo(null); // 居中
        主窗口.setLayout(new GridLayout(10, 1, 10, 10));

        主窗口.add(new JLabel("<html><h2>学生信息管理系统</h2></html>"));

        JButton 班级和学生 = new JButton("班级和学生");
        班级和学生.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                new ClassWindow();
            }
        });
        主窗口.add(班级和学生);

        JButton 全部学生 = new JButton("列出全部学生");
        全部学生.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                new StudentWindow();
            }
        });
        主窗口.add(全部学生);

        JButton 按姓名查找学生 = new JButton("按姓名查找学生");
        按姓名查找学生.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                String name = JOptionPane.showInputDialog(null, "请输入姓名:", "查找",
                        JOptionPane.QUESTION_MESSAGE);
                if (name != null) {
                    new StudentWindow(Student.selectByName(name));
                }
            }
        });
        主窗口.add(按姓名查找学生);

        JButton 学生排序 = new JButton("按成绩对学生排序");
        学生排序.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                String[] 选项 = { "数学", "Java", "操作系统" };
                String[] 对应 = { "math", "java", "os" };
                int sel = JOptionPane.showOptionDialog(null, "请选择排序基准:",
                        "按成绩排序", JOptionPane.DEFAULT_OPTION,
                        JOptionPane.QUESTION_MESSAGE, null, 选项, 选项[0]);
                if (sel >= 0 && sel < 3) {
                    new StudentWindow(Student.selectOrberBy(对应[sel]));
                }
            }
        });
        主窗口.add(学生排序);

        JButton 添加学生 = new JButton("添加学生");
        添加学生.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                new StudentAddWindow();
            }
        });
        // 主窗口.add(添加学生);

        JButton 删除学生 = new JButton("删除学生");
        删除学生.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                int id = 0;
                try {
                    id = Integer.parseInt(JOptionPane.showInputDialog(null,
                            "请输入学号:"));
                } catch (Exception ee) {
                    return;
                }
                Student you = Student.selectById(id);
                if (you == null) {
                    JOptionPane.showMessageDialog(null, "没有找到学号为'" + id
                            + "'的学生！", "错误", JOptionPane.ERROR_MESSAGE);
                    return;
                }
                int sel = JOptionPane.showConfirmDialog(null, "你确定删除学生 '" + you
                        + "' 吗？", "确认删除", JOptionPane.YES_NO_OPTION);
                if (sel == JOptionPane.YES_OPTION) {
                    if (Student.deleteById(id)) {
                        JOptionPane.showMessageDialog(null, "删除成功！", "删除",
                                JOptionPane.INFORMATION_MESSAGE);
                    } else {
                        JOptionPane.showMessageDialog(null, "删除失败！", "错误",
                                JOptionPane.ERROR_MESSAGE);
                    }
                }
            }
        });
        主窗口.add(删除学生);

        JButton 关于 = new JButton("关于");
        关于.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                JOptionPane.showMessageDialog(null,
                        "<html><h1>关于</h1><p>作者: 田劲锋</p></html>", "关于",
                        JOptionPane.INFORMATION_MESSAGE);
            }
        });
        主窗口.add(关于);

        主窗口.setVisible(true);
    }
}
