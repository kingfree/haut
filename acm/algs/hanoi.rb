
def 移动(n, a, b)
  puts "#{a} -> #{b}"
end

def 汉诺(n, a, b, c)
  if n > 0
    汉诺(n - 1, a, c, b)
    移动(n, a, b)
    汉诺(n - 1, c, b, a)
  end
end

汉诺(3, '甲', '乙', '丙')

