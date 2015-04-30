package exp6;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

/**
 * 学生类
 * 
 * @version 2015-4-5
 * @author Kingfree
 */
public class Student {

	private int id;
	private String name;

	public Student(int id, String name) {
		setId(id);
		setName(name);
	}

	private void setId(int id) {
		this.id = id;
	}

	private void setName(String name) {
		this.name = name;
	}

	public int getId() {
		return id;
	}

	public String getName() {
		return name;
	}

	public int getScore(String key) {
		PreparedStatement st = null;
		ResultSet rs = null;
		Connection conn = null;
		try {
			conn = DBUtils.getConnection();

			String sql = new String("SELECT scores.score"
					+ " FROM scores, subjects"
					+ " WHERE scores.student_id = ?"
					+ " AND subjects.name = ?"
					+ " AND subjects.id = scores.subject_id");
			st = conn.prepareStatement(sql);
			st.setInt(1, getId());
			st.setString(2, key);
			rs = st.executeQuery();

			while (rs.next()) {
				return rs.getInt("score");
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBUtils.free(rs, st, conn);
		}
		return 0;
	}

	private HashMap<String, Integer> getScores() {
		PreparedStatement st = null;
		ResultSet rs = null;
		Connection conn = null;
		try {
			conn = DBUtils.getConnection();

			String sql = new String("SELECT"
					+ " students.name, subjects.name as subject_name"
					+ ", subjects.memo, scores.score"
					+ " FROM students, subjects, scores"
					+ " WHERE scores.student_id = ?"
					+ " AND subjects.id = scores.subject_id");
			st = conn.prepareStatement(sql);
			st.setInt(1, getId());
			rs = st.executeQuery();

			HashMap<String, Integer> m = new HashMap<String, Integer>();
			while (rs.next()) {
				m.put(rs.getString("subject_name"), rs.getInt("score"));
			}
			return m;
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBUtils.free(rs, st, conn);
		}
		return null;
	}

	@Override
	public String toString() {
		return getName() + " " + getId() + " " + getScoresString();
	}

	private String getScoresString() {
		StringBuilder s = new StringBuilder(); 
		Map<String, Integer> m = getScores();
		Iterator<Entry<String, Integer>> i = m.entrySet().iterator();
		while (i.hasNext()) {
			Map.Entry<String, Integer> e = (Entry<String, Integer>) i.next();
			String key = (String) e.getKey();
			int val = (int) e.getValue();
			s.append(" " + key + ":" + val);
		}
		return s.substring(1);
	}

	public static void dispAll() {
		
	}

}
