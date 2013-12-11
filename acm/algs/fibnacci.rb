
def f1(n)
  n <= 1 ? 1 : f1(n - 1) + f1(n - 2)
end

def f(n)
  s = n.times.each_with_object([0, 1]) do |i, o|
    o << o[-2] + o[-1]
  end
  s[-1]
end

n = gets.to_i
puts f(n)

