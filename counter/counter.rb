require 'rubygems'
require 'nokogiri'
require "open-uri"
require "json"
require "scanf"

TIMEFMT = "%Y年%m月%d日 %H:%M:%S"
MUKOU = "无效票"
NISE = "非法票" # 伪票

ESCAPECHARS = /[<>【】]/
CONTFORM = {
  "联赛" => {
    :begin_time => /开始时间：(?<month>[0-9]+)月(?<day>[0-9]+)日(?<hour>[0-9]+):(?<min>[0-9]{,2})/,
    :end_time => /结束时间：(?<month>[0-9]+)月(?<day>[0-9]+)日(?<hour>[0-9]+):(?<min>[0-9]{,2})/,
    :time_inc => '开始时间',
    :deban_inc => '出场阵容',
    :item_name => /(<<.+?>>|[^><]+?（.+?场）)/,
    :group_inc => '场）',
    :chara_inc => '<<',
    :ticket_name => /<+.+?>+/,
    :need_pre => '',
    :need_suf => '',
    :banmul => false,
    :vote_limit => 1,
    :level_limit => 8,
    :blacklist => 'blacklist.txt',
    :whitelist => 'whitelist.txt'
  },
  "歌赏" => {
    :begin_time => /有效时间：(?<year>[0-9]+)\.(?<month>[0-9]+)\.(?<day>[0-9]+)\s+(?<hour>[0-9]+)：(?<min>[0-9]{,2})\s*[-~]/,
    :end_time => /有效时间：.*[-~]\s*(?<year>[0-9]+)[\.\s]*(?<month>[0-9]+)[\.\s]*(?<day>[0-9]+)\s+(?<hour>[0-9]+)：(?<min>[0-9]{,2})/,
    :time_inc => '有效时间',
    :deban_inc => '比赛曲目',
    :item_name => /(【[0-9]+\..+?】|[^【】]+?曲目：)/,
    :group_inc => '曲目',
    :chara_inc => '【',
    :ticket_name => /【[0-9]+\..+?】/,
    :need_pre => '【投票】',
    :need_suf => '',
    :banmul => false,
    :vote_limit => 1,
    :level_limit => 8,
    :blacklist => '',
    :whitelist => ''
  }
}

class Array
  def find_lgn(ele)
    ele == bsearch {|x| x >= ele }
  end
end

def match_to_time(t, sc = 00)
  year = t.names.include?("year") ? t[:year].to_i : Time.now.year
  return Time.new(year, t[:month].to_i, t[:day].to_i, t[:hour].to_i, t[:min].to_i, sc)
end

