package main

import (
	. "fmt"
	"net"
	"os"
	"time"
)

func main() {
	Println("启动 TCP 服务")
	service := ":1200"
	tcpAddr, err := net.ResolveTCPAddr("tcp4", service)
	checkErr(err)
	listener, err := net.ListenTCP("tcp", tcpAddr)
	checkErr(err)
	for {
		con, err := listener.Accept()
		if err != nil {
			Fprintf(os.Stderr, "%s", err)
			continue
		}
		go handleClient(con)
	}
}

func handleClient(con net.Conn) {
	defer con.Close()
	daytime := time.Now().String()
	Println(daytime)
	con.Write([]byte(daytime))
}

func checkErr(err error) {
	if err != nil {
		Fprintf(os.Stderr, "错误：%s\n", err.Error())
		os.Exit(1)
	}
}
