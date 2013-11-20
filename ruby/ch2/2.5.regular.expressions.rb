while line = gets
	if line =~ /P(erl|ython)/
		puts "提到编程语言：#{line}"
	end
end
