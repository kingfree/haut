require 'rubygems'
require 'nokogiri'
require "open-uri"
require "json"
require "scanf"

TIMEFMT = "%Y年%m月%d日 %H:%M:%S"
MUKOU = "无效"

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

  def buildblacklist(file)
    $stderr.printf "加载黑名单%s...", file
    f = open(file)
    while u = f.gets and u != nil
      @blacklist.push(u.force_encoding(Encoding::UTF_8).strip)
    end
    @blacklist.sort!
  end

  def buildwhitelist(file)
    $stderr.printf "加载白名单%s...", file
    f = open(file)
    while u = f.gets and u != nil
      @whitelist.push(u.force_encoding(Encoding::UTF_8).strip)
    end
    @whitelist.sort!
  end

  def inblack?(user)
    # return @blacklist.bsearch { |x| x == user }
    return @blacklist.find_index { |x| x == user }
  end

  def inwhite?(user)
    # return @whitelist.bsearch { |x| x == user }
    return @whitelist.find_index { |x| x == user }
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
      @nums += e.nums if e.name != MUKOU
    end
    @groups.sort_by! { |x| - x.nums }
  end

  def addgroup(group)
    @groups.push(group)
  end

  def find(name, grp = "")
    # 不指定无效票认为是有效票，查找之
    # TODO: 顺序查找效率较低，需要改善算法
    if grp == MUKOU
          # $stderr.printf "(%s)Y ", name
      i = @groups.find_index { |x| x.name == MUKOU }
      j = @groups[i].charas.find_index { |f| f.name == name }
      return i, j if j # 是已经存在的无效票则返回
      n = Character.new(name) # 添加一个无效票
      @groups[i].charas.push(n)
      return i, @groups[i].charas.length - 1
    else
          # $stderr.printf "(%s)N ", name
      @groups.each_index do |i|
        next if @groups[i].name == MUKOU
        j = @groups[i].charas.find_index { |f| f.name == name }
        return i, j if j # 是有效票则返回
      end
    end
          # $stderr.printf "(%s)O ", name
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
    @charas.sort_by! { |x| - x.nums }
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
  attr_accessor :aid, :author, :floor, :date, :text, :votes

  def initialize(aid, author, floor, date, text)
    @aid = aid.to_i
    @author = author.to_s.force_encoding(Encoding::UTF_8).strip
    @floor = floor.to_i
    l = date.scanf("%d-%d-%d %d:%d")
    @date = Time.new(l[0], l[1], l[2], l[3], l[4], 00)
    @text = text
    @votes = Array.new
  end

  def output(file = STDOUT)
    file.puts "#{@author}(#{@aid}) #{@floor}楼 #{@date}"
    # @votes.each { |t| printf "[%s]", t }
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

  def output
    @posts.each { |t| t.output }
  end

  def mukou(t)
    return true if t.date > @comp.time_e or t.date < @comp.time_b
    # 超过比赛时间
    return true if @posts.find_index { |x| x.aid == t.aid }
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
    l = ticket(t) if not mukou(t) # 如果投票者合法
    if @comp.banmul and l.length > @comp.vote_limit
      for k in 0..t.votes.length - 1 # 剩下的无效票
        i, j = @comp.find(t.votes[k], MUKOU)
        @comp.add_index(i, j) # 记录无效票
      end
      return true # 已投票
    else
      flag = false
      k = 0
      l.each do |v|
        break if k >= @comp.vote_limit
        @comp.add_index(v[1], v[2])
        t.votes[k] = nil
        k += 1
      end
      flag = true if k == @comp.vote_limit # 已投有效票
      t.votes.each do |v|
        next if v == nil
        i, j = @comp.find(v, MUKOU)
        @comp.add_index(i, j) # 记录无效票
      end
      return flag
    end
  end

  def parse(page)
    page.css('.l_post').each do |post|
      info = JSON.parse(post.attr('data-field'))
      t = Post.new(
        info['author']['id'],
        info['author']['name'],
        info['content']['floor'],
        info['content']['date'],
        post.css('.d_post_content')[0].content
      )
      if t.floor == 1
        @comp.lz = t.author # 运营
      end
      if t.author == @comp.lz # 运营发布的消息
        if @comp.time_b == nil
          @comp.settime!(
            t.text.match(/开始时间：([0-9]+月[0-9]+日[0-9]+:[0-9]{,2})/)[1],
            t.text.match(/结束时间：([0-9]+月[0-9]+日[0-9]+:[0-9]{,2})/)[1]
          )
        end
        if t.text.match('出场阵容')
          g = Group.new(MUKOU) # 无效票组
          t.text.scan(/(<<.+?>>|[^><]+?（.+?场）)/) do |uu|
            u = uu[0].to_s
            if u.match('<<')
              k = Character.new(u)
              g.addcharas(k) # 规定的有效票
            elsif u.match('场）')
              # 如果是“场”单字会匹配“战场原黑仪”，稳妥起见放在第二分支
              @comp.addgroup(g)
              g = Group.new(u)
            end
          end
          @comp.addgroup(g)
        end
      else
        count(t, /<<.+?>>/)
      end
      @posts.push(t)
    end
  end

  def fetch(pid = 0, url = "http://tieba.baidu.com/p/")
    return if pid == 0
    puts ""
    url = "#{url}#{pid}"
    $stderr.puts url
    page = Nokogiri::HTML(open(url))

    @comp.title = page.css('.core_title_txt')[0].attr('title')

    pager = page.css('.l_posts_num').css('.l_reply_num')[0].text
    lastpn = pager.match(/共\s*([0-9]+?)\s*页/)[1].to_i
    $stderr.print "共#{lastpn}页 "
    $stderr.print "正在处理 [1]"
    parse(page)
    lastpn = @limit if @limit > 0
    for pn in 2..lastpn
      $stderr.print "[#{pn}]"
      page = Nokogiri::HTML(open("#{url}?pn=#{pn}"))
      parse(page)
      # TODO: 有必要缓存页面或解析结果的前 lastpn - 1 页内容
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
  pid.each do |pt|
    tie = Posts.new(max)
    tie.fetch(pt)
    tie.comp.output
    # tie.output
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
