package ch9;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class CopyDir6 {

    public static void main(String[] args) {

        if (args.length < 2) {
            System.out.println("用法: java ch9.CopyDir6 <来源目录> <目标目录>");
            return;
        }
        String source = args[0];
        String target = args[1];

        try {
            copyDir(new File(source), new File(target));
        } catch (Exception e) {
            System.err.println("复制目录失败！");
        }

    }

    private static void copyDir(File source, File target) throws IOException {
        if (source.isDirectory()) {
            if (!target.exists()) {
                target.mkdir();
            }
            String files[] = source.list();
            for (String file : files) {
                copyDir(new File(source, file), new File(target, file));
            }
        } else {
            try (InputStream in = new FileInputStream(source);
                    OutputStream out = new FileOutputStream(target)) {
                byte[] buffer = new byte[1024];
                int length;
                while ((length = in.read(buffer)) > 0) {
                    out.write(buffer, 0, length);
                }
            }
        }
    }
}
