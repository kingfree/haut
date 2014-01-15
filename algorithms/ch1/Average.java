import java.io.*;
import java.util.*;

public class Average {
	public static void main(String[] args) {
		Scanner cin = new Scanner(System.in);
		double sum = 0.0;
		int cnt = 0;
		while (cin.hasNextDouble()) {
			sum += cin.nextDouble();
			cnt++;
		}
		double avg = sum / cnt;
		System.out.printf("Average is %.5f\n", avg);
	}
}