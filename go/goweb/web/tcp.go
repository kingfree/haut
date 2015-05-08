package main

import (
	. "fmt"
	"io/ioutil"
	"net"
	"os"
)

func main() {
	if len(os.Args) != 2 {
		Fprintf(os.Stderr, "用法：%s <主机>:<端口>\n", os.Args[0])
		os.Exit(1)
	}
	service := os.Args[1]
	tcpAddr, err := net.ResolveTCPAddr("tcp4", service)
	checkErr(err)
	conn, err := net.DialTCP("tcp", nil, tcpAddr)
	checkErr(err)
	_, err = conn.Write([]byte("HEAD / HTTP/1.0\n\n"))
	checkErr(err)
	result, err := ioutil.ReadAll(conn)
	checkErr(err)
	Println(string(result))
	os.Exit(0)
}

func checkErr(err error) {
	if err != nil {
		Fprintf(os.Stderr, "错误：%s\n", err.Error())
		os.Exit(1)
	}
}