class Contest
  attr_accessor :title, :time_b, :time_e, :lz, :groups, :nums, :ima, :blacklist, :whitelist

  def initialize(blackfile = "", whitefile = "")
    @title = ""
    @lz = ""
    @time_b = nil
    @time_e = nil
    @nums = 0
    @people = 0
    @groups = Array.new
    @ima = Time.now
    @ghash = Hash.new
    @blacklist = Array.new
    @whitelist = Array.new
  end

  def clear
    @title = ""
    @lz = ""
    @time_b = nil
    @time_e = nil
    @nums = 0
    @people = 0
    @groups.clear
    @ima = Time.now
    @blacklist.clear
    @whitelist.clear
  end

  def addpeople
    @people += 1
  end

  def buildblacklist(file)
    return if !File.exist?(file)
    $stderr.printf "加载%s ", file
    f = open(file)
    while u = f.gets and !u.nil?
      @blacklist.push(u.force_encoding(Encoding::UTF_8).strip)
    end
    f.close
    @blacklist.sort!
  end

  def buildwhitelist(file)
    return if !File.exist?(file)
    $stderr.printf "加载%s ", file
    f = open(file)
    while u = f.gets and !u.nil?
      @whitelist.push(u.force_encoding(Encoding::UTF_8).strip)
    end
    f.close
    @whitelist.sort!
  end

  def inblack?(user)
    return false if @blacklist.empty?
    return @blacklist.find_lgn(user)
  end

  def inwhite?(user)
    return true if @whitelist.empty?
    return @whitelist.find_lgn(user)
  end

  def settime!(s, t)
    @time_b = match_to_time(s)
    @time_e = match_to_time(t)
  end

  def output(file = STDOUT)
    sort!
    file.printf "%s\n", @title
    # file.puts "运营：#{@lz}"
    file.printf "开始时间：%s  结束时间：%s\n", @time_b.strftime(TIMEFMT), @time_e.strftime(TIMEFMT)
    file.printf "当前时间：%s  有效投票：%d票 / %d人\n", @ima.strftime(TIMEFMT), @nums, @people
    tmp = Hash.new
    @groups.each do |e|
      tmp[e.name] = ""
      tmp[e.name] += sprintf("\n%s 共%d票\n", e.name, e.nums)
      i, j = 0, 0
      e.charas.each do |f|
        i += 1
        j = i if i < 2 or e.charas[i - 2].nums != f.nums
        tmp[e.name] += sprintf("%2d位 %3d票 %s\n", j, f.nums, f.name.gsub(ESCAPECHARS, "")) # 输出时也不需要特殊标记，去掉
      end
      file.write(tmp[e.name]) unless e.name == NISE or e.name == MUKOU
    end
    file.write(tmp[MUKOU])
  end

  def sort!
    @nums = 0
    @groups.each do |e|
      e.sort!
      e.count!
      @nums += e.nums if e.name != MUKOU and e.name != NISE
    end
    # @groups.sort_by! {|x| -x.nums }
  end

  def addgroup(group)
    @groups.push(group)
  end

  def find(name, grp = "")
    # 不指定无效票认为是有效票，查找之
    # TODO: 顺序查找效率较低，需要改善算法
    if grp == MUKOU or grp == NISE # 无效票、伪票
      name.gsub!(ESCAPECHARS, "") # 这些票不需要特殊标记来验证，可以去掉
      i = @groups.find_index {|x| x.name == grp }
      j = @groups[i].charas.find_index {|f| f.name == name }
      return i, j if j # 是已经存在的无效票则返回
      n = Character.new(name) # 添加一个无效票
      @groups[i].charas.push(n)
      return i, @groups[i].charas.length - 1
    else # 真票、有效票
      @groups.each_index do |i|
        next if @groups[i].name == MUKOU
        j = @groups[i].charas.find_index {|f| f.name == name }
        return i, j if j # 是有效票则返回
      end
    end
    return nil, nil
  end

  def add_index(a, b)
    return false if a.nil? or b.nil?
    @groups[a].charas[b].add # 计数
    return true
  end
end

class Group
  attr_accessor :name, :charas, :nums

  def initialize(name, charas = [])
    @name = name
    @charas = charas
    @nums = 0
  end

  def addcharas(chara)
    @charas.push(chara)
  end

  def sort!
    @charas.sort_by! {|x| [-x.nums, x.name] }
  end

  def count!
    @nums = 0
    @charas.each {|e| @nums += e.nums }
    @nums
  end
end

class Character
  attr_accessor :name, :nums, :group

  def initialize(name, group = "", nums = 0)
    @name = name
    @nums = nums
    @group = group
  end

  def add
    @nums += 1
  end
end

class Post
  attr_accessor :aid, :author, :floor, :date, :text, :votes, :real, :level

  def initialize(aid, author, floor, t, text, level)
    @aid = aid.to_i
    @author = author.to_s.force_encoding(Encoding::UTF_8).strip
    @floor = floor.to_i
    @date = match_to_time(t.match(/(?<year>[0-9]+)-(?<month>[0-9]+)-(?<day>[0-9]+)\s+(?<hour>[0-9]+):(?<min>[0-9]{,2})/))
    @text = text
    @votes = Array.new
    @real = 0
    @level = level.to_i
  end

  def output(file = STDOUT)
    file.puts "#{@author}(#{@aid}) #{@floor}楼 #{@date}"
    file.puts "#{@text}"
  end
