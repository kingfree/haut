package cat;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * 将标准输入的内容写出到文件
 * 
 * @date 2015-3-26
 * @version 2015-3-27
 * @author Kingfree
 */
public class CatToFile {

	public static void main(String[] args) {

		if (args.length < 1) {
			System.out.println("用法: java cat.CatToFile <文件名>");
			return;
		}

		try (InputStreamReader in = new InputStreamReader(System.in);
				FileOutputStream out = new FileOutputStream(args[0])) {
			int b;
			while ((b = in.read()) != -1) {
				out.write(b);
			}
		} catch (FileNotFoundException e) {
			System.err.println("错误: 找不到文件！");
		} catch (SecurityException e) {
			System.err.println("错误: 文件无法访问！");
		} catch (IOException e) {
			System.err.println("错误: 输入输出发生异常！");
		}
	}
}
