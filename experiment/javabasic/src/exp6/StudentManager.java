package exp6;

import java.io.Serializable;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Scanner;

import javax.swing.SwingUtilities;

/**
 * 学生信息管理系统
 * 
 * @version 2015-3-27
 * @author Kingfree
 */
public class StudentManager implements Serializable {
    private static final long serialVersionUID = -7641996911847167805L;
    private static Scanner in = new Scanner(System.in);
	
	public static int mainMenu() {
        System.out.println("学生信息管理系统");
        System.out.println("1 显示所有学生信息 2 按学号查找 3 按姓名查找");
        System.out.println("4 按学号删除 5按成绩排序 6 退出");
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
        		findId();
        		break;
        	case 3:
        		findName();
        		break;
        	case 4:
        		delId();
        		break;
        	case 5:
        		sortScore();
        		break;
        	case 6:
        	default:
        		return;
        	}
        } while (sel != 0);
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

	private static void sortOS() {
        // TODO Auto-generated method stub
        
    }

    private static void sortJava() {
        // TODO Auto-generated method stub
        
    }

    private static void sortMath() {
        // TODO Auto-generated method stub
        
    }

    private static void delId() {
		System.out.println("请输入学号：");
		int id = in.nextInt();
	}

	private static void findName() {
		System.out.println("请输入姓名：");
		String name = in.next();
	}

	private static void findId() {
		System.out.println("请输入学号：");
		int id = in.nextInt();
	}

	private static void showAll() {
		Student.dispAll();
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
        String arg = "-cli";
        if (args.length > 0) {
            arg = args[0].toLowerCase();
        }
        switch (arg) {
        case "-gui":
        	init();
            gui();
            break;
        case "-cli":
        	init();
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
        PreparedStatement st = null;
        ResultSet rs = null;
        Connection conn = null;
        try {
            conn = DBUtils.getConnection();
            Statement sql = conn.createStatement();

            sql.executeUpdate("DROP TABLE IF EXISTS students");
            sql.executeUpdate("CREATE TABLE students(id INTEGER PRIMARY KEY, name STRING)");
            sql.executeUpdate("DROP TABLE IF EXISTS subjects");
            sql.executeUpdate("CREATE TABLE subjects (id INTEGER PRIMARY KEY, name STRING"
                    + ", memo STRING)");
            sql.executeUpdate("DROP TABLE IF EXISTS scores");
            sql.executeUpdate("CREATE TABLE scores(" + "student_id INTEGER"
                    + ", subject_id INTEGER" + ", score INTEGER"
                    + ", FOREIGN KEY(student_id) REFERENCES students(id)"
                    + ", FOREIGN KEY(subject_id) REFERENCES subjects(id)" + ")");

            sql.executeUpdate("INSERT INTO subjects VALUES(1, 'os', '操作系统')");
            sql.executeUpdate("INSERT INTO subjects VALUES(2, 'math', '离散数学')");
            sql.executeUpdate("INSERT INTO subjects VALUES(3, 'java', '程序设计')");

            sql.executeUpdate("INSERT INTO students VALUES(12, '张三')");
            sql.executeUpdate("INSERT INTO scores VALUES(12, 1, 90)");
            sql.executeUpdate("INSERT INTO scores VALUES(12, 2, 90)");
            sql.executeUpdate("INSERT INTO scores VALUES(12, 3, 90)");

        } catch (SQLException e) {
            System.err.println(e.getMessage());
        } finally {
            DBUtils.free(rs, st, conn);
        }
	}

}
