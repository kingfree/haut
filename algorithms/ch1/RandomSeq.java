import java.io.*;
import java.util.*;

public class RandomSeq {
	public static void main(String[] args) {
		int N = Integer.parseInt(args[0]);
		double lo = Double.parseDouble(args[1]);
		double hi = Double.parseDouble(args[2]);
		for (int i = 0; i < N; i++) {
			double x = (Math.random() * (hi - lo)) + lo;
			System.out.printf("%.2f\n", x);
		}
	}
}