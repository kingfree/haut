package ch4;

/**
 * @version 2015-4-3
 * @author Kingfree
 */
public class CountString {

    public static void main(String[] args) {
        String str = "I can write programs in 12 languages.";
        if (args.length < 1) {
            System.out.println("用法: java prob14.CountString <字符串>");
        } else {
            str = args[0];
        }

        int num = 0;
        int alp = 0;
        int ohr = 0;
        for (char c : str.toCharArray()) {
            if (Character.isAlphabetic(c))
                alp++;
            else if (Character.isDigit(c))
                num++;
            else
                ohr++;
        }

        System.out.println(str);
        System.out.println("数字:" + num + " 字母:" + alp + " 其他:" + ohr);
    }

}
