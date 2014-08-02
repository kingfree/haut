require 'nokogiri'
require "open-uri"
require "json"
require "scanf"

url = "http://saimoe.thread.jp/test/read.cgi/ast/"
pid = 1405689525

code_name = /(\[\[AS14\-.{1,2}\-.{8}\-.{2}\]\]\-[0-9]{5})/
item_name = /\s*&lt;&lt;(.+?)&gt;&gt;\s*/
id_name = /(ID:(.{9})|<><>(.{,9})$)/
ngidnum = 10

path = "D:\\二次元\\最萌\\2014日萌\\一予投票\\"
pid = ARGV[0] ? ARGV[0] : "y1"
rgt = File.open("#{path}#{pid}.txt") { |f| f.read }
rgt.encode!("UTF-8", "Shift_JIS", :invalid => :replace, :undef => :replace)
deban = Array.new
rgt.each_line { |e| deban << e.strip }
# deban.each { |e| $stderr.print e, "\n" }

list = Hash.new
ids = Hash.new

path = "C:\\Program Files (x86)\\Jane Style\\Logs\\2ch\\saimoe\\2014\\"
saimoe = "D:\\app\\V2C\\log\\saimoe.thread.jp\\ast\\"
jbbs = "D:\\app\\V2C\\log\\jbbs_\\anime\\10101\\"
pids = ARGV[1] ? ARGV[1].split(',') : [1405689525]
t = 0
pids.each do |pid|
  filename, filecode = "#{saimoe}#{pid}.dat", "Shift_JIS"
  filename, filecode = "#{jbbs}#{pid}.cgi", "EUC-JP" unless File.exist?(filename)
  page = File.open(filename) { |f| f.read }
  page.encode!("UTF-8", filecode, :invalid => :replace, :undef => :replace)
  # print page
  page.split(/[\r\n]/).each do |line|
    next if line.match(id_name) == nil
    next if line.match(code_name) == nil
    code = line.match(code_name)[1].to_s
    id = line.match(id_name)[1].to_s
    ids[id] = [code, []] unless ids.key?(id)
    # printf "%s %s %d\n", id, ids[id][0], ids[id][1].length
    # print line, "\n"
    next unless ids[id][1].length == 0 or (ids[id][1].length > ngidnum and code == ids[id][1])
    ids[id][1] = []
    line.scan(item_name) do |chas|
      cha = "<<#{chas[0]}>>"
      # print cha
      next if deban.index(cha).nil?
      ids[id][1] << cha
    end
    ids[id][1].uniq!
  end
end

tot = 0
ids.each do |id, pou|
  tot += 1 if pou[1].length > 0
  next if pou[1].length > 10
  # print pou[0], "\n"
  pou[1].each do |cha|
    list[cha] = 0 unless list.key?(cha)
    list[cha] += 1
  end
end

list = list.sort_by {|k, x| -x }
i, j = 0, 0
m = ARGV[2] ? ARGV[2].to_i : list.length
list.each do |k, e|
  i += 1
  j = i if i <= 1 or e != list[i - 2][1]
  printf "%2d位 %3d票 %s\n", j, e, k[0..34]
  break if i > m
end

printf "投票：%d\n", tot

if ARGV[3]
  a = ARGV[3].encode("UTF-8", "GBK")
  printf "%s\n", a
  i, j = 0
  list.each do |k, e|
    i += 1
    j = i if i <= 1 or e != list[i - 2][1]
    if k.include?(a)
      printf "%2d位 %3d票 %s\n", j, e, k
    end
  end
end
