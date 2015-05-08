package main

import (
	. "fmt"
	"html/template"
	"os"
	"strings"
)

type Friend struct {
	Fname string
}

type Person struct {
	UserName string
	Emails   []string
	Friends  []*Friend
}

func main() {
	嵌套输出()
	条件处理()
	模板函数()
	嵌套模板()
}

func 嵌套模板() {
	Println("嵌套模板")
	s, _ := template.ParseFiles("header.html", "content.html", "footer.html")
	s.ExecuteTemplate(os.Stdout, "header", nil)
	s.ExecuteTemplate(os.Stdout, "content", nil)
	s.ExecuteTemplate(os.Stdout, "footer", nil)
	s.Execute(os.Stdout, nil) // 默认没有输出
}

func 处理邮件(args ...interface{}) string {
	ok := false
	var s string
	if len(args) == 1 {
		s, ok = args[0].(string)
	}
	if !ok {
		s = Sprint(args...)
	}
	ss := strings.Split(s, "@")
	if len(ss) != 2 {
		return s
	}
	return (ss[0] + "#" + ss[1])
}

func 模板函数() {
	Println("模板函数")
	f1 := Friend{Fname: "片岡優希"}
	f2 := Friend{Fname: "池田華菜"}
	t := template.New("fieldname example")
	t = t.Funcs(template.FuncMap{"emailDeal": 处理邮件})
	t, _ = t.Parse(`
        おはよう {{.UserName}} さん
        {{range .Emails}}
        メール一つ {{. | emailDeal}}
        {{end}}
        {{with .Friends}}
        {{range .}}
        友達は {{.Fname}}
        {{end}}
        {{end}}
    `)
	p := Person{UserName: "吉田未春",
		Emails:  []string{"miharu@yoshi.da", "haru@yahoo.jp"},
		Friends: []*Friend{&f1, &f2},
	}
	t.Execute(os.Stdout, p)
	Println()
}

func 条件处理() {
	Println("条件处理")
	e := template.New("template test")
	e = template.Must(e.Parse("空 {{if ``}}不会输出{{end}}\n"))
	e.Execute(os.Stdout, nil)
	w := template.New("template test")
	w = template.Must(w.Parse("非空 {{if `anything`}}会输出{{end}}\n"))
	w.Execute(os.Stdout, nil)
	i := template.New("template test")
	i = template.Must(i.Parse("{{if `anything`}}如果{{else}}否则{{end}}\n"))
	i.Execute(os.Stdout, nil)
}

func 嵌套输出() {
	Println("嵌套输出")
	f1 := Friend{Fname: "片岡優希"}
	f2 := Friend{Fname: "池田華菜"}
	t := template.New("fieldname example")
	t, _ = t.Parse(`
        おはよう　{{.UserName}}　さん
        {{range .Emails}}
        メール一つ{{.}}
        {{end}}
        {{with .Friends}}
        {{range .}}
        友達は{{.Fname}}
        {{end}}
        {{end}}
    `)
	p := Person{UserName: "吉田未春",
		Emails:  []string{"miharu@yoshi.da", "haru@yahoo.jp"},
		Friends: []*Friend{&f1, &f2},
	}
	t.Execute(os.Stdout, p)
	Println()
}
