package exp6;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.regex.MatchResult;
import java.util.regex.Pattern;

import javax.swing.SwingUtilities;

/**
 * 学生信息管理系统
 * 
 * @version 2015-5-14
 * @author Kingfree
 */
public class StudentManager {

    private static Connection conn = null;
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
        init();
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
            case 6:
            default:
                System.out.println("成功退出系统！");
                return;
            }
        } while (sel != 0);
    }

    private static void add(Student stu) {
        String sql = "INSERT INTO students (id, class_id, name, os, math, java) VALUES (?, 1, ?, ?, ?, ?)";
        try {
            int res = DBUtils.execute(conn, sql, stu.getId(), stu.getName(),
                    stu.getOs(), stu.getMath(), stu.getJava());
            if (res == 1) {
                System.out.println("成功添加学生。");
            } else {
                System.out.println("添加操作出现异常！");
            }
        } catch (Exception e) {
            System.out.println("添加失败！");
        }
    }

    public static Student inputStudent(Scanner in) {
        for (;;) {
            System.out.print(">");
            try (Scanner line = new Scanner(in.nextLine())) {
                line.findInLine(Pattern
                        .compile("(\\d+)\\s(.+)\\s(\\d+)\\s(\\d+)\\s(\\d+)"));
                try {
                    MatchResult result = line.match();
                    int id = Integer.parseInt(result.group(1));
                    String name = result.group(2);
                    int os = Integer.parseInt(result.group(3));
                    int math = Integer.parseInt(result.group(3));
                    int java = Integer.parseInt(result.group(3));
                    Student you = new Student(id, name, os, math, java);
                    return you;
                } catch (Exception e) {
                    System.out.println("输入格式有误，请重新输入！");
                }
            }
        }
    }

    private static void addStudent() {
        System.out.println("请输入学生信息: ");
        System.out.println("格式: <学号> <姓名> <数学成绩> <Java成绩> <操作系统成绩>");

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
        String sql = new String("SELECT * FROM students ORDER BY " + col
                + " DESC");
        try {
            List<Student> emList = DBUtils.queryBeanList(conn, sql,
                    new IResultSetCall<Student>() {
                        public Student invoke(ResultSet rs) throws SQLException {
                            Student e = new Student(rs.getInt("id"), rs
                                    .getString("name"), rs.getInt("os"), rs
                                    .getInt("math"), rs.getInt("java"));
                            return e;
                        }
                    });
            print(emList);
        } catch (Exception e) {
            System.out.println("没有找到记录！");
        }
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
        String sql = "SELECT * FROM students WHERE id = ?";
        try {
            Student you = DBUtils.queryBean(conn, sql, Student.class, id);
            System.out.println("你确定删除学生 '" + you + "' 吗？(Y/n)");
            String s = in.nextLine();
            s = in.nextLine();
            char c = s.charAt(0);
            System.out.println(s);
            System.out.println(c);
            if (!(c == 'y' || c == 'Y')) {
                return;
            }
        } catch (Exception e) {
            System.out.println("没有找到学生！");
            return;
        }
        sql = "DELETE FROM students WHERE id = ?";
        try {
            int res = DBUtils.execute(conn, sql, id);
            if (res == 1) {
                System.out.println("已删除。");
            } else {
                System.out.println("删除操作出现异常！");
            }
        } catch (Exception e) {
            System.out.println("删除失败！");
        }
    }

    private static void findByName() {
        System.out.println("请输入姓名：");
        String name = in.next().trim();
        System.out.println("查找：'" + name + "'");
        String sql = "SELECT * FROM students WHERE name LIKE '%张%'";
        try {
            Student you = DBUtils.queryBean(conn, sql, Student.class);
            print(you);
        } catch (Exception e) {
            System.out.println("没有找到记录！");
        }
    }

    private static void findById() {
        System.out.println("请输入学号：");
        int id = in.nextInt();
        String sql = "SELECT * FROM students WHERE id = ?";
        try {
            Student you = DBUtils.queryBean(conn, sql, Student.class, id);
            print(you);
        } catch (Exception e) {
            System.out.println("没有找到记录！");
        }
    }

    private static void showAll() {
        String sql = new String("SELECT * FROM students");
        try {
            List<Student> emList = DBUtils.queryBeanList(conn, sql,
                    new IResultSetCall<Student>() {
                        public Student invoke(ResultSet rs) throws SQLException {
                            Student e = new Student(rs.getInt("id"), rs
                                    .getString("name"), rs.getInt("os"), rs
                                    .getInt("math"), rs.getInt("java"));
                            return e;
                        }
                    });
            print(emList);
        } catch (Exception e) {
            System.out.println("没有找到记录！");
        }
    }

    public static void test() {
        System.out.println("Test");
    }

    public static void gui() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new MainWindow();
            }
        });
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
        case "-cli":
            cli();
            break;
        case "-test":
            test();
            break;
        default:
            System.out.println("用法: java ssys.StudentManager [参数]");
            System.out.println("参数: \t-gui \t图形界面（默认）");
            System.out.println("     \t-cli \t命令行界面");
            System.out.println("     \t-test \t测试模式");
        }
    }

    private static void init() {
        try {
            conn = DBUtils.openConnection();

            List<String> sql = new ArrayList<String>();

            // sql.add("DROP TABLE IF EXISTS students");
            sql.add("CREATE TABLE IF NOT EXISTS students(id INTEGER PRIMARY KEY, class_id INTEGER"
                    + ", name STRING, os INTEGER, math INTEGER, java INTEGER)");

            // sql.add("INSERT INTO students VALUES(1, 1, '张三', 90, 90, 90)");
            // sql.add("INSERT INTO students VALUES(4, 1, '小明', 89, 72, 88)");
            // sql.add("INSERT INTO students VALUES(2, 2, '李四', 80, 70, 91)");
            // sql.add("INSERT INTO students VALUES(3, 3, '王五', 60, 67, 70)");

            // sql.add("DROP TABLE IF EXISTS classes");
            sql.add("CREATE TABLE IF NOT EXISTS classes(id INTEGER PRIMARY KEY, name STRING)");

            // sql.add("INSERT INTO classes VALUES(1, '软件1301')");
            // sql.add("INSERT INTO classes VALUES(2, '软件1302')");
            // sql.add("INSERT INTO classes VALUES(3, '软件1303')");
            // sql.add("INSERT INTO classes VALUES(4, '软件1304')");
            // sql.add("INSERT INTO classes VALUES(5, '软件1305')");

            DBUtils.executeAsBatch(conn, sql);
        } catch (SQLException e) {
            System.err.println(e.getMessage());
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
