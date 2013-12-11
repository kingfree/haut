# n 的 m 划分
def q(n, m)
  return 0 if n < 1 or m < 1
  return 1 if n == 1 or m == 1
  return q(n, n) if n < m
  return q(n, m - 1) + 1 if n == m
  q(n, m - 1) + q(n - m, m)
end

n = gets.to_i
m = gets.to_i
s = q(n, m)
puts "q(#{n}, #{m}) = #{s}"

