# 《程序员的数学》结城浩

## 第1章 0的故事——无即是有

### 十进制计数法

使用十种数字，数位有一定的意义。

基数、底，指数

### 二进制计数法

基数转换

### 按位计数法

8八进制、十六进制、N进制

罗马计数法不是按位计数法：

1. 数位无意义，只表示数字本身
2. 没有 0
3. 使用IVXLCDM来计数
4. 将并排的数字加起来

### 指数法则

定义10^0=1

### 0所起的作用

统一标准，简化规则

### 为什么发明计数法

将大问题分解为小的单元

## 第2章 逻辑——真与假的二元世界

### 逻辑的重要性

逻辑是消除歧义的工具

### 兼顾完整性和排他性

没有遗漏，没有重复，注意边界值

### 建立复杂命题

真值表，文氏图

1. 逻辑非：不是A，¬A (not A)。双重否定等于肯定
2. 逻辑与：A并且B，A∧B (A and B)
3. 逻辑或：A或者B，A∨B (A or B)
4. 异或：A或者B（但不都满足），A⊕B (A xor B)
5. 相等：A和B相等，A=B
6. 蕴含：若A则B，A=>B

A和B的真假组合共4种，每种组合都有两种运算结果，共有2^4=16种运算种类。

使用二进制表示则可以完整表示出0x0~0xF这16个数字：见代码[`truetable.c`](truetable.c)

### 德摩根定律

* ¬A∨¬B = ¬(A∧B)
* ¬A∧¬B = ¬(A∨B)

对偶性

### 卡诺图

将说有命题的真假组合以二维表形式表示，然后分解组合为1*1,1*2,1*4,2*2,4*4的的网格，根据其所在行列真假情况写出简化的表达式。

**重要**：需要图示

### 包含未定义的三值逻辑

true/false/undefined

1. 带条件的逻辑与（A && B）：A真同B，A假恒假，A未恒未
2. 带条件的逻辑或（A || B）: A假同B，A真恒真，A未恒未
3. 三值逻辑中的否定（!A）：非真为假，非假为真，非未为未

德摩根定律适用

列举量为3^9

## 第3章 余数——周期性和分组

### 余数分组

今天日曜日，100天之后是何曜日？100÷7=14…2，火曜日。

余数的力量：将较大的数字除一次就能分组。

今天日曜日，10^100天之后是何曜日？100÷6=16…4，木曜日。

1234567^987654321的个位数是？循环节1793，987654321÷4余1，答案为7。

运用余数，大数字的问题就能简化成小数字的问题。

### 奇偶校验

奇偶校验位将数字分为两个集合。

* 通过黑白棋通信
* 寻找恋人的思考题
* 铺设草席的思考题
* 一笔画的思考题：七桥问题：只有0或2个奇点

## 第4章 数学归纳法——征服无穷数列

### 高斯求和

首项加末项乘以项数除以二

### 数学归纳法——征服无穷数列

1. 证明“P(0)成立”；
2. 证明不论k为0以上的哪个整数，“若P(k)成立，则P(k+1)也成立”。

实例：求出奇数的和

错误的归纳：投掷硬币正反面一致

### 编程和数学归纳法

证明P(n)成立的程序：[`prove.c`](prove.c)

    在学校学习数学归纳法之初，我不是很理解这个结构。虽说等式的计算并没有
    那么难，但我不认为数学归纳法是有效的证明方法。当初我搞不明白的是步骤2。
    在步骤2中，是要假设P(k)成立，推导出P(k+1)。我当时却想：“P(k)不是现在
    要证明的式子吗？如果这样假设就谈不上证明了吧。”现在想起来，就相当于
    把`prove`函数的输入**参数n**和其中使用的**本地变量k**混为一谈了。

循环不变式：保持断言M(n)成立：[`sum.c`](sum.c)

## 第5章 排列组合——解决计数问题的方法

计数是与整数的对应关系。

计数时必须注意不“重复”，不“遗漏”。

