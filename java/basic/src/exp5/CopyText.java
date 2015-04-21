package exp5;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

/**
 * 用 Reader/Writer 拷贝文件
 * 
 * @version 2015-4-21
 * @author Kingfree
 */
public class CopyText {

    public static void main(String[] args) {

        if (args.length < 2) {
            System.out.println("用法: java exp5.CopyText <来源文件> <目的文件>");
            return;
        }

        try (FileReader in = new FileReader(args[0]);
                FileWriter out = new FileWriter(args[1])) {
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
