package main

import (
	"encoding/xml"
	. "fmt"
	"io/ioutil"
	"os"
)

type Recurlyservers struct {
	XMLName     xml.Name `xml:"servers"`
	Version     string   `xml:"version,attr"`
	Svs         []server `xml:"server"`
	Description string   `xml:",innerxml"`
}

type server struct {
	XMLName    xml.Name `xml:"server"`
	ServerName string   `xml:"serverName"`
	ServerIP   string   `xml:"serverIP"`
}

type Servers struct {
	XMLName xml.Name  `xml:"servers"`
	Version string    `xml:"version,attr"`
	Svs     []oserver `xml:"server"`
}

type oserver struct {
	ServerName string `xml:"serverName"`
	ServerIP   string `xml:"serverIP"`
}

func main() {
	file, _ := os.Open("servers.xml")
	defer file.Close()
	data, _ := ioutil.ReadAll(file)
	v := Recurlyservers{}
	xml.Unmarshal(data, &v)
	Println(v)

	w := &Servers{Version: "1"}
	w.Svs = append(w.Svs, oserver{"東京", "1.2.3.4"})
	w.Svs = append(w.Svs, oserver{"京都", "5.6.7.8"})
	ouput, _ := xml.MarshalIndent(w, "  ", "    ")
	os.Stdout.Write([]byte(xml.Header))
	os.Stdout.Write(ouput)
}
