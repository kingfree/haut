package exp6;

import java.sql.ResultSet;
import java.sql.SQLException;

@FunctionalInterface
public interface ResultSetHandler {

    /**
     * 该方法会由一个lambda表达式执行
     * 
     * @param resultSet 结果集
     * @throws SQLException
     */
    public void handle(ResultSet resultSet) throws SQLException;

}
