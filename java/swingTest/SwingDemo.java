import java.awt.*;
import javax.swing.*;

class SwingDemo {

	SwingDemo() {
		JFrame frame = new JFrame("简单的 Swing 应用");
		frame.setSize(400, 300);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLayout(new FlowLayout());

		JLabel label = new JLabel("你好，世界！");
		frame.add(label);

		JTextField text = new JTextField("一个输入框", 20);
		frame.add(text);

		JButton button = new JButton("按钮");
		frame.add(button);

		frame.setVisible(true);
	}

	public static void main(String args[]) {

		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (UnsupportedLookAndFeelException e) {
			e.printStackTrace();
		}

		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				new SwingDemo();
			}
		});
	}
}