end

class Posts
  attr_accessor :posts, :comp

  def initialize(limit = 0)
    @posts = Array.new
    @limit = limit.to_i
    @rules = Hash.new
    @comp = Contest.new
    @did = Hash.new
  end

  def clear
    @posts.clear
    @comp.clear
    @did.clear
  end

  def output(file = STDOUT)
    @posts.each {|t| t.output(file) }
  end

  def did?(name)
    if @did.key?(name)
      return @did[name] == 0 ? false : true
    end
    return false
  end

  def kiroku(name, v = 0)
    if @did.key?(name)
      @did[name] += v
    else
      @did[name] = v
    end
  end

  def mukou(t) # 返回真记为无效票
    # return false
    return "时" if t.date > @comp.time_e or t.date < @comp.time_b
    # 超过比赛时间
    return "黑" if @comp.inblack?(t.author)
    return "白" unless @comp.inwhite?(t.author)
    # 白名单和黑名单过滤
    return "级" if t.level < @rules[:level_limit]
    # 等级不够
    return "前" unless t.text.include?(@rules[:need_pre])
    return "后" unless t.text.include?(@rules[:need_suf])
    # 需要投票格式
    return "重" if did?(t.author)
    # 已经投过票，多重
    return false
  end

  def ticket(t)
    l = Array.new
    k = 0
    loop do
      return l if l.length >= t.votes.length or k >= t.votes.length
      i, j = @comp.find(t.votes[k])
      l.push([k, i, j]) if !i.nil? and !j.nil?
      k += 1
    end
  end

  def count(t, tpl)
    flag = 0
    t.text.scan(tpl) {|m| t.votes.push(m.to_s) } # 把票放进数组里
    l = Array.new
    if x = mukou(t) # 如果投票者非法，即投的是伪票
      $stderr.printf("{%s[%s]} ", t.author, x) if !t.votes.empty?
      t.votes.each do |v|
        i, j = @comp.find(v, NISE)
        @comp.add_index(i, j)
      end
      t.votes.each {|v| $stderr.printf "%s ", v }
    else # 投票者合法
      l = ticket(t) 
      if @rules[:banmul] and l.length > @rules[:vote_limit] # 多投Ban掉
        t.votes.each do |v|
          i, j = @comp.find(v, MUKOU)
          @comp.add_index(i, j) # 记录无效票
        end
      else # 未限制多投或投票数符合要求
        l.each do |v|
          break if flag >= @rules[:vote_limit]
          @comp.add_index(v[1], v[2])
          t.votes[v[0]] = nil
          flag += 1
        end
        t.votes.delete(nil)
        t.votes.each do |v|
          i, j = @comp.find(v, MUKOU)
          @comp.add_index(i, j) # 记录无效票
        end
      end
      if !t.votes.empty?
        $stderr.printf "(%s)", t.author
        t.votes.each {|v| $stderr.printf "%s ", v }
      end
    end
    @comp.addpeople if flag != 0
    return flag
  end

  def writecache(pid, pn, op, ed)
    a = Array.new
    for i in op..ed do
      t = @posts[i]
      c = {
        :aid => t.aid,
        :author => t.author,
        :floor => t.floor,
        :date => t.date,
        :text => t.text,
        :level => t.level
      }
      a.push(c)
    end
    of = cachefile(pid, pn)
    File.delete(of) if File.exist?(of)
    open(of, "w") {|f| f.write(JSON.generate(a).force_encoding("utf-8")) }
  end

  def readcache(pid, pn)
    return false unless File.exist?(cachefile(pid, pn))
    a = open(cachefile(pid, pn)) {|f| f.read }
    a = JSON.parse(a.force_encoding("utf-8"))
    a.each do |post|
      t = Post.new(
        post["aid"],
        post["author"],
        post["floor"],
        post["date"],
        post["text"],
        post["level"]
      )
      record!(t)
    end
    return true
  end

  def parse(page)
    op = @posts.length
    page.css('.l_post').each do |post|
      info = JSON.parse(post.attr('data-field'))
      t = Post.new(
        info['author']['id'],
        info['author']['name'],
        info['content']['floor'],
        info['content']['date'],
        post.css('.d_post_content')[0].content,
        info['author']['grade_level']
      )
      record!(t)
    end
    ed = @posts.length - 1
    return op, ed
  end

  def record!(t)
    @comp.lz = t.author if t.floor == 1 # 运营
    if t.author == @comp.lz # 运营发布的消息
      if @comp.time_b == @comp.time_e and t.text.include?(@rules[:time_inc])
        @comp.settime!(t.text.match(@rules[:begin_time]),
          t.text.match(@rules[:end_time]))
      end
      if t.text.include?(@rules[:deban_inc])
        g = Group.new(MUKOU) # 无效票组
        t.text.scan(@rules[:item_name]) do |uu|
          u = uu[0].to_s
          if u.include?(@rules[:chara_inc])
            k = Character.new(u)
            g.addcharas(k) # 规定的有效票
          elsif u.include?(@rules[:group_inc])
            @comp.addgroup(g)
            g = Group.new(u)
          end
        end
        @comp.addgroup(g)
        @comp.addgroup(Group.new(NISE)) # 伪票组
      end
    else
      kiroku(t.author, count(t, @rules[:ticket_name]))
    end
    @posts.push(t)
  end

  def parseit(url, pid, pn, readnew = false)
    if readnew or !readcache(pid, pn)
      $stderr.print "[新]"
      page = Nokogiri::HTML(open(pageurl(url, pid, pn)))
      bg, ed = parse(page)
      writecache(pid, pn, bg, ed)
    end
  end

  def pageurl(url, pid, pn)
    return "#{url}#{pid}?pn=#{pn}"
  end

  def cachefile(pid, pn)
    Dir.mkdir("tmp") unless Dir.exist?("tmp")
    return "tmp/#{pid}_#{pn}"
  end

  def fetch(pid = 0, url = "http://tieba.baidu.com/p/")
    return if pid == 0
    puts ""
    page = Nokogiri::HTML(open("#{url}#{pid}"))

    @comp.title = page.css('.core_title_txt')[0].attr('title')
    CONTFORM.each do |k, v|
      if @comp.title.include?(k)
        @comp.buildblacklist(v[:blacklist])
        @comp.buildwhitelist(v[:whitelist])
        @rules = v
        break
      end
    end
    $stderr.printf "%s ", @comp.title

    pager = page.css('.l_posts_num').css('.l_reply_num')[0].text
    lastpn = pager.match(/共\s*([0-9]+?)\s*页/)[1].to_i
    $stderr.print "共#{lastpn}页 "

    lastpn = @limit if @limit > 0
    for pn in 1..lastpn
      $stderr.print "[#{pn}]"
      parseit(url, pid, pn, pn == lastpn)
    end

    $stderr.puts " 完成"
  end
end

if $0 == __FILE__
  max = ARGV[0].to_i
  ls = Posts.new(max)
  pid = Array.new
  while line = $stdin.gets and line[0] != '0'
    pid.push(line.to_i)
  end
  pid.each do |pt|
    ls.clear
    ls.fetch(pt)
    ls.comp.output
  end
end

# 应该实现一个高并发的Go的服务器，然后在网页用Ajax查询Go服务器
# Ruby解释器定时翻新最后一页，并把数据返回给Go服务器，减轻负荷
#       HTML/Ajax Page <=> Go Server <=> Ruby Parser
#
# 功能自定义性强，去伪/无效/萌文/曲线图/...功能给出接口可选
# 将来要适应联赛/歌赏/新星萌/日萌……
#
# 也应该实现一个图形界面，用C/C++/Qt重写，也考虑C#/.Net方案
# 必须发布成可执行文件
