package main
import (
    . "fmt"
    "strconv"
)

type Element interface{}
type List [] Element

type Person struct {
    name string
    age int
}

func (p Person) String() string {
    return "(姓名: " + p.name + " - 年龄: " + strconv.Itoa(p.age) + " 岁)"
}

func main() {
    list := make(List, 4)
    list[0] = 1
    list[1] = "hello"
    list[2] = Person{"阿澄佳奈", 30}
    list[3] = 3.14

    for i, e := range list {
        switch v := e.(type) {
        case int:
            Printf("list[%d] 为整数类型，其值为 %d\n", i, v)
        case string:
            Printf("list[%d] 为字符串，其值为 %s\n", i, v)
        case Person:
            Printf("list[%d] 为 Person 类型，其值为 %s\n", i, v)
        default:
            Printf("list[%d] 为其他未知类型\n", i)
        }
    }
}