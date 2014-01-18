package main

import (
	"crypto/md5"
	. "fmt"
	"html/template"
	"io"
	"log"
	. "net/http"
	"os"
	"strconv"
	"time"
)

func index(w ResponseWriter, r *Request) {
	t, _ := template.ParseFiles("index.html")
	t.Execute(w, nil)
}

func login(w ResponseWriter, r *Request) {
	Println("方法：", r.Method)
	if r.Method == "GET" {
		crutime := time.Now().Unix()
		h := md5.New()
		io.WriteString(h, strconv.FormatInt(crutime, 10))
		token := Sprintf("%x", h.Sum(nil))

		t, _ := template.ParseFiles("login.html")
		t.Execute(w, token)
	} else {
		r.ParseForm()
		token := r.Form.Get("token")
		if token != "" {
			Println("标识：", token)
			// 验证合法性
		} else {
			Println("标识：未获取")
			// 报错
		}
		Println(r)
		Println("用户名长度：", len(r.Form["username"][0]))
		Println("用户名：", template.HTMLEscapeString(r.Form.Get("username")))
		Println("密码：", template.HTMLEscapeString(r.Form.Get("password")))
		template.HTMLEscape(w, []byte(r.Form.Get("username")))
	}
}

func upload(w ResponseWriter, r *Request) {
	Println("方法：", r.Method)
	if r.Method == "GET" {
		crutime := time.Now().Unix()
		h := md5.New()
		io.WriteString(h, strconv.FormatInt(crutime, 10))
		token := Sprintf("%x", h.Sum(nil))

		t, _ := template.ParseFiles("upload.html")
		t.Execute(w, token)
	} else {
		r.ParseMultipartForm(32 << 20)
		file, handler, err := r.FormFile("uploadfile")
		if err != nil {
			Println(err)
			return
		}
		defer file.Close()
		Fprintf(w, "%v", handler.Header)
		f, err := os.OpenFile("./tmp/"+handler.Filename,
			os.O_WRONLY|os.O_CREATE, 0666)
		if err != nil {
			Println(err)
			return
		}
		defer f.Close()
		io.Copy(f, file)
	}
}

func main() {
	HandleFunc("/", index)
	HandleFunc("/login", login)
	HandleFunc("/upload", upload)
	Println("启动服务器……")
	err := ListenAndServe(":80", nil)
	if err != nil {
		log.Fatal("监听服务: ", err)
	}
}
