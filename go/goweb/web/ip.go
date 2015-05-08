package main

import (
	. "fmt"
	"net"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		Fprintf(os.Stderr, "用法：%s <IP地址>", os.Args[0])
		os.Exit(1)
	}
	name := os.Args[1]
	addr := net.ParseIP(name)
	if addr == nil {
		Println("地址不可用")
	} else {
		Println("地址为", addr)
	}
	os.Exit(0)
}
