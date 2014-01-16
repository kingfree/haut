package main
import (
    . "fmt"
)

func fib(c, quit chan int) {
    x, y := 1, 1
    for {
        select {
        case c <- x:
            x, y = y, x + y
        case <-quit:
            Println("退出")
            return
        }
    }
}

func main() {
    c := make(chan int)
    q := make(chan int)
    go func() {
        for i := 0; i < 10; i++ {
            Println(<-c)
        }
        q <- 0
    }()
    fib(c, q)
}
