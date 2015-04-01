package ssys;

import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.Window;

import javax.swing.SwingUtilities;
import javax.swing.UIManager;

/**
 * 设置窗口的外观<br>
 * 在完成顶级窗口组装之后，再调用此功能。<br>
 * <a href="http://www.oschina.net/code/snippet_2268249_45028">
 * http://www.oschina.net/code/snippet_2268249_45028</a>
 */
public class WindowFeature {

	/**
	 * 传入组件和字体，完成三项修改<br>
	 * 如果字体为null，则不改字体
	 * 
	 * @param window
	 *            窗口
	 * @param font
	 *            字体
	 */
	public static void modifyWindow(Window window, Font font) {
		if (font != null)
			modifyFont(window, font);
		setLAF(window);
		window.setLocationRelativeTo(null); // 居中，感谢0x0001指点
	}

	/**
	 * 控制台输出所有已安装的LAF
	 */
	public static void printInstalledLAF() {
		for (UIManager.LookAndFeelInfo info : UIManager
				.getInstalledLookAndFeels()) {
			System.out.println("●" + info.getName() + ":\n" + info);
		}
	}

	/**
	 * 修改字体，适用任意Component
	 * 
	 * @param window
	 *            窗口
	 * @param font
	 *            字体
	 */
	private static void modifyFont(Component window, Font font) {
		window.setFont(font);
		if (window instanceof Container) {
			Container container = (Container) window;
			for (Component c : container.getComponents()) {
				modifyFont(c, font);
			}
		}
	}

	private static void setLAF(Window window) {
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName()); // 当前系统风格
			SwingUtilities.updateComponentTreeUI(window);
		} catch (Exception e) {
		}
	}

}