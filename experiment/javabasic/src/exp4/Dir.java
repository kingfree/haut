package exp4;

import java.io.IOException;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.text.DecimalFormat;
import java.util.Stack;

/**
 * 目录递归显示大小
 * 
 * @version 2015-4-21
 * @author Kingfree
 */
public class Dir {

    /**
     * 将字节大小转换成人类可读的大小表示
     * 
     * @param size 字节大小
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
     * @param args 命令行参数，表示要搜索的目录
     */
    public static void main(String[] args) {
        String path = args.length < 1 ? "." : args[0];
        Path dir = Paths.get(path);
        try {
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

                public FileVisitResult postVisitDirectory(Path dir,
                        IOException exc) throws IOException {
                    long size = fileSizes.pop();
                    System.out.printf("[%6s] %s\n", humanSize(size),
                            dir.normalize());
                    if (!fileSizes.isEmpty())
                        fileSizes.push(fileSizes.pop() + size);
                    return FileVisitResult.CONTINUE;
                }

                public FileVisitResult visitFileFailed(Path file,
                        IOException exc) throws IOException {
                    return FileVisitResult.CONTINUE;
                }
            });
        } catch (IOException e) {
            System.out.println("目录遍历失败");
        }
    }

}
