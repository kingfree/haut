require 'nokogiri'
require "open-uri"
require "json"
require "scanf"

url = "http://saimoe.thread.jp/test/read.cgi/ast/"
pid = 1405689525

code_name = /\[\[AS14.*\]\]-[0-9]{5}/
item_name = /\s*&lt;&lt;(.+?)&gt;&gt;\s*/

path = "D:\\二次元\\最萌\\2014日萌\\一予投票\\"
pid = ARGV[1] ? ARGV[1] : "y1"
rgt = File.open("#{path}#{pid}.txt") { |f| f.read }
rgt = rgt.force_encoding(Encoding::Shift_JIS)
deban = Array.new
rgt.each_line { |e| deban << e.strip }
# deban.each { |e| $stderr.print e, "" }

list = Hash.new

path = "C:\\Program Files (x86)\\Jane Style\\Logs\\2ch\\saimoe\\2014\\"
pid = ARGV[0] ? ARGV[0] : 1405689525
page = File.open("#{path}#{pid}.dat") { |f| f.read }
page = page.force_encoding(Encoding::Shift_JIS)
# page.encode!(Encoding::UTF_8, Encoding::Shift_JIS)

page.each_line do |line|
  next if line.match(code_name) == nil
  line.scan(item_name) do |chas|
    cha = "<<#{chas[0]}>>"
    next if deban.index(cha).nil?
    # print cha
    list[cha] = 0 if !list.key?(cha)
    list[cha] += 1
  end
end

list = list.sort_by {|k, x| -x }
i, j = 0, 0
list.each do |k, e|
  i += 1
  j = i if i <= 1 or e != list[i - 2][1]
  printf "%2d位 %3d票 %s\n", j, e, k.encode(Encoding::UTF_8)
end
