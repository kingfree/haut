package main
import "fmt"

type Human struct {
    name string
    age int
    phone string
}

type Student struct {
    Human //类继承（匿名字段）
    school string
    loan float32
}

type Employee struct {
    Human
    company string
    money float32
}

func (h Human) SayHi() {
    fmt.Printf("你好，我是 %s ，请拨打 %s 联系我。\n", h.name, h.phone)
}

func (h Human) Sing(lyrics string) {
    fmt.Println("せ～の　", lyrics)
}

type Men interface { //接口
    SayHi() //鸭子类型
    Sing(lyrics string)
}

func (e Employee) SayHi() { //方法重载
    fmt.Printf("你好，我是 %s ，在 %s 工作，请拨打 %s 联系我。\n",
        e.name, e.company, e.phone)
}

func main() {
    mark := Student{Human{"Mark", 25, "222-222-yyyy"}, "MIT", 0.00}
    paul := Student{Human{"Paul", 26, "111-222-yyyy"}, "Harvard", 100}
    sam := Employee{Human{"Sam", 45, "111-888-xxxx"}, "Google", 1000}
    tom := Employee{Human{"Tom", 36, "444-888-xxxx"}, "Microsoft", 5000}

    var i Men
    i = mark
    fmt.Println("这是学生 Mike:")
    i.SayHi()
    i.Sing("1 2 3 Hi")

    i = tom
    fmt.Println("这是会社员 Tom:")
    i.SayHi()
    i.Sing("1 2 3 4 Go-han")

    fmt.Println("尝试把 Men 切开看看")
    x := make([]Men, 3)
    x[0], x[1], x[2] = paul, sam, mark
    for _, value := range x {
        value.SayHi()
    }
}
