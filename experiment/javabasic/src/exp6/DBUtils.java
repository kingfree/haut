package exp6;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Time;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 数据库工具类
 * 参考 https://github.com/felixyin/DBUtil/
 * 
 * @version 2015-5-14
 * @author Kingfree
 */
public final class DBUtils {

    private DBUtils() {
    }

    static {
        try {
            Class.forName("org.sqlite.JDBC");
        } catch (ClassNotFoundException e) {
            System.out.println("驱动加载出错!");
        }
    }

    private static Connection con = null;

    public static Connection openConnection() throws SQLException {
        if (null == con || con.isClosed()) {
            con = DriverManager.getConnection("jdbc:sqlite:bin/sample.db");
        }
        return con;
    }

    public static void closeConnection() throws SQLException {
        try {
            if (null != con)
                con.close();
        } finally {
            con = null;
            System.gc();
        }
    }

    public static List<Map<String, Object>> queryMapList(Connection con,
            String sql) throws SQLException, InstantiationException,
            IllegalAccessException {
        List<Map<String, Object>> lists = new ArrayList<Map<String, Object>>();
        Statement preStmt = null;
        ResultSet rs = null;
        try {
            preStmt = con.createStatement();
            rs = preStmt.executeQuery(sql);
            ResultSetMetaData rsmd = rs.getMetaData();
            int columnCount = rsmd.getColumnCount();
            while (null != rs && rs.next()) {
                Map<String, Object> map = new HashMap<String, Object>();
                for (int i = 0; i < columnCount; i++) {
                    String name = rsmd.getColumnName(i + 1);
                    Object value = rs.getObject(name);
                    map.put(name, value);
                }
                lists.add(map);
            }
        } finally {
            if (null != rs)
                rs.close();
            if (null != preStmt)
                preStmt.close();
        }
        return lists;
    }

    public static List<Map<String, Object>> queryMapList(Connection con,
            String sql, Object... params) throws SQLException,
            InstantiationException, IllegalAccessException {
        List<Map<String, Object>> lists = new ArrayList<Map<String, Object>>();
        PreparedStatement preStmt = null;
        ResultSet rs = null;
        try {
            preStmt = con.prepareStatement(sql);
            for (int i = 0; i < params.length; i++)
                preStmt.setObject(i + 1, params[i]);// 下标从1开始
            rs = preStmt.executeQuery();
            ResultSetMetaData rsmd = rs.getMetaData();
            int columnCount = rsmd.getColumnCount();
            while (null != rs && rs.next()) {
                Map<String, Object> map = new HashMap<String, Object>();
                for (int i = 0; i < columnCount; i++) {
                    String name = rsmd.getColumnName(i + 1);
                    Object value = rs.getObject(name);
                    map.put(name, value);
                }
                lists.add(map);
            }
        } finally {
            if (null != rs)
                rs.close();
            if (null != preStmt)
                preStmt.close();
        }
        return lists;
    }

    public static <T> List<T> queryBeanList(Connection con, String sql,
            Class<T> beanClass) throws SQLException, InstantiationException,
            IllegalAccessException {
        List<T> lists = new ArrayList<T>();
        Statement stmt = null;
        ResultSet rs = null;
        Field[] fields = null;
        try {
            stmt = con.createStatement();
            rs = stmt.executeQuery(sql);
            fields = beanClass.getDeclaredFields();
            for (Field f : fields)
                f.setAccessible(true);
            while (null != rs && rs.next()) {
                T t = beanClass.newInstance();
                for (Field f : fields) {
                    String name = f.getName();
                    try {
                        Object value = rs.getObject(name);
                        setValue(t, f, value);
                    } catch (Exception e) {
                    }
                }
                lists.add(t);
            }
        } finally {
            if (null != rs)
                rs.close();
            if (null != stmt)
                stmt.close();
        }
        return lists;
    }

