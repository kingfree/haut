package exp6;

import java.awt.BorderLayout;
import java.awt.Insets;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

public class StudentWindow extends JFrame {

    private static final long serialVersionUID = 1L;
    private StudentWindow 主窗口 = null;
    private JPanel 主面板 = null;
    DefaultTableModel 学生模型 = null;
    private List<Student> 学生 = null;

    StudentWindow() {
        super();
        this.学生 = Student.selectAll();
        初始化();
    }

    StudentWindow(List<Student> 学生) {
        super();
        this.学生 = 学生;
        初始化();
    }

    StudentWindow(Student stu) {
        super();
        if (stu != null) {
            this.学生.add(stu);
        }
        初始化();
    }

    private void 初始化() {
        this.setTitle("学生信息");
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
            主面板.setLayout(new BorderLayout());

            String[] 表头 = { "学号", "姓名", "班级", "数学", "Java", "操作系统" };
            学生模型 = new DefaultTableModel(表头, 0);

            for (Student stu : 学生) {
                Object[] a = { stu.getId(), stu.getName(),
                        Classe.selectById(stu.getClassId()).getName(),
                        stu.getMath(), stu.getJava(), stu.getOs() };
                学生模型.addRow(a);
            }
            JTable 学生表格 = new JTable(学生模型);
            学生表格.setEnabled(false);
            主面板.add(学生表格.getTableHeader(), BorderLayout.NORTH);
            主面板.add(学生表格);
        }
        return 主面板;
    }

}
