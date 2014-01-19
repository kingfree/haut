package main

import (
	. "fmt"
	"net/rpc/jsonrpc"
	"os"
)

type Args struct {
	A, B int
}

type Quotient struct {
	Quo, Rem int
}

func main() {
	if len(os.Args) != 2 {
		Fprintf(os.Stderr, "用法：%s <服务器>:<端口>\n", os.Args[0])
		os.Exit(1)
	}
	addr := os.Args[1]
	client, err := jsonrpc.Dial("tcp", addr)
	checkErr(err)

	args := Args{17, 8}

	var reply int
	err = client.Call("Arith.Multiply", args, &reply)
	checkErr(err)
	Printf("Arith: %d × %d = %d\n", args.A, args.B, reply)

	var quo Quotient
	err = client.Call("Arith.Divide", args, &quo)
	checkErr(err)
	Printf("Arith: %d ÷ %d = %d ... %d\n", args.A, args.B, quo.Quo, quo.Rem)
}

func checkErr(err error) {
	if err != nil {
		Fprintln(os.Stderr, "错误：", err)
		os.Exit(1)
	}
}
