package main
import (
    . "fmt"
)

func sum(a []int, c chan int) {
    sum := 0
    for _, v := range a {
        sum += v
    }
    c <- sum // 发送
}

func main() {
    a := []int{7, 2, 8, -9, 4, 0}
    c := make(chan int)
    go sum(a[:len(a)/2], c)
    go sum(a[len(a)/2:], c)
    x, y := <-c, <-c // 收取
    Println(x, y, x + y)

    b := make(chan int, 2) // 改为 1 报错，改为 3 正常运行
    b <- 1
    b <- 2
    Println(<-b, <-b)
}
