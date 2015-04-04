package ssys;

import java.io.Serializable;
import java.util.Arrays;

/**
 * 学生类
 * @version 2015-4-5
 * @author Kingfree
 */
public class Student implements Serializable {

    private static final long serialVersionUID = 1L;

    private long              id;
    private String            name;
    private int[]             score            = { 0, 0, 0 };
    private String[]          scoreName        = { "os", "java", "math" };
    private int               scoreSize        = 3;

    public Student(long id, String name) {
        setId(id);
        setName(name);
    }

    private int getColumn(String key) throws ArrayIndexOutOfBoundsException {
        int i = Arrays.asList(scoreName).indexOf(key);
        if (i == -1) {
            throw new ArrayIndexOutOfBoundsException();
        }
        return i;
    }

    public long getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public int getScoreAt(int i) {
        return score[i];
    }

    public int getScoreBy(String key) {
        return score[getColumn(key)];
    }

    public String getScoreNameAt(int i) {
        return scoreName[i];
    }

    public String getScores() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < scoreSize - 1; i++) {
            sb.append(scoreName[i] + ":" + score[i] + " ");
        }
        sb.append(scoreName[scoreSize - 1] + ":" + score[scoreSize - 1]);
        return sb.toString();
    }

    public void setId(long id) {
        this.id = Math.abs(id);
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setScoreAt(int i, int score) {
        this.score[i] = Math.abs(score);
    }

    public void setScoreBy(String key, int score) {
        this.score[getColumn(key)] = Math.abs(score);
    }

    public void setScoreNameAt(int i, String scoreName) {
        this.scoreName[i] = scoreName;
    }

    @Override
    public String toString() {
        return getName() + " " + getId() + " " + getScores();
    }

}
