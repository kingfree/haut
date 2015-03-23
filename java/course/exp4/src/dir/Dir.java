package dir;

import java.io.*;
import java.nio.file.*;
import java.nio.file.attribute.*;
import java.text.*;
import java.util.Stack;

/**
 * 目录递归显示大小
 * 
 * @date 2015-3-22
 * @version 2015-3-23
 * @author Kingfree
 */
public class Dir {

	/**
	 * 将字节大小转换成人类可读的大小表示
	 * 
	 * @param size
	 *            字节大小
	 * @return 可读的字符串
	 */
	public static String humanSize(long size) {
		if (size <= 0)
			return "0";
		final String[] units = new String[] { "B", "K", "M", "G", "T" };
		int n = (int) (Math.log10(size) / Math.log10(1024));
		return new DecimalFormat("#0.#").format(size / Math.pow(1024, n))
				+ units[n];
	}

	/**
	 * 类似于 Linux 的 <code>du -a</code> 功能
	 * 
	 * @param args
	 *            命令行参数，表示要搜索的目录
	 * @throws IOException 访问文件失败抛出 I/O 异常
	 */
	public static void main(String[] args) throws IOException {
		String path = args.length < 1 ? "." : args[0];
		Path dir = Paths.get(path);
		Files.walkFileTree(dir, new SimpleFileVisitor<Path>() {

			private Stack<Long> fileSizes = new Stack<Long>();

			public FileVisitResult visitFile(Path file,
					BasicFileAttributes attrs) throws IOException {
				fileSizes.push(fileSizes.pop() + attrs.size());
				System.out.printf(" %6s  %s\n", humanSize(attrs.size()),
						file.normalize());
				return FileVisitResult.CONTINUE;
			}

			public FileVisitResult preVisitDirectory(Path dir,
					BasicFileAttributes attrs) throws IOException {
				fileSizes.push(Long.valueOf(0));
				return FileVisitResult.CONTINUE;
			}

			public FileVisitResult postVisitDirectory(Path dir, IOException exc)
					throws IOException {
				long size = fileSizes.pop();
				System.out.printf("[%6s] %s\n", humanSize(size),
						dir.normalize());
				if (!fileSizes.isEmpty())
					fileSizes.push(fileSizes.pop() + size);
				return FileVisitResult.CONTINUE;
			}

			public FileVisitResult visitFileFailed(Path file, IOException exc)
					throws IOException {
				return FileVisitResult.CONTINUE;
			}
		});
	}

}