    public static <T> List<T> queryBeanList(Connection con, String sql,
            Class<T> beanClass, Object... params) throws SQLException,
            InstantiationException, IllegalAccessException {
        List<T> lists = new ArrayList<T>();
        PreparedStatement preStmt = null;
        ResultSet rs = null;
        Field[] fields = null;
        try {
            preStmt = con.prepareStatement(sql);
            for (int i = 0; i < params.length; i++)
                preStmt.setObject(i + 1, params[i]);// 下标从1开始
            rs = preStmt.executeQuery();
            fields = beanClass.getDeclaredFields();
            for (Field f : fields)
                f.setAccessible(true);
            while (null != rs && rs.next()) {
                T t = beanClass.newInstance();
                for (Field f : fields) {
                    String name = f.getName();
                    try {
                        Object value = rs.getObject(name);
                        setValue(t, f, value);
                    } catch (Exception e) {
                    }
                }
                lists.add(t);
            }
        } finally {
            if (null != rs)
                rs.close();
            if (null != preStmt)
                preStmt.close();
        }
        return lists;
    }

    public static <T> List<T> queryBeanList(Connection con, String sql,
            IResultSetCall<T> qdi) throws SQLException {
        List<T> lists = new ArrayList<T>();
        Statement stmt = null;
        ResultSet rs = null;
        try {
            stmt = con.createStatement();
            rs = stmt.executeQuery(sql);
            while (null != rs && rs.next())
                lists.add(qdi.invoke(rs));
        } finally {
            if (null != rs)
                rs.close();
            if (null != stmt)
                stmt.close();
        }
        return lists;
    }

    public static <T> List<T> queryBeanList(Connection con, String sql,
            IResultSetCall<T> qdi, Object... params) throws SQLException {
        List<T> lists = new ArrayList<T>();
        PreparedStatement preStmt = null;
        ResultSet rs = null;
        try {
            preStmt = con.prepareStatement(sql);
            for (int i = 0; i < params.length; i++)
                preStmt.setObject(i + 1, params[i]);
            rs = preStmt.executeQuery();
            while (null != rs && rs.next())
                lists.add(qdi.invoke(rs));
        } finally {
            if (null != rs)
                rs.close();
            if (null != preStmt)
                preStmt.close();
        }
        return lists;
    }

    public static <T> T queryBean(Connection con, String sql, Class<T> beanClass)
            throws SQLException, InstantiationException, IllegalAccessException {
        List<T> lists = queryBeanList(con, sql, beanClass);
        if (lists.size() != 1)
            throw new SQLException("SqlError：期待一行返回值，却返回了太多行！");
        return lists.get(0);
    }

    public static <T> T queryBean(Connection con, String sql,
            Class<T> beanClass, Object... params) throws SQLException,
            InstantiationException, IllegalAccessException {
        List<T> lists = queryBeanList(con, sql, beanClass, params);
        if (lists.size() != 1)
            throw new SQLException("SqlError：期待一行返回值，却返回了太多行！");
        return lists.get(0);
    }

    @SuppressWarnings("unchecked")
    public static <T> List<T> queryObjectList(Connection con, String sql,
            Class<T> objClass) throws SQLException, InstantiationException,
            IllegalAccessException {
        List<T> lists = new ArrayList<T>();
        Statement stmt = null;
        ResultSet rs = null;
        try {
            stmt = con.createStatement();
            rs = stmt.executeQuery(sql);
            label: while (null != rs && rs.next()) {
                Constructor<?>[] constor = objClass.getConstructors();
                for (Constructor<?> c : constor) {
                    Object value = rs.getObject(1);
                    try {
                        lists.add((T) c.newInstance(value));
                        continue label;
                    } catch (Exception e) {
                    }
                }
            }
        } finally {
            if (null != rs)
                rs.close();
            if (null != stmt)
                stmt.close();
        }
        return lists;
    }

    public static <T> List<T> queryObjectList(Connection con, String sql,
            Class<T> objClass, Object... params) throws SQLException,
            InstantiationException, IllegalAccessException {
        List<T> lists = new ArrayList<T>();
        PreparedStatement preStmt = null;
        ResultSet rs = null;
        try {
            preStmt = con.prepareStatement(sql);
            for (int i = 0; i < params.length; i++)
                preStmt.setObject(i + 1, params[i]);
            rs = preStmt.executeQuery();
            label: while (null != rs && rs.next()) {
                Constructor<?>[] constor = objClass.getConstructors();
                for (Constructor<?> c : constor) {
                    String value = rs.getObject(1).toString();
                    try {
                        @SuppressWarnings("unchecked")
                        T t = (T) c.newInstance(value);
                        lists.add(t);
                        continue label;
                    } catch (Exception e) {
                    }
                }
            }
        } finally {
            if (null != rs)
                rs.close();
            if (null != preStmt)
                preStmt.close();
        }
        return lists;
    }

