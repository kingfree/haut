package main

import "fmt"

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

func main() {
	x, y, z := 3, 4, 5
	a := max(x, y)
	b := max(x, z)
	s := "max(%d, %d) = %d\n"
	fmt.Printf(s, x, y, a)
	fmt.Printf(s, x, z, b)
	fmt.Printf(s, y, z, max(y, z))
}
