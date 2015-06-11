package exp6;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.List;

import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class StudentAddWindow extends JFrame {

    private static final long serialVersionUID = 1L;
    private StudentAddWindow 主窗口 = null;
    private JPanel 主面板 = null;
    DefaultListModel<Student> 学生模型 = new DefaultListModel<>();
    DefaultComboBoxModel<Classe> 班级模型 = new DefaultComboBoxModel<>();
    JComboBox<Classe> 班级下拉框 = null;

    StudentAddWindow() {
        super();
        初始化();
        主窗口 = this;
    }

    private void 初始化() {
        this.setTitle("添加学生");
        // this.setSize(300, 400);
        this.setContentPane(取主面板());
        this.pack();
        this.setLocationRelativeTo(null); // 居中
        this.setVisible(true);
    }

    @Override
    public Insets getInsets() {
        Insets squeeze = new Insets(40, 20, 20, 20); // 上左下右
        return squeeze;
    }

    private JPanel 取主面板() {
        if (主面板 == null) {
            主面板 = new JPanel();
            主面板.setLayout(new GridLayout(7, 2, 10, 10));

            JLabel 学号标签 = new JLabel("学号: ");
            JTextField 学号文本框 = new JTextField("" + (Student.getMaxId() + 1));
            主面板.add(学号标签);
            主面板.add(学号文本框);

            JLabel 姓名标签 = new JLabel("姓名: ");
            JTextField 姓名文本框 = new JTextField();
            主面板.add(姓名标签);
            主面板.add(姓名文本框);

            JLabel 班级标签 = new JLabel("班级: ");
            List<Classe> classes = Classe.selectAll();
            for (Classe cls : classes) {
                班级模型.addElement(cls);
            }
            JComboBox<Classe> 班级下拉框 = new JComboBox<Classe>(班级模型);
            主面板.add(班级标签);
            主面板.add(班级下拉框);

            JLabel 数学标签 = new JLabel("数学成绩: ");
            JTextField 数学文本框 = new JTextField("100");
            主面板.add(数学标签);
            主面板.add(数学文本框);

            JLabel Java标签 = new JLabel("Java成绩: ");
            JTextField Java文本框 = new JTextField("100");
            主面板.add(Java标签);
            主面板.add(Java文本框);

            JLabel OS标签 = new JLabel("操作系统成绩: ");
            JTextField OS文本框 = new JTextField("100");
            主面板.add(OS标签);
            主面板.add(OS文本框);

            JButton 添加按钮 = new JButton("添加");
            添加按钮.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                    try {
                        int id = Integer.parseInt(学号文本框.getText());
                        int cid = ((Classe)班级下拉框.getSelectedItem()).getId();
                        String name = 姓名文本框.getText().trim();
                        int os = Integer.parseInt(OS文本框.getText());
                        int math = Integer.parseInt(数学文本框.getText());
                        int java = Integer.parseInt(Java文本框.getText());
                        Student you = new Student(id, cid, name, os, math, java);
                        if (Student.insert(you)) {
                            JOptionPane.showMessageDialog(null, "添加成功！", "添加",
                                    JOptionPane.INFORMATION_MESSAGE);
                            主窗口.setVisible(false);
                        } else {
                            JOptionPane.showMessageDialog(null, "添加失败！", "错误",
                                    JOptionPane.ERROR_MESSAGE);
                        }
                    } catch (Exception ee) {
                        return;
                    }
                }
            });
            主面板.add(添加按钮);

        }
        return 主面板;
    }

}
