
def merge(左, 右)
  结果 = []
  until 左.empty? or 右.empty?
    结果 << (左.first < 右.first ? 左.shift : 右.shift)
  end
  结果 + 左 + 右
end

def merge_sort(数组)
  return 数组 if 数组.size < 2
  左 = 数组.first(数组.size / 2)
  右 = 数组.last(数组.size - 数组.size / 2)
  merge(merge_sort(左), merge_sort(右))
end

n = gets.to_i
a = Array.new(n) { |i| rand(n * 10) }
p a
p merge_sort(a)

