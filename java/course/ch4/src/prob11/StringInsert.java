package prob11;

/**
 * @version 2015-4-2
 * @author Kingfree
 */
public class StringInsert {

	public static void main(String[] args) {
		String str = "China is a country.";
		System.out.println(str);
		String ins = " great";
		StringBuffer sb = new StringBuffer(str);
		sb.insert("China is a".length(), ins);
		System.out.println(sb);
	}

}
