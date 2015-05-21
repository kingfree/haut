package ch9;

import java.io.IOException;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.StandardCopyOption;
import java.nio.file.attribute.BasicFileAttributes;

public class CopyDir7 {

    public static void main(String[] args) {

        if (args.length < 2) {
            System.out.println("用法: java ch9.CopyDir7 <来源目录> <目标目录>");
            return;
        }
        String source = args[0];
        String target = args[1];

        try {
            Files.walkFileTree(Paths.get(source), new CopyDirVisitor(source,
                    target));
        } catch (Exception e) {
            System.err.println("复制目录失败！");
        }

    }

}

class CopyDirVisitor extends SimpleFileVisitor<Path> {
    private Path fromPath;
    private Path toPath;
    private StandardCopyOption copyOption = StandardCopyOption.REPLACE_EXISTING;

    CopyDirVisitor(String from, String to) {
        fromPath = Paths.get(from);
        toPath = Paths.get(to);
    }

    @Override
    public FileVisitResult preVisitDirectory(Path dir, BasicFileAttributes attrs)
            throws IOException {
        Path targetPath = toPath.resolve(fromPath.relativize(dir));
        if (!Files.exists(targetPath)) {
            Files.createDirectory(targetPath);
        }
        return FileVisitResult.CONTINUE;
    }

    @Override
    public FileVisitResult visitFile(Path file, BasicFileAttributes attrs)
            throws IOException {
        Files.copy(file, toPath.resolve(fromPath.relativize(file)), copyOption);
        return FileVisitResult.CONTINUE;
    }
}
