package exp6;

import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.regex.MatchResult;
import java.util.regex.Pattern;

/**
 * 学生信息管理系统
 * 
 * @version 2015-6-10
 * @author Kingfree
 */
public class StudentManager {

    private static Scanner in = new Scanner(System.in);

    public static int mainMenu() {
        System.out.println("学生信息管理系统");
        System.out.println("1 显示所有学生信息 2 按学号查找 3 按姓名查找");
        System.out.println("4 按学号删除 5 按成绩排序 6 退出");
        System.out.println("7 添加学生");
        System.out.println("请输入数字(1-6)");
        int sel = in.nextInt();
        return sel;
    }

    public static void cli() {
        int sel = 0;
        do {
            sel = mainMenu();
            switch (sel) {
            case 1:
                showAll();
                break;
            case 2:
                findById();
                break;
            case 3:
                findByName();
                break;
            case 4:
                delById();
                break;
            case 5:
                sortScore();
                break;
            case 7:
                addStudent();
                break;
            case 6:
                System.out.println("成功退出系统！");
                return;
            }
        } while (sel != 0);
    }

    private static void add(Student stu) {
        if (Student.insert(stu)) {
            System.out.println("成功添加学生。");
        } else {
            System.out.println("添加学生失败！");
        }
    }

    public static Student inputStudent(Scanner in) {
        for (;;) {
            System.out.print(">");
            try (Scanner line = new Scanner(in.nextLine())) {
                line.findInLine(Pattern
                        .compile("(\\d+)\\s(\\d+)\\s(.+)\\s(\\d+)\\s(\\d+)\\s(\\d+)"));
                try {
                    MatchResult result = line.match();
                    int id = Integer.parseInt(result.group(1));
                    int cid = Integer.parseInt(result.group(2));
                    String name = result.group(3);
                    int os = Integer.parseInt(result.group(4));
                    int math = Integer.parseInt(result.group(5));
                    int java = Integer.parseInt(result.group(6));
                    Student you = new Student(id, cid, name, os, math, java);
                    return you;
                } catch (Exception e) {
                    System.out.println("输入格式有误，请重新输入！");
                }
            }
        }
    }

    private static void addStudent() {
        System.out.println("请输入学生信息: ");
        System.out.println("格式: <学号> <班级> <姓名> <数学成绩> <Java成绩> <操作系统成绩>");

        try (Scanner in = new Scanner(System.in)) {
            Student you = inputStudent(in);
            System.out.println("成功读入学生 '" + you + "'");
            add(you);
        }
    }

    private static void sortScore() {
        System.out.println("1 按math成绩 2 按os成绩 3 按java成绩，请输入(1-3)");
        int sel = in.nextInt();
        switch (sel) {
        case 1:
            sortMath();
            break;
        case 2:
            sortOS();
            break;
        case 3:
            sortJava();
            break;
        default:
            return;
        }
    }

    private static void showBy(String col) {
        print(Student.selectOrberBy(col));
    }

    private static void sortOS() {
        showBy("os");
    }

    private static void sortJava() {
        showBy("java");
    }

    private static void sortMath() {
        showBy("math");
    }

    private static void delById() {
        System.out.println("请输入学号：");
        int id = in.nextInt();
        Student you = Student.selectById(id);
        if (you == null) {
            System.out.println("没有找到学生！");
            return;
        }
        System.out.println("你确定删除学生 '" + you + "' 吗？(Y/n)");
        String s = in.nextLine();
        s = in.nextLine();
        char c = s.charAt(0);
        System.out.println(s);
        System.out.println(c);
        if (!(c == 'y' || c == 'Y')) {
            return;
        }

        if (Student.deleteById(id)) {
            System.out.println("已删除。");
        } else {
            System.out.println("删除失败！");
        }
    }

    private static void findByName() {
        System.out.println("请输入姓名：");
        String name = in.next().trim();
        System.out.println("查找：'" + name + "'");
        List<Student> you = Student.selectByName(name);
        if (you == null) {
            System.out.println("没有找到！");
        } else {
            print(you);
        }
    }

    private static void findById() {
        System.out.println("请输入学号：");
        int id = in.nextInt();
        Student you = Student.selectById(id);
        if (you == null) {
            System.out.println("没有找到！");
        } else {
            print(you);
        }
    }

    private static void showAll() {
        print(Student.selectAll());
    }

    public static void gui() {
        new LoginWindow();
    }

    public static void cheet() {
        new MainWindow();
    }

    /**
     * 系统启动入口
     * @param args 指定启动方式
     */
    public static void main(String[] args) {
        String arg = "-gui";
        if (args.length > 0) {
            arg = args[0].toLowerCase();
        }
        switch (arg) {
        case "-gui":
            gui();
            break;
        case "-cheet":
            cheet();
            break;
        case "-cli":
            cli();
            break;
        default:
            System.out.println("用法: java ssys.StudentManager [参数]");
            System.out.println("参数: \t-gui \t图形界面（默认）");
            System.out.println("     \t-cli \t命令行界面");
        }
    }

    private static void print(Object obj) {
        if (obj instanceof List) {
            List<?> list = (List<?>) obj;
            for (Object o : list) {
                if (o instanceof Map) {
                    @SuppressWarnings("unchecked")
                    Map<String, Object> map = (Map<String, Object>) o;
                    Set<String> set = map.keySet();
                    for (String key : set) {
                        Object value = map.get(key);
                        System.out.print(key + ":" + value + "\t");
                    }
                    System.out.println();
                } else {
                    System.out.println(o);
                }
            }
            System.out.println("总共查询出数据数量是：" + list.size());
        } else {
            System.out.println(obj);
        }
    }

}
