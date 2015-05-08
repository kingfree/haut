package ch4;

/**
 * @version 2015-4-2
 * @author Kingfree
 */
public class Chicken {

    public static void main(String[] args) {
        double 公鸡价格 = 3;
        double 母鸡价格 = 2;
        double 小鸡价格 = 1.0 / 3;
        double 现有钱数 = 100;
        int 想买鸡数 = 100;

        try {
            现有钱数 = Double.parseDouble(args[0]);
            想买鸡数 = Integer.parseInt(args[1]);
        } catch (Exception e) {
            System.out.println("用法: java prob13.Chicken [现有钱数] [想买鸡数]");
            System.out.println("参数: \t现有钱数\t默认为100");
            System.out.println("     \t想买鸡数\t默认为100");
        }
        
        System.out.println("《百鸡问题》");
        System.out.println("现有钱数=" + 现有钱数);
        System.out.println("想买鸡数=" + 想买鸡数);

        int i = 0;
        for (int 公鸡数 = 0; 公鸡数 <= 想买鸡数; 公鸡数++) {
            for (int 母鸡数 = 0; 母鸡数 <= 想买鸡数; 母鸡数++) {
                int 小鸡数 = 想买鸡数 - (公鸡数 + 母鸡数);
                if (公鸡价格 * 公鸡数 + 母鸡价格 * 母鸡数 + 小鸡价格 * 小鸡数 == 现有钱数) {
                    System.out.print("方案" + (++i) + ": ");
                    System.out.print(" 公鸡数=" + 公鸡数);
                    System.out.print(" 母鸡数=" + 母鸡数);
                    System.out.print(" 小鸡数=" + 小鸡数);
                    System.out.println();
                }
            }
        }

    }

}
