package input;

import java.io.FileInputStream;
import java.io.IOException;

/**
 * @date 2015-3-24
 * @author Kingfree
 */
public class InputStreamTest {

	/**
	 * 读入一个文本文件并输出
	 * 
	 * @param args
	 *            参数为文件名
	 */
	public static void main(String[] args) {

		if (args.length < 1) {
			System.out.println("用法: java input.InputStreamTest <文件名>");
			return;
		}

		try (FileInputStream in = new FileInputStream(args[0])) {
			int b;
			while ((b = in.read()) != -1) {
				System.out.write(b);
			}
		} catch (IOException e) {
			System.out.println("输入输出错误: " + e);
		}

	}
}
