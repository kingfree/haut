package main

import (
	"encoding/json"
	. "fmt"
)

type Server struct {
	ServerName string
	ServerIP   string
}

type Serverslice struct {
	Servers []Server
}

func main() {
	var s Serverslice
	str := `{"servers":[{"serverName":"京都", "serverIP":"127.0.0.1"}, {"serverName":"東京", "serverIP":"127.0.0.2"}]}`
	json.Unmarshal([]byte(str), &s)
	Println(s)

	var x Serverslice
	x.Servers = append(x.Servers, Server{
		ServerName: "長野",
		ServerIP:   "1.2.3.4",
	})
	x.Servers = append(x.Servers, Server{
		ServerName: "奈良",
		ServerIP:   "5.6.7.8",
	})
	b, _ := json.Marshal(x)
	Println(b)
	Println(string(b))
}
