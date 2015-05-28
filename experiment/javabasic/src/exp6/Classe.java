package exp6;

public class Classe {

    private int id;
    private String name;

    public Classe() {
    }

    @Override
    public String toString() {
        return name;
    }

    public Classe(int id, String name) {
        super();
        this.id = id;
        this.name = name;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
