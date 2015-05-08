package main

import (
	. "fmt"
	"github.com/drone/routes"
	. "net/http"
)

func getuser(w ResponseWriter, r *Request) {
	p := r.URL.Query()
	uid := p.Get(":uid")
	Fprintln(w, "获取用户", uid)
}

func edituser(w ResponseWriter, r *Request) {
	p := r.URL.Query()
	uid := p.Get(":uid")
	Fprintln(w, "更改用户", uid)
}

func deluser(w ResponseWriter, r *Request) {
	p := r.URL.Query()
	uid := p.Get(":uid")
	Fprintln(w, "删除用户", uid)
}

func adduser(w ResponseWriter, r *Request) {
	p := r.URL.Query()
	uid := p.Get(":uid")
	Fprintln(w, "添加用户", uid)
}

func main() {
	Println("启动 REST 服务器")
	mux := routes.New()
	mux.Get("/user/:uid", getuser)
	mux.Post("/user/:uid", edituser)
	mux.Del("/user/:uid", deluser)
	mux.Put("/user/:uid", adduser)
	Handle("/", mux)
	ListenAndServe(":8088", nil)
}
