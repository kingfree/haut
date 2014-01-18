package main

import (
	. "fmt"
	"io/ioutil"
	"net/http"
	"regexp"
	"strings"
)

func main() {
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
}
