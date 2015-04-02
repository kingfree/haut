package ssys;

import java.sql.*;

/**
 * @version 2015-4-1
 * @author Kingfree
 */
public class UnitTest {

	/**
	 * @param args
	 * @throws ClassNotFoundException
	 */
	public static void main(String[] args) throws ClassNotFoundException {
		// load the sqlite-JDBC driver using the current class loader
		Class.forName("org.sqlite.JDBC");

		Connection connection = null;
		try {
			// create a database connection
			connection = DriverManager.getConnection("jdbc:sqlite:sample.db");
			Statement statement = connection.createStatement();
			statement.setQueryTimeout(30); // set timeout to 30 sec.

			statement.executeUpdate("drop table if exists students");
			statement
					.executeUpdate("create table students (id integer, name string, "
							+ "math integer, os integer, java integer)");
			statement
					.executeUpdate("insert into students values(12, '张三', 90, 90, 90)");
			ResultSet rs = statement.executeQuery("select * from students");
			while (rs.next()) {
				System.out.printf("%s %d os:%d java:%d math:%d",
						rs.getString("name"), rs.getInt("id"), rs.getInt("os"),
						rs.getInt("java"), rs.getInt("math"));
			}
		} catch (SQLException e) {
			// if the error message is "out of memory",
			// it probably means no database file is found
			System.err.println(e.getMessage());
		} finally {
			try {
				if (connection != null)
					connection.close();
			} catch (SQLException e) {
				// connection close failed.
				System.err.println(e);
			}
		}
	}

	public UnitTest() {
		super();
		// TODO Auto-generated constructor stub
	}

	@Override
	public String toString() {
		return "UnitTest []";
	}

}
