package exp6;

import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * 数据集调用接口
 */
public interface IResultSetCall<T> {

    public T invoke(ResultSet rs) throws SQLException;

}
