require 'uri'
require 'open-uri'
require 'csv'

def fetch(url)
  open(url) { |r| r.read }
end

def 抽取(网页, 开始, 结束)
  始 = 网页.index(开始)
  终 = 网页.index(结束, 始)
  p 始
  p 终
  网页[始..终]
end

def 抓取标题(网页)
  %r{<span.*?id="btAsinTitle".*?>.*?padding-left.*?(.*?).*?<.*?</span>}m =~ 网页
  清理标题($1)
end

def 抓取作者(网页)
  作者锚 = %r{<a.*?href=".*?field-author.*?".*?>(.+?)</a\s*>}m
  网页.scan(作者锚).flatten.collect { |作者| 清理作者(作者) }
end

def 抓取图书(网页)
  结果 = {}
  网页 = 抽取(网页, /<div.*class\s*=\s*"buying".*?>/, %r{</dev\s*>})
  结果[:标题] = 抓取标题(网页)
  结果[:作者] = 抓取作者(网页)
  结果
end

def 清理标题(标题)
  装 = /\[\s*.+装\s*\]/m
  标题.gsub(装, '').strip.squeeze(' ')
end

def 清理作者(作者)
  作者.squeeze(' ')
end

def 抓取列表(网页)
  结果 = []
  全部 = %r{shoveler.*?</ul\s*>}m
  单个 = %r{<li.*?>.*?</li\s*>}m
  列表 = 网页[全部]
  列表.scan(单个).collect do |项目|
    { :链接 => /href\s*=\s*(.*?)"/.match(项目)[1],
      :标题 => %r{<a.*?>(.*?)</a\s*>}.match(项目)[1] }
  end
end

def 格式化(图书)
  self.send(格式, 图书)
end

def 普通字符串(图书)
  图书[:标题]
end

def 逗号分割串(图书)
  标题 = 图书[:标题]
  作者 = 图书[:作者].join(', ')
  CSV.generate_line([标题, 作者])
end

def 旅程(链接, 深度 = 10)
  目前 = []
  深度.times do
    页面 = fetch(链接)
    图书 = 抓取图书(页面)
    目前 << 图书[:标题]
    # puts 格式化(图书)
    puts 图书
    下一本 = 抓取列表(页面).find { |可能| !目前.include?(可能[:标题]) }
    链接 = 下一本[:链接]
  end
end

if $0 == __FILE__
  if ARGV[0] == '--csv'
    格式 = :逗号分割串
    ARGV.shift
  else
    格式 = :普通字符串
  end
  if ARGV[0]
    起点 = ARGV[0]
  else
    puts "输入链接："
    起点 = gets
  end
  旅程(起点)
end


