/**
 * 
 */
package dir;

import java.io.*;
import java.nio.file.*;
import java.nio.file.attribute.*;
import java.text.*;

/**
 * @date 2015-3-22
 * @author Kingfree
 */
public class Dir {

    public static String humanSize(long size) {
        if (size <= 0)
            return "0";
        final String[] units = new String[] { "B", "kB", "MB", "GB", "TB" };
        int n = (int) (Math.log10(size) / Math.log10(1024));
        return new DecimalFormat("#,##0.#").format(size / Math.pow(1024, n))
                + " " + units[n];
    }

    /**
     * @param args
     */
    public static void main(String[] args) throws IOException {
        String path = args.length < 1 ? ".." : args[0];
        Path dir = Paths.get(path);
        Files.walkFileTree(dir, new SimpleFileVisitor<Path>() {

            private long fileSize = 0;

            public FileVisitResult visitFile(Path file,
                    BasicFileAttributes attrs) throws IOException {
                fileSize += attrs.size();
                System.out.printf("      %8s %s\n", humanSize(attrs.size()),
                        file.normalize());
                return FileVisitResult.CONTINUE;
            }

            public FileVisitResult preVisitDirectory(Path dir,
                    BasicFileAttributes attrs) throws IOException {
                fileSize = 0;
                return FileVisitResult.CONTINUE;
            }

            public FileVisitResult postVisitDirectory(Path dir, IOException exc)
                    throws IOException {
                System.out.printf("<DIR> %8s %s\n", humanSize(fileSize),
                        dir.normalize());
                return FileVisitResult.CONTINUE;
            }

            public FileVisitResult visitFileFailed(Path file, IOException exc)
                    throws IOException {
                return FileVisitResult.CONTINUE;
            }
        });
    }

}
