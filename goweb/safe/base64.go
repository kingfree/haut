package main

import (
	"encoding/base64"
	. "fmt"
)

func 编码(src []byte) []byte {
	return []byte(base64.StdEncoding.EncodeToString(src))
}

func 解码(src []byte) ([]byte, error) {
	return base64.StdEncoding.DecodeString(string(src))
}

func main() {
	hello := "おはよう、世界！"
	Println(hello)
	debyte := 编码([]byte(hello))
	Println(string(debyte))
	enbyte, err := 解码(debyte)
	if err != nil {
		Println(err)
	}
	if string(enbyte) != hello {
		Println("转换有误")
	}
	Println(string(enbyte))
}
