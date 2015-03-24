package reader;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * @date 2015-3-24
 * @author Kingfree
 */
public class ReaderTest {

	/**
	 * 读入一个文本文件并输出
	 * 
	 * @param args
	 *            参数为文件名
	 */
	public static void main(String[] args) throws IOException {

		if (args.length < 1) {
			System.out.println("用法: java reader.ReaderTest <文件名> [编码]");
			return;
		}

		String encoding = args.length > 1 ? args[1] : "UTF-8";

		try (InputStreamReader in = new InputStreamReader(new FileInputStream(
				args[0]), encoding)) {
			int c;
			while ((c = in.read()) != -1) {
				System.out.print((char) c);
			}
		} catch (IOException e) {
			System.out.println("输入输出错误: " + e);
		}

	}

}
