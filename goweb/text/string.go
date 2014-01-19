package main

import (
	. "fmt"
	. "strconv"
	. "strings"
)

func main() {
	Println(Contains("seefood", "foo"))
	Println(Contains("seefood", "bar"))
	Println(Contains("seefood", ""))
	Println(Contains("", ""))

	s := []string{"foo", "bar", "baz"}
	Println(Join(s, ", "))

	Println(Index("chicken", "ken"))
	Println(Index("chicken", "dmr"))

	Println("ba" + Repeat("na", 2))

	Println(Replace("ありがどうございます", "ます", "ました", -1))
	Println(Replace("arigadougozaimasu", "a", "あ", 2))

	Printf("%q\n", Split("a,b,c", ","))
	Printf("%q\n", Split("a man a plan a canal panama", "a "))
	Printf("%q\n", Split(" xyz ", ""))
	Printf("%q\n", Split("", "balabala"))

	Printf("%q\n", Trim(" !!! Achtung !!! ", "! "))

	Printf("%q\n", Fields(" o kekkon omedetou  "))

	str := make([]byte, 0, 100)
	str = AppendInt(str, 4567, 10)
	str = AppendBool(str, false)
	str = AppendQuote(str, "dareyori")
	str = AppendQuoteRune(str, 'も')
	Println(string(str))

	a := FormatBool(false)
	b := FormatFloat(3.1415, 'g', 12, 64)
	c := FormatInt(32767, 10)
	d := FormatUint(1087, 10)
	e := Itoa(1023)
	Println(a, b, c, d, e)

	p, _ := ParseBool(a)
	y, _ := ParseFloat(b, 64)
	f, _ := ParseInt(c, 10, 64)
	g, _ := ParseUint(d, 10, 64)
	r, _ := Atoi(e)
	Println(p, y, f, g, r)
}