在编程中，作为普遍规则来掌握：**第k个数据是第k-1号**。

### 集合法则

* 加法法则：|A∪B| = |A| + |B|，其中A、B无交集
* 容斥原理：|A∪B| = |A| + |B| - |A∩B|
* 乘法法则：|A×B| = |A| × |B|

### 置换、排列、组合

将n个事物按顺序进行排列叫**置换**，S(n) = n!。

从n个事物中取出k个进行置换叫**排列**，P(n,k) = n! / (n-k)!。

先考虑顺库进行计数，然后除以重复度，是计算组合时常用的计算方法。

从n个事物中取出k个而不考虑顺序叫**组合**，C(n,k) = n! / k!(n-k)!。

## 第6章 递归——自我定义

### 汉诺塔

H(n) = H(n-1) + 1 + H(n-1), H(0) = 0

H(n) = 2^n - 1

汉诺塔解法的程序：[`hanoi.c`](hanoi.c)

### 再谈阶乘

n! = n × (n-1)!, 0! = 1

递归和归纳的本质都是“将复杂问题简化”。

### 斐波那契数列

兔子繁殖问题。

F(n) = F(n-1) + F(n-2), F(0) = 0, F(1) = 1

摆砖头问题、创作旋律问题

### 杨辉三角形（帕斯卡三角形）

C(n, k) = C(n-1, k-1) + C(n-1, k)

找出复杂问题中隐含的递归结构：

* 从整体问题中隐去部分问题
* 判断剩余部分和整体问题是同类问题

### 递归图形

描绘n层树枝的函数：

    drawtree(n)
    1 if n > 0
    2   left            // 左转一定角度
    3   forward(n)      // 前进n步并划线（画出第n层的树枝）
    4   darawtree(n-1)
    5   back(n)         // 后退n步不划线
    6   right           // 右转一定角度
    7
    8   right
    9   forward(n)
    10  darawtree(n-1)
    11  back(n)
    12  left

谢尔平斯基三角形：用颜色区分出杨辉三角形的奇数和偶数。

## 第7章 指数爆炸——解决复杂问题

折纸问题：一张1mm的纸对折39次(54万公里)就超过了地月距离(39万公里)。

### 倍数游戏——指数爆炸引发的难题

程序中的设置选项，需要2^(复选框数)次测试才可能全部覆盖。

要一个不漏个测试设定选项的所有可能性是不现实的。

不能认为是“有限的”就不假思索。

### 二分法查找

关键在于，每判断一次就能筛选出近一半的查找对象，多判断一次就能从近两倍的查找对象中找出目标数据。

### 对数

对数是解决指数爆炸的相应手段。

lg(A×B) = lg A + lg B

### 密码

密钥的的字长越长，暴力破解越费时。

### 如何处理指数爆炸

1. 极力求解
2. 变相求解
3. 近似求解
4. 概率求解

## 第8章 不可解问题——无法编写的程序

### 反证法

反证法就是：先假设命题的否定形式成立，然后再进行推理，引出矛盾。亦称归谬法。

整数是无穷的。质数是无穷的。

### 可数

集合的元素是有限的，或者集合中的所有元素都与正整数一一对应，这个集合就是可数的。元素可按一定规律既无重复也无遗漏地输出来。

可数集合：有限集合、0以上所有偶数的集合、所有整数的集合、所有有理数的集合、所有程序（符合编程语言语法的有限的字符的排列）的集合……

### 对角论证法

不可数集合：所有整数数列的集合、所有实数的集合、所有函数的集合、所有程序生成的整数数列的集合……

*一般地，无穷是可数的，无穷的无穷是不可数的。*

### 不可解问题

不可解问题是“原则上不能用程序来解决的问题”。不能写成程序的函数是存在的。

### 停机问题

停机问题就是‘判断“某程序在给定数据下，是否会在有限时间内结束运行”的问题’。

## 第9章 总结——程序员的数学

### 何为解决问题

* 认清模式，进行抽象化
* 由不擅长催生出的智慧
* 幻想法则
