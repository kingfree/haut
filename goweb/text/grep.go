package main

import (
	. "fmt"
	"io/ioutil"
	"net/http"
	"regexp"
	"strings"
)

func main() {
	Println("网页抓取")

	resp, err := http.Get("http://cojs.tk/cogs/")
	if err != nil {
		Println(err)
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		Println(err)
		return
	}
	src := string(body)
	// 将 HTML 标签转成小写
	re, _ := regexp.Compile("\\<[\\S\\s]+?\\>")
	src = re.ReplaceAllStringFunc(src, strings.ToLower)
	// 去除 style
	re, _ = regexp.Compile("\\<style[\\S\\s]+?\\</style>\\>")
	src = re.ReplaceAllString(src, "")
	// 去除 script
	re, _ = regexp.Compile("\\<script[\\S\\s]+?\\</script>\\>")
	src = re.ReplaceAllString(src, "")
	// 去除所有乱七八糟的
	re, _ = regexp.Compile("\\<[\\S\\s]+?\\>")
	src = re.ReplaceAllString(src, "")
	// 去除连续换行符
	re, _ = regexp.Compile("\\s{2,}")
	src = re.ReplaceAllString(src, "\n")

	Println(strings.TrimSpace(src))

	正则应用()

	展开()
}

func 展开() {
	Println("展开")
	src := []byte(`
		call hello azusa
		hello kyouko
		call hello momo
	`)
	pat := regexp.MustCompile(`(?m)(call)\s+(?P<cmd>\w+)\s+(?P<arg>.+)\s*$`)
	res := []byte{}
	for _, s := range pat.FindAllSubmatchIndex(src, -1) {
		res = pat.Expand(res, []byte("$cmd('$arg')\n"), src, s)
	}
	Println(string(res))
}

func 正则应用() {
	Println("正则应用")

	a := "I am learing Go language"
	Println(a)

	re, _ := regexp.Compile("[a-z]{2,4}")

	one := re.Find([]byte(a))
	Println("Find:", string(one))

	all := re.FindAll([]byte(a), -1)
	Print("FindAll:")
	for _, v := range all {
		Print(" [", string(v), "]")
	}
	Println()

	index := re.FindIndex([]byte(a))
	Println("FindIndex:", index)

	allindex := re.FindAllIndex([]byte(a), -1)
	Println("FindAllIndex:", allindex)

	rf, _ := regexp.Compile("am(.*)lang(.*)")

	submatch := rf.FindSubmatch([]byte(a))
	Print("FindSubmatch:")
	for _, v := range submatch {
		Print(" [", string(v), "]")
	}
	Println()

	submatchindex := rf.FindSubmatchIndex([]byte(a))
	Println("FindSubmatchIndex:", submatchindex)

	submatchall := rf.FindAllSubmatch([]byte(a), -1)
	Print("FindAllSubmatch:")
	for _, w := range submatchall {
		Print(" [")
		for _, v := range w {
			Print("[", string(v), "] ")
		}
		Print("] ")
	}
	Println()

	submatchallindex := rf.FindAllSubmatchIndex([]byte(a), -1)
	Println("FindAllStringSubmatchIndex:", submatchallindex)
}
