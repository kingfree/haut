package main

import (
	. "fmt"
	"runtime"
)

func say(s string) {
	for i := 0; i < 5; i++ {
		runtime.Gosched() // 让出 CPU 时间片
		Println(s)
	}
}

func main() {
	runtime.GOMAXPROCS(2) // 显式声明线程数
	go say("かわ")          // 新开并发
	say("やま")
}