    public static <T> T queryObject(Connection con, String sql,
            Class<T> objClass) throws SQLException, InstantiationException,
            IllegalAccessException {
        List<T> lists = queryObjectList(con, sql, objClass);
        if (lists.size() != 1)
            throw new SQLException("SqlError：期待一行返回值，却返回了太多行！");
        return lists.get(0);
    }

    public static <T> T queryObject(Connection con, String sql,
            Class<T> objClass, Object... params) throws SQLException,
            InstantiationException, IllegalAccessException {
        List<T> lists = queryObjectList(con, sql, objClass, params);
        if (lists.size() != 1)
            throw new SQLException("SqlError：期待一行返回值，却返回了太多行！");
        return lists.get(0);
    }

    public static int[] executeAsBatch(Connection con, List<String> sqlList)
            throws SQLException {
        return executeAsBatch(con, sqlList.toArray(new String[] {}));
    }

    public static int[] executeAsBatch(Connection con, String[] sqlArray)
            throws SQLException {
        Statement stmt = null;
        try {
            stmt = con.createStatement();
            for (String sql : sqlArray) {
                stmt.addBatch(sql);
            }
            return stmt.executeBatch();
        } finally {
            if (null != stmt) {
                stmt.close();
            }
        }
    }

    public static int[] executeAsBatch(Connection con, String sql,
            Object[][] params) throws SQLException {
        PreparedStatement preStmt = null;
        try {
            preStmt = con.prepareStatement(sql);
            for (int i = 0; i < params.length; i++) {
                Object[] rowParams = params[i];
                for (int k = 0; k < rowParams.length; k++) {
                    Object obj = rowParams[k];
                    preStmt.setObject(k + 1, obj);
                }
                preStmt.addBatch();
            }
            return preStmt.executeBatch();
        } finally {
            if (null != preStmt) {
                preStmt.close();
            }
        }
    }

    public static int execute(Connection con, String sql) throws SQLException {
        Statement stmt = null;
        try {
            stmt = con.createStatement();
            return stmt.executeUpdate(sql);
        } finally {
            if (null != stmt)
                stmt.close();
        }
    }

    public static int execute(Connection con, String sql, Object... params)
            throws SQLException {
        PreparedStatement preStmt = null;
        try {
            preStmt = con.prepareStatement(sql);
            for (int i = 0; i < params.length; i++)
                preStmt.setObject(i + 1, params[i]);// 下标从1开始
            return preStmt.executeUpdate();
        } finally {
            if (null != preStmt)
                preStmt.close();
        }
    }

    private static <T> void setValue(T t, Field f, Object value)
            throws IllegalAccessException {
        // TODO 以数据库类型为准绳，还是以java数据类型为准绳？还是混合两种方式？
        if (null == value)
            return;
        String v = value.toString();
        String n = f.getType().getName();
        if ("java.lang.Byte".equals(n) || "byte".equals(n)) {
            f.set(t, Byte.parseByte(v));
        } else if ("java.lang.Short".equals(n) || "short".equals(n)) {
            f.set(t, Short.parseShort(v));
        } else if ("java.lang.Integer".equals(n) || "int".equals(n)) {
            f.set(t, Integer.parseInt(v));
        } else if ("java.lang.Long".equals(n) || "long".equals(n)) {
            f.set(t, Long.parseLong(v));
        } else if ("java.lang.Float".equals(n) || "float".equals(n)) {
            f.set(t, Float.parseFloat(v));
        } else if ("java.lang.Double".equals(n) || "double".equals(n)) {
            f.set(t, Double.parseDouble(v));
        } else if ("java.lang.String".equals(n)) {
            f.set(t, value.toString());
        } else if ("java.lang.Character".equals(n) || "char".equals(n)) {
            f.set(t, (Character) value);
        } else if ("java.lang.Date".equals(n)) {
            f.set(t, new Date(((java.sql.Date) value).getTime()));
        } else if ("java.lang.Timer".equals(n)) {
            f.set(t, new Time(((java.sql.Time) value).getTime()));
        } else if ("java.sql.Timestamp".equals(n)) {
            f.set(t, (java.sql.Timestamp) value);
        } else {
            System.out.println("SqlError：暂时不支持此数据类型，请使用其他类型代替此类型！");
        }
    }

}
