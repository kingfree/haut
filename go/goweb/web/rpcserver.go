package main

import (
	"errors"
	. "fmt"
	"net"
	"net/rpc"
	"net/rpc/jsonrpc"
	"os"
)

type Args struct {
	A, B int
}

type Quotient struct {
	Quo, Rem int
}

type Arith int

func (t *Arith) Multiply(args *Args, reply *int) error {
	*reply = args.A * args.B
	return nil
}

func (t *Arith) Divide(args *Args, quo *Quotient) error {
	if args.B == 0 {
		return errors.New("除数不得为零")
	}
	quo.Quo = args.A / args.B
	quo.Rem = args.A % args.B
	return nil
}

func main() {
	arith := new(Arith)
	rpc.Register(arith)
	addr, err := net.ResolveTCPAddr("tcp", ":1234")
	checkErr(err)
	lisr, err := net.ListenTCP("tcp", addr)
	checkErr(err)
	Println("启动 RPC 服务器 (JSON)")
	for {
		con, err := lisr.Accept()
		if err != nil {
			continue
			Fprintln(os.Stderr, "错误：", err)
		}
		jsonrpc.ServeConn(con)
	}
}

func checkErr(err error) {
	if err != nil {
		Fprintln(os.Stderr, "错误：", err)
		os.Exit(1)
	}
}
