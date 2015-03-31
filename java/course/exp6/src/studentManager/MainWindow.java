package studentManager;

import java.awt.FlowLayout;
import java.awt.Font;

import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;

/**
 * 主窗口
 * 
 * @version 2015-3-27
 * @author Kingfree
 */
public class MainWindow {
	MainWindow() {
		JFrame frame = new JFrame("学生信息管理系统");
		frame.setSize(800, 600);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLayout(new FlowLayout());

		JLabel label = new JLabel("班级：");
		frame.add(label);

		String[] classes = { "软件1301班", "软件1302班", "软件1303班", "软件1304班",
				"软件1305班" };
		JComboBox<String> combobox = new JComboBox<String>(classes);
		frame.add(combobox);

		WindowFeature.modifyWindow(frame, new Font("微软雅黑", Font.PLAIN, 12));
		frame.setVisible(true);
	}
}
