package exp4;

import java.util.Scanner;
import java.util.regex.MatchResult;
import java.util.regex.Pattern;

import exp3.Student;

/**
 * @version 2015-4-21
 * @author Kingfree
 */
public class StudentTest {

    public static Student inputStudent(Scanner in) {
        for (;;) {
            System.out.print(">");
            try (Scanner line = new Scanner(in.nextLine())) {
                line.findInLine(Pattern.compile("(\\d+)\\s(.+)\\s(\\d+)"));
                try {
                    MatchResult result = line.match();
                    int id = Integer.parseInt(result.group(1));
                    String name = result.group(2);
                    int age = Integer.parseInt(result.group(3));
                    Student you = new Student(name, age, 'f', id);
                    return you;
                } catch (IllegalStateException | IndexOutOfBoundsException e) {
                    System.out.println("输入格式有误，请重新输入！");
                }
            }
        }
    }

    public static void main(String[] args) {

        System.out.println("请输入学生信息: ");
        System.out.println("格式: <学号> <姓名> <年龄>");

        try (Scanner in = new Scanner(System.in)) {
            Student you = inputStudent(in);
            System.out.println("成功读入学生信息！");
            System.out.println(you);
        }
    }

}
