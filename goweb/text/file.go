package main

import (
	. "fmt"
	"os"
)

func main() {
	os.Mkdir("tmp", 0777)
	os.MkdirAll("tmp/test/tesuto", 0777)
	err := os.Remove("tmp")
	if err != nil {
		Println(err)
	}
	os.RemoveAll("tmp")

	u := "koko.txt"
	fout, err := os.Create(u)
	if err != nil {
		Println(u, err)
		return
	}
	Printf("创建 %s 成功\n", u)
	defer fout.Close()
	for i := 0; i < 5; i++ {
		fout.WriteString("ただのテストだけ、\r\n")
		fout.Write([]byte("只是一个测试罢了，\r\n"))
	}

	fin, err := os.Open(u)
	defer fin.Close()
	if err != nil {
		Println(u, err)
		return
	}
	Printf("打开 %s 成功\n", u)
	buf := make([]byte, 1024)
	for {
		n, _ := fin.Read(buf)
		if n == 0 {
			break
		}
		os.Stdout.Write(buf[:n])
	}

	err = os.Remove(u)
	for err != nil {
		Println(err)
		fin.Close()
		fout.Close()
		err = os.Remove(u)
	}
	Printf("删除 %s 成功\n", u)
}
