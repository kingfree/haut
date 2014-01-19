package main

import (
	. "fmt"
	"os"
)

var locales map[string]map[string]string

func main() {
	locales = make(map[string]map[string]string, 3)
	en := make(map[string]string, 10)
	en["hello"] = "hello"
	en["world"] = "world"
	locales["en"] = en
	cn := make(map[string]string, 10)
	cn["hello"] = "你好"
	cn["world"] = "世界"
	locales["zh-CN"] = cn
	jp := make(map[string]string, 10)
	jp["hello"] = "こんにちは"
	jp["world"] = "せかい"
	locales["jp"] = jp
	lang := "zh-CN"
	if len(os.Args) > 1 {
		lang = os.Args[1]
	}
	Println(msg(lang, "hello"))
	Println(msg(lang, "world"))
}

func msg(locale, key string) string {
	if v, ok := locales[locale]; ok {
		if w, ok := v[key]; ok {
			return w
		}
	}
	return ""
}
