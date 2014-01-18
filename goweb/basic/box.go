package main

import "fmt"

const (
	白 = iota
	黑
	蓝
	红
	黄
)

type Color byte

type Box struct {
	宽度, 高度, 深度 float64
	颜色         Color
}

type Boxlist []Box

func (b Box) 体积() float64 {
	return b.宽度 * b.高度 * b.深度
}

func (b *Box) 置颜色(c Color) {
	b.颜色 = c
}

func (bl Boxlist) 最大的颜色() Color {
	v := 0.00
	k := Color(白)
	for _, b := range bl {
		if b.体积() > v {
			v = b.体积()
			k = b.颜色
		}
	}
	return k
}

func (bl Boxlist) 染黑() {
	for i, _ := range bl {
		bl[i].置颜色(黑)
	}
}

func (c Color) String() string {
	字串 := []string{"白色", "黑色", "蓝色", "红色", "黄色"}
	return 字串[c]
}

func main() {
	boxes := Boxlist{
		Box{4, 4, 4, 红},
		Box{10, 10, 1, 黄},
		Box{1, 1, 20, 黑},
		Box{10, 10, 1, 蓝},
		Box{10, 30, 1, 白},
		Box{20, 20, 20, 黄},
	}
	fmt.Printf("我们有 %d 个盒子\n", len(boxes))
	// Println 会自动添加空格
	fmt.Println("第一个盒子的体积是", boxes[0].体积(), "cm³")
	fmt.Println("最后一个盒子的颜色是", boxes[len(boxes)-1].颜色.String())
	fmt.Println("最大的盒子是", boxes.最大的颜色().String())
	fmt.Println("让我们把它们都染黑……")
	boxes.染黑()
	fmt.Println("第二个盒子的颜色是", boxes[1].颜色.String())
	fmt.Println("显然此时最大的是", boxes.最大的颜色().String())
}
