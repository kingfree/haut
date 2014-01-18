package main

import "fmt"

type person struct {
	姓名 string
	年龄 int
}

func 长者(a, b person) (person, int) {
	if a.年龄 > b.年龄 {
		return a, a.年龄 - b.年龄
	}
	return b, b.年龄 - a.年龄
}

func main() {
	var 张三 person
	张三.姓名, 张三.年龄 = "张三", 18
	李四 := person{年龄: 25, 姓名: "李四"}
	王五 := person{"王五", 43}
	ao, ad := 长者(张三, 李四)
	bo, bd := 长者(王五, 李四)
	co, cd := 长者(张三, 王五)
	s := "[%s, %s] %s 大 %d 岁\n"
	fmt.Printf(s, 张三.姓名, 李四.姓名, ao.姓名, ad)
	fmt.Printf(s, 王五.姓名, 李四.姓名, bo.姓名, bd)
	fmt.Printf(s, 张三.姓名, 王五.姓名, co.姓名, cd)
}
