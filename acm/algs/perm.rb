
def perm(a, k)
  raise ArgumentError, "要求传入数组实例" unless a.instance_of? Array
  if k == a.length
    puts a.inspect
  else
    for i in k .. a.length - 1
      swap(a, k, i)
      perm(a, k + 1)
      swap(a, k, i)
    end
  end
end

def swap(a, i, j)
  tmp = a[i]
  a[i] = a[j]
  a[j] = tmp
end

n = gets.to_i
a = Array.new(n) { |i| i + 1 }
puts "原数组："
puts a.inspect
puts "全排列："
perm a, 0
puts "库函数："
a.permutation.to_a.each { |i| puts i.inspect }

