require 'rubygems'
require 'nokogiri'
require "open-uri"
require "json"
require "scanf"

TIMEFMT = "%Y年%m月%d日 %H:%M:%S"
MUKOU = "无效"
NISE = "无资格票" # 伪票

class Array
  def find_lgn(ele)
    res = bsearch { |x| x >= ele }
    if res == ele
      return true
    else
      return false
    end
  end
end

class Contest
  attr_accessor :title, :time_b, :time_e, :vote_limit, :banmul, :lz, :groups, :nums, :ima, :blacklist, :whitelist

  def initialize(blackfile, whitefile)
    @title = ""
    @vote_limit = 1
    @banmul = false # 多次投票Ban掉
    @lz = ""
    @time_b = nil
    @time_e = nil
    @nums = 0
    @groups = Array.new
    @ima = Time.now
    @ghash = Hash.new
    @blacklist = Array.new
    buildblacklist(blackfile) if blackfile != ""
    @whitelist = Array.new
    buildwhitelist(whitefile) if whitefile != ""
  end

  def clear
    @title = ""
    @lz = ""
    @time_b = nil
    @time_e = nil
    @nums = 0
    @groups.clear
    @ima = Time.now
  end

  def buildblacklist(file)
    # $stderr.printf "加载%s ", file
    f = open(file)
    while u = f.gets and u != nil
      @blacklist.push(u.force_encoding(Encoding::UTF_8).strip)
    end
    @blacklist.sort!
  end

  def buildwhitelist(file)
    # $stderr.printf "加载%s ", file
    f = open(file)
    while u = f.gets and u != nil
      @whitelist.push(u.force_encoding(Encoding::UTF_8).strip)
    end
    @whitelist.sort!
  end

  def inblack?(user)
    return @blacklist.find_lgn(user)
  end

  def inwhite?(user)
    return @whitelist.find_lgn(user)
  end

  def settime!(s, t)
    year = Time.now.year
    l = s.scanf("%d月%d日%d:%d")
    @time_b = Time.new(year, l[0], l[1], l[2], l[3], 00)
    l = t.scanf("%d月%d日%d:%d")
    @time_e = Time.new(year, l[0], l[1], l[2], l[3], 59)
  end

  def output
    sort!
    printf "%s\n", @title
    # puts "运营：#{@lz}"
    printf "开始时间：%s  结束时间：%s\n", @time_b.strftime(TIMEFMT), @time_e.strftime(TIMEFMT)
    # puts "单人投票限制：#{@vote_limit}票"
    printf "当前时间：%s  有效票数：%d票\n", @ima.strftime(TIMEFMT), @nums
    @groups.each do |e|
      printf "\n%s 共%d票\n", e.name, e.nums
      next if e.name == NISE
      i, j = 0, 0
      e.charas.each do |f|
        i += 1
        j = i if i < 2 or e.charas[i - 2].nums != f.nums
        printf "%2d位 %3d票 %s\n", j, f.nums, f.name
      end
    end
  end

  def sort!
    @nums = 0
    @groups.each do |e|
      e.sort!
      e.count!
      @nums += e.nums if e.name != MUKOU and e.name != NISE
    end
    @groups.sort_by! { |x| -x.nums }
  end

  def addgroup(group)
    @groups.push(group)
  end

  def find(name, grp = "")
    # 不指定无效票认为是有效票，查找之
    # TODO: 顺序查找效率较低，需要改善算法
    if grp == MUKOU # 无效票
      i = @groups.find_index { |x| x.name == MUKOU }
      j = @groups[i].charas.find_index { |f| f.name == name }
      return i, j if j # 是已经存在的无效票则返回
      n = Character.new(name) # 添加一个无效票
      @groups[i].charas.push(n)
      return i, @groups[i].charas.length - 1
    elsif grp == NISE # 伪票
      i = @groups.find_index { |x| x.name == NISE }
      j = @groups[i].charas.find_index { |f| f.name == name }
      return i, j if j
      n = Character.new(name)
      @groups[i].charas.push(n)
      return i, @groups[i].charas.length - 1
    else # 真票、有效票
      @groups.each_index do |i|
        next if @groups[i].name == MUKOU
        j = @groups[i].charas.find_index { |f| f.name == name }
        return i, j if j # 是有效票则返回
      end
    end
    return nil, nil
  end

  def add_index(a, b)
    return false if a == nil or b == nil
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
    @charas.sort_by! { |x| [-x.nums, x.name.gsub(/[<>+-=]/, "")] }
  end

  def count!
    @nums = 0
    @charas.each { |e| @nums += e.nums }
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
  attr_accessor :aid, :author, :floor, :date, :text, :votes, :real

  def initialize(aid, author, floor, date, text)
    @aid = aid.to_i
    @author = author.to_s.force_encoding(Encoding::UTF_8).strip
    @floor = floor.to_i
    l = date.scanf("%d-%d-%d %d:%d")
    @date = Time.new(l[0], l[1], l[2], l[3], l[4], 00)
    @text = text
    @votes = Array.new
    @real = 0
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
    @comp = Contest.new('blacklist.txt', 'whitelist.txt')
  end

  def clear
    @posts.clear
    @comp.clear
  end

  def output
    @posts.each { |t| t.output }
  end

  def mukou(t)
    # return false
    return true if t.date > @comp.time_e or t.date < @comp.time_b
    # 超过比赛时间
    # return true if @posts.find_index { |x| x.aid == t.aid}
    # 已经投过票
    return true if @comp.inblack?(t.author)
    return false if @comp.inwhite?(t.author)
    # 白名单和黑名单过滤
    return true
  end

  def ticket(t)
    l = Array.new
    k = 0
    loop do
      return l if l.length >= t.votes.length or k >= t.votes.length
      i, j = @comp.find(t.votes[k])
      l.push([k, i, j]) if i != nil and j != nil
      k += 1
    end
  end

  def count(t, tpl)
    t.text.scan(tpl) { |m| t.votes.push(m.to_s) } # 把票放进数组里
    l = Array.new
    if mukou(t) # 如果投票者非法，即投的是伪票
      for k in 0..t.votes.length - 1
        i, j = @comp.find(t.votes[k], NISE)
        @comp.add_index(i, j)
        $stderr.printf "{%s %s} ", t.author, t.votes[k]
      end
    else # 投票者合法
      l = ticket(t) 
      if @comp.banmul and l.length > @comp.vote_limit # 多投Ban掉
        for k in 0..t.votes.length - 1
          i, j = @comp.find(t.votes[k], MUKOU)
          @comp.add_index(i, j) # 记录无效票
          $stderr.printf "(%s %s) ", t.author, t.votes[k]
        end
      else # 未限制多投或投票数符合要求
        k = 0
        l.each do |v|
          break if k >= @comp.vote_limit
          @comp.add_index(v[1], v[2])
          t.votes[v[0]] = nil
          k += 1
        end
        t.votes.each do |v|
          next if v == nil
          i, j = @comp.find(v, MUKOU)
          @comp.add_index(i, j) # 记录无效票
          $stderr.printf "(%s %s) ", t.author, v 
        end
      end
    end
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
      }
      a.push(c)
    end
    open(cachefile(pid, pn), "w") { |f| f.write(JSON.generate(a).force_encoding("utf-8")) }
  end

  def readcache(pid, pn)
    return false unless File.exist?(cachefile(pid, pn))
    a = open(cachefile(pid, pn)) { |f| f.read }
    a = JSON.parse(a.force_encoding("utf-8"))
    a.each do |post|
      t = Post.new(
        post["aid"],
        post["author"],
        post["floor"],
        post["date"],
        post["text"]
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
        post.css('.d_post_content')[0].content
      )
      record!(t)
    end
    ed = @posts.length - 1
    return op, ed
  end

  def record!(t)
    @comp.lz = t.author if t.floor == 1 # 运营
    if t.author == @comp.lz # 运营发布的消息
      if @comp.time_b == nil
        @comp.settime!(
          t.text.match(/开始时间：([0-9]+月[0-9]+日[0-9]+:[0-9]{,2})/)[1],
          t.text.match(/结束时间：([0-9]+月[0-9]+日[0-9]+:[0-9]{,2})/)[1]
        )
      end
      if t.text.include?('出场阵容')
        g = Group.new(MUKOU) # 无效票组
        t.text.scan(/(<<.+?>>|[^><]+?（.+?场）)/) do |uu|
          u = uu[0].to_s
          if u.include?('<<')
            k = Character.new(u)
            g.addcharas(k) # 规定的有效票
          elsif u.include?('（')
            # 如果是“场”单字会匹配“战场原黑仪”，稳妥起见放在第二分支
            @comp.addgroup(g)
            g = Group.new(u)
          end
        end
        @comp.addgroup(g)
        @comp.addgroup(Group.new(NISE)) # 伪票组
      end
    else
      count(t, /<+.+?>+/)
    end
    @posts.push(t)
  end

  def parseit(url, pid, pn, readnew = false)
    unless readnew or readcache(pid, pn)
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
  $stderr.puts "输入帖子编号(负数或0结束)："
  pid = Array.new
  while line = $stdin.gets.to_i and line > 0
    pid.push(line)
  end
  tie = Posts.new(max)
  pid.each do |pt|
    tie.clear
    tie.fetch(pt)
    tie.comp.output
    #tie.output
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
