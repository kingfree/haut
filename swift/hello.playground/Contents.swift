// swiftguide.cn
// 1.2 Swift 初见

println("你好，世界！")

var 变量 = 42
变量 = 50
let 常量 = 42

let 推断整数 = 70
let 推断双精度数 = 70.0
let 指定双精度数: Double = 70

let 指定浮点数: Float = 80

let 标签 = "宽度是 "
let 宽度 = 94
let 宽度标签 = 标签 + String(宽度)

let 苹果 = 3
let 橘子 = 5
let 苹果总结 = "我有 \(苹果) 个苹果。"
let 水果总结 = "我有 \(苹果 + 橘子) 个水果。"

let 名字 = "小倉唯"
let 招呼 = "你好，\(名字)！\(指定浮点数)"

var 星期 = ["日", "月", "火", "水", "木", "金", "土"]
星期[1] = "一"

var 爱称 = [
    "東山奈央": "闹",
    "水樹奈々": "77",
]
爱称["喜多村英梨"] = "酋长"

let 空数组 = [String]()
let 空字典 = [String: Float]()

星期 = []
爱称 = [:]

let 分数表 = [75, 43, 103, 87, 12]
var 队分数 = 0
for 分数 in 分数表 {
    if 分数 > 50 {
        队分数 += 3
    } else {
        队分数 += 1
    }
}
队分数

var 可选字符串: String? = "こんにちは"
可选字符串 = nil

var 可选名字: String? = "能登麻美子"
var 祝贺 = "你好"
可选名字 = nil
if let 名 = 可选名字 {
    祝贺 = "你好，\(名)"
} else {
    祝贺 = "你好，早見沙織"
}

let 蔬菜 = "红椒"
switch 蔬菜 {
case "芹菜":
    let 评论 = "加点葡萄干做成“原木上的蚂蚁”。"
case "黄瓜", "水芥菜":
    let 评论 = "能做成好茶三明治。"
case let x where x.hasSuffix("椒"):
    let 评论 = "这个\(x)是香辛料吗？"
default:
    let 评论 = "每种蔬菜都很美味。"
}

let 有趣的数字 = [
    "素数": [2, 3, 5, 7, 11, 13],
    "斐波那契数": [1, 1, 2, 3, 5, 8],
    "平方数": [1, 4, 9, 16, 25],
]
var 最大值 = 0
var 最大列: String? = nil
for (类别, 数字) in 有趣的数字 {
    for 数 in 数字 {
        if 数 > 最大值 {
            最大列 = 类别
            最大值 = 数
        }
    }
}
最大列
最大值

var n = 2
while n < 100 {
    n = n * 2
}
n

var m = 2
do {
    m = m * 2
} while m < 100
m

var firstForLoop = 0
for i in 0..<4 {
    firstForLoop += i
}
firstForLoop

var secondForLoop = 0
for var i = 0; i < 4; ++i {
    secondForLoop += i
}
secondForLoop

// 函数
func greet(name: String, day: String) -> String {
    return "你好 \(name)，今天是 \(day)."
}
greet("小明", "月曜日")

// 多个返回值
func calculateStatistics(scores: [Int]) -> (min: Int, max: Int, sum: Int) {
    var min = scores[0]
    var max = scores[0]
    var sum = 0
    
    for score in scores {
        if score > max {
            max = score
        } else if score < min {
            min = score
        }
        sum += score
    }
    
    return (min, max, sum)
}
let statistics = calculateStatistics([5, 3, 100, 3, 9])
statistics.sum
statistics.2

// 多个参数
func sumOf(numbers: Int...) -> Int {
    var sum = 0
    for number in numbers {
        sum += number
    }
    return sum
}
sumOf()
sumOf(42, 597, 12)

// 函数嵌套
func returnFifteen() -> Int {
    var y = 10
    func add() {
        y += 5
    }
    add()
    return y
}
returnFifteen()

// 函数作为返回值
func makeIncrementer() -> (Int -> Int) {
    func addOne(number: Int) -> Int {
        return 1 + number
    }
    return addOne
}
var increment = makeIncrementer()
increment(7)

// 函数作为参数
func hasAnyMatches(list: [Int], condition: Int -> Bool) -> Bool {
    for item in list {
        if condition(item) {
            return true
        }
    }
    return false
}
func lessThanTen(number: Int) -> Bool {
    return number < 10
}
var numbers = [20, 19, 7, 12]
hasAnyMatches(numbers, lessThanTen)

// 闭包
numbers.map({
    (number: Int) -> Int in
    let result = 3 * number
    return result
})

// 忽略类型的闭包
let mappedNumbers = numbers.map({ number in 3 * number })
mappedNumbers

// 指定参数位置（而不是名字）的闭包
let sortedNumbers = sorted(numbers) { $0 > $1 }
sortedNumbers

// 类
class Shape {
    var numberOfSides = 0
    func simpleDescription() -> String {
        return "A shape with \(numberOfSides) sides."
    }
}

// 类的实例化
var shape = Shape()
shape.numberOfSides = 7
var shapeDescription = shape.simpleDescription()

// 类的构造方法
class NamedShape {
    var numberOfSides: Int = 0
    var name: String
    
    init(name: String) {
        self.name = name
    }
    
    func simpleDescription() -> String {
        return "A shape with \(numberOfSides) sides."
    }
}

// 类的继承和方法覆盖
class Square: NamedShape {
    var sideLength: Double
    
    init(sideLength: Double, name: String) {
        self.sideLength = sideLength
        super.init(name: name)
        numberOfSides = 4
    }
    
    func area() ->  Double {
        return sideLength * sideLength
    }
    
