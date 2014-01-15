package main
import "fmt"

type testInt func(int) bool

func isOdd(a int) bool {
    if a % 2 == 0 {
        return false
    }
    return true
}

func isEven(a int) bool {
    return !isOdd(a)
}

func filter(slice []int, f testInt) []int {
    var result []int
    for _, value := range slice {
        if f(value) {
            result = append(result, value)
        }
    }
    return result
}

func main() {
    slice := []int {1, 2, 3, 4, 5, 7}
    fmt.Println("slice = ", slice)
    odd := filter(slice, isOdd)
    fmt.Println("slice 的奇数元素为：", odd)
    even := filter(slice, isEven)
    fmt.Println("slice 的偶数元素为：", even)
}