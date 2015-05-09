package exp4;

import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;

/**
 * @version 2015-4-21
 * @author Kingfree
 */
public class ReaderTest {

    /**
     * 读入一个文本文件并输出
     * 
     * @param args 参数为文件名
     */
    public static void main(String[] args) {

        if (args.length < 1) {
            System.out.println("用法: java exp4.ReaderTest <文件名>");
            return;
        }

        try (Reader in = new FileReader(args[0])) {
            int c;
            while ((c = in.read()) != -1) {
                System.out.print((char) c);
            }
        } catch (IOException e) {
            System.out.println("输入输出错误: " + e);
        }

    }

}