    override func simpleDescription() -> String {
        return "A square with sides of length \(sideLength)."
    }
}
let test = Square(sideLength: 5.2, name: "my test square")
test.area()
test.simpleDescription()

// 属性的获取和设置
class EquilateralTriangle: NamedShape {
    var sideLength: Double = 0.0
    
    init(sideLength: Double, name: String) {
        self.sideLength = sideLength
        super.init(name: name)
        numberOfSides = 3
    }
    
    var perimeter: Double {
        get {
            return 3.0 * sideLength
        }
        set {
            sideLength = newValue / 3.0
        }
    }
    
    override func simpleDescription() -> String {
        return "An equilateral triagle with sides of length \(sideLength)."
    }
}
var triangle = EquilateralTriangle(sideLength: 3.1, name: "a triangle")
triangle.perimeter
triangle.perimeter = 9.9
triangle.sideLength

// 设置之后执行代码
class TriangleAndSquare {
    var triangle: EquilateralTriangle {
        willSet {
            square.sideLength = newValue.sideLength
        }
    }
    var square: Square {
        willSet {
            triangle.sideLength = newValue.sideLength
        }
    }
    init(size: Double, name: String) {
        square = Square(sideLength: size, name: name)
        triangle = EquilateralTriangle(sideLength: size, name: name)
    }
}
var triangleAndSquare = TriangleAndSquare(size: 10, name: "another test shape")
triangleAndSquare.square.sideLength
triangleAndSquare.triangle.sideLength
triangleAndSquare.square = Square(sideLength: 50, name: "larger square")
triangleAndSquare.triangle.sideLength

// 方法的参数名要求显式说明
class Counter {
    var count: Int = 0
    func incrementBy(amount: Int, numberOfTimes times: Int) {
        count += amount * times
    }
}
var counter = Counter()
counter.incrementBy(2, numberOfTimes: 7)

// 处理可选值
let optionalSquare: Square? = Square(sideLength: 2.5, name: "optional square")
let sideLength = optionalSquare?.sideLength

// 枚举
enum Rank: Int {
    case Ace = 1
    case Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten
    case Jack, Queen, King
    func simpleDescription() -> String {
        switch self {
        case .Ace:
            return "ace"
        case .Jack:
            return "jack"
        case .Queen:
            return "queen"
        case .King:
            return "king"
        default:
            return String(self.rawValue)
        }
    }
}
let ace = Rank.Ace
let aceRawValue = ace.rawValue

// 枚举转换原始值
if let convertedRank = Rank(rawValue: 3) {
    let threeDescription = convertedRank.simpleDescription()
}

// 枚举的成员值
enum Suit {
    case Spades, Hearts, Diamonds, Clubs
    func simpleDescription() -> String {
        switch self {
        case .Spades:
            return "spades"
        case .Hearts:
            return "hearts"
        case .Diamonds:
            return "diamonds"
        case .Clubs:
            return "clubs"
        }
    }
    
}
let hearts = Suit.Hearts
let heartsDescription = hearts.simpleDescription()

// 结构
struct Card {
    var rank: Rank
    var suit: Suit
    func simpleDescription() -> String {
        return "The \(rank.simpleDescription()) of \(suit.simpleDescription())"
    }
}
let threeOfSpades = Card(rank: .Three, suit: .Spades)
let threeOfSpadesDescription = threeOfSpades.simpleDescription()

enum ServerResponse {
    case Result(String, String)
    case Error(String)
}

let success = ServerResponse.Result("6:00 am", "8:09 pm")
let failure = ServerResponse.Error("Out of cheese.")

switch success {
case let .Result(sunrise, sunset):
    let serverResponse = "Sunrise is at \(sunrise) and sunset is at \(sunset)."
case let .Error(error):
    let serverResponse = "Failure...  \(error)"
}

// 协议（接口）
protocol ExampleProtocol {
    var simpleDescription: String { get }
    mutating func adjust()
}

class SimpleClass: ExampleProtocol {
    var simpleDescription: String = "A very simple class."
    var anotherProperty: Int = 69105
    func adjust() {
        simpleDescription += "  Now 100% adjusted."
    }
}
var a = SimpleClass()
a.adjust()
let aDescription = a.simpleDescription

struct SimpleStructure: ExampleProtocol {
    var simpleDescription: String = "A simple structure"
    mutating func adjust() {
        simpleDescription += " (adjusted)"
    }
}
var b = SimpleStructure()
b.adjust()
let bDescription = b.simpleDescription

// 类的扩展
extension Int: ExampleProtocol {
    var simpleDescription: String {
        return "The number \(self)"
    }
    mutating func adjust() {
        self += 42
    }
}
7.simpleDescription


let protocolValue: ExampleProtocol = a
protocolValue.simpleDescription
// protocolValue.anotherProperty  // 取消该注释以观察错误提示

// 泛型
func repeat<ItemType>(item: ItemType, times: Int) -> [ItemType] {
    var result = [ItemType]()
    for i in 0..<times {
        result.append(item)
    }
    return result
}
repeat("knock", 4)

// Reimplement the Swift standard library's optional type
enum OptionalValue<T> {
    case None
    case Some(T)
}
var possibleInteger: OptionalValue<Int> = .None
possibleInteger = .Some(100)

// 泛型的限定
func anyCommonElements <T, U where T: SequenceType, U: SequenceType, T.Generator.Element: Equatable, T.Generator.Element == U.Generator.Element> (lhs: T, rhs: U) -> Bool {
    for lhsItem in lhs {
        for rhsItem in rhs {
            if lhsItem == rhsItem {
                return true
            }
        }
    }
    return false
}
anyCommonElements([1, 2, 3], [3])
