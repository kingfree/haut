require 'rubygems'
require 'active_support'

n = ARGV[0].to_i
print "话数: #{n}\n"
a = ARGV[1].split(/[-\/\s]/)
s = Time.new(a[0], a[1], a[2])
print "放送开始(#{s}): \n"

n.times do |i|
    i += 1
    printf "%d|%s|%s|%s|%s\n", i, "", "", "24m", s.strftime("%Y-%m-%d")
    s += 60*60*24*7
end
