package exp6;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.List;

import javax.swing.DefaultComboBoxModel;
import javax.swing.DefaultListModel;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

public class ClassWindow extends JFrame {

    private static final long serialVersionUID = 1L;
    private ClassWindow 主窗口 = null;
    private JPanel 主面板 = null;
    DefaultListModel<Student> 学生模型 = new DefaultListModel<>();
    DefaultComboBoxModel<Classe> 班级模型 = new DefaultComboBoxModel<>();
    JComboBox<Classe> 班级下拉框 = null;
    JList<Student> 学生列表框 = null;

    ClassWindow() {
        super();
        初始化();
        主窗口 = this;
    }

    private void 初始化() {
        this.setTitle("班级和学生");
        this.setSize(300, 260);
        this.setContentPane(取主面板());
        this.setLocationRelativeTo(null); // 居中
        this.setVisible(true);
    }

    private JPanel 取主面板() {
        if (主面板 == null) {
            主面板 = new JPanel();
            
            JPanel 班级面板 = new JPanel();
            班级面板.add(new JLabel("班级:"));

            List<Classe> classes = Classe.selectAll();
            for (Classe cls : classes) {
                班级模型.addElement(cls);
            }

            班级下拉框 = new JComboBox<Classe>(班级模型);
            班级下拉框.addItemListener(new ItemListener() {
                public void itemStateChanged(ItemEvent ie) {
                    if (ie.getStateChange() == ItemEvent.SELECTED) {
                        主窗口.changeList((Classe) ie.getItem());
                    }
                }
            });
            班级面板.add(班级下拉框);

            JPanel 学生面板 = new JPanel();
            学生面板.add(new JLabel("学生:"));
            学生列表框 = new JList<>(学生模型); 
            
            JScrollPane 列表框面板 = new JScrollPane(学生列表框);
            学生面板.add(列表框面板);

            主面板.add(班级面板, BorderLayout.NORTH);
            主面板.add(学生面板, BorderLayout.SOUTH);

            if (classes != null && !classes.isEmpty()) {
                changeList(classes.get(0));
            }
        }
        return 主面板;
    }

    private void changeList(Classe cls) {
        try {
            学生模型.clear();
            for (Student stu : Student.selectByClass(cls)) {
                学生模型.addElement(stu);
            }
        } catch (Exception e) {
        }
    }

}
