/**
 * 在终端上显示一些信息
 * @version 2015-3-8
 * @author Kingfree
 */
public class Welcome {

	public static void main(String[] args) {
		String[] greeting = new String[3];
		greeting[0] = "Hello, World!";
		greeting[1] = "你好，世界！";
		greeting[2] = "こんにちは、セカイ！";
		
		for (String g : greeting) {
			System.out.println(g);
		}
	}

}
