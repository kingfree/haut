package studentManager;

import javax.swing.SwingUtilities;

/**
 * 学生信息管理系统
 * 
 * @version 2015-3-27
 * @author Kingfree
 */
public class StudentManager {

	public static void cli() {
		System.out.println("学生信息管理系统");
	}

	public static void test() {
		System.out.println("Test");
	}

	public static void gui() {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				new MainWindow();
			}
		});
	}

	/**
	 * 系统启动入口
	 * 
	 * @param args
	 *            指定启动方式
	 */
	public static void main(String[] args) {
		String arg = "-gui";
		if (args.length > 0) {
			arg = args[0].toLowerCase();
		}
		switch (arg) {
		case "-gui":
			gui();
			break;
		case "-cli":
			cli();
			break;
		case "-test":
			test();
			break;
		default:
			System.out.println("用法: java studentManager.StudentManager [参数]");
			System.out.println("参数: \t-gui \t图形界面（默认）");
			System.out.println("     \t-cli \t命令行界面");
			System.out.println("     \t-test \t测试模式");
		}
	}

}
