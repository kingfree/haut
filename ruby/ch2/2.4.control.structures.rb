print "输入数字："
n = gets.to_i
puts "#{n} 是负数！" if n < 0
if n % 2 == 1
    puts "#{n} 是奇数。"
elsif n == 0
    puts "#{n} 是零。"
else
    puts "#{n} 是偶数。"
end

while line = gets and line[0] != '0'
    puts line.downcase
end
