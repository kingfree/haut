def call_block
	puts "方法开始"
	yield
	yield
	puts "方法结束"
end

call_block { puts "在块中" }

animes = %w{ Saki Yuruyuri Kiniro Nonnon Minamike }
animes.each {|a| puts "《#{a}》" }

10.times { print "*" }
1.upto(10) {|i| print i }
('A'..'z').each {|c| print c }
