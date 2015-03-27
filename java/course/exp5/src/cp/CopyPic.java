package cp;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * 用 Stream 拷贝较大的文件如图片
 * 
 * @version 2015-3-27
 * @author Kingfree
 */
public class CopyPic {

	public static void main(String[] args) {

		if (args.length < 2) {
			System.out.println("用法: java cp.CopyPic <来源文件> <目的文件>");
			return;
		}

		try {
			FileInputStream in = new FileInputStream(args[0]);
			FileOutputStream out = new FileOutputStream(args[1]);
			// 利用读入字节数组的办法可以加快速度
			byte[] b = new byte[in.available()];
			while (in.read(b) != -1) {
				out.write(b);
			}
			in.close();
			out.close();
		} catch (FileNotFoundException e) {
			System.err.println("错误: 找不到文件！");
		} catch (SecurityException e) {
			System.err.println("错误: 文件无法访问！");
		} catch (IOException e) {
			System.err.println("错误: 输入输出发生异常！");
		}
	}

}
