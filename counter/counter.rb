require 'rubygems'
require 'nokogiri'
require "open-uri"
require "json"
require "scanf"

TIMEFMT = "%Y年%m月%d日 %H:%M:%S"
MUKOU = "无效"

class Contest
  attr_accessor :title, :time_b, :time_e, :vote_limit, :lz, :groups, :nums, :ima

  def initialize()
    @title = ""
    @vote_limit = 1
    @lz = ""
    @time_b = nil
    @time_e = nil
    @nums = 0
    @groups = Array.new
    @ima = Time.now
    @ghash = Hash.new
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
      i = 0
      e.charas.each do |f|
        i += 1
        printf "%2d位 %3d票 %s\n", i, f.nums, f.name
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
      i = @groups.find_index { |x| x.name == MUKOU }
      j = @groups[i].charas.find_index { |f| f.name == name }
      return i, j if j # 是已经存在的无效票则返回
      return add_mukou(name) # 添加一个无效票
    else
      @groups.each_index do |i|
        next if @groups[i].name == MUKOU
        j = @groups[i].charas.find_index { |f| f.name == name }
        return i, j if j # 是有效票则返回
      end
    end
    return nil, nil
  end

  def add_mukou(name)
    # 添加新的无效票
    i = @groups.find_index { |x| x.name == MUKOU }
    n = Character.new(name)
    @groups[i].charas.push(n)
    return i, @groups[i].charas.length - 1
  end

  def add(name, grp = "")
    a, b = find(name, grp) # 尝试查找
    return false if a == nil and b == nil # 找不到返回假
    # $stderr.puts @groups[a].name if grp == MUKOU
    @groups[a].charas[b].add # 找到了，计数
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
    @author = author
    @floor = floor.to_i
    l = date.scanf("%d-%d-%d %d:%d")
    @date = Time.new(l[0], l[1], l[2], l[3], l[4], 00)
    @text = text
    @votes = Array.new
  end

  def output
    puts "#{@author}(#{@aid}) #{@floor}楼 #{@date}"
    puts "  #{@text}"
  end

end

class Posts
  attr_accessor :posts, :comp

  def initialize(limit = 0)
    @posts = Array.new
    @limit = limit.to_i
    @comp = Contest.new
  end

  def output()
    @posts.each { |t| t.output }
  end

  def mukou(t)
    # $stderr.puts t.date, @comp.time_e
    return true if t.date > @comp.time_e or t.date < @comp.time_b
    # 超过比赛时间
    return true if @posts.find_index { |x| x.aid == t.aid }
    # $stderr.printf "[%d/%d]", l, @posts.length
    # 已经投过票
    return false
    # TODO: 需要添加白名单和黑名单过滤
  end

  def tryadd(votes)
    i = 0
    votes.each do |m|
      return i if @comp.add(m) # 返回第一个合法值的下标
      i += 1
    end
    return nil
  end

  def count(t, tpl)
    t.text.scan(tpl) { |m| t.votes.push(m.to_s) } # 把票放进数组里
    # $stderr.printf "%d-", t.votes.length
    unless mukou(t) # 如果投票者合法
      i = tryadd(t.votes) # 尝试把投票记录下来
      # $stderr.printf "(%d)", i.to_i
      t.votes.delete_at(i) if i != nil # 如果投票合法就删掉
    end
    # $stderr.printf "=>%d\n", t.votes.length
    t.votes.each { |m| @comp.add(m, MUKOU) } # 剩下的全是无效票
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
    puts
    url = "#{url}#{pid}"
    $stderr.puts url
    page = Nokogiri::HTML(open(url))

    comp.title = page.css('.core_title_txt')[0].attr('title')

    pager = page.css('.l_pager')[0]
    pages = pager.css('a').select { |link| link.text == "尾页" }
    lastl = pages[0].attr('href')
    lastpn = lastl.match(/\?pn=([0-9]+)/)[1].to_i
    $stderr.puts "共有 #{lastpn} 页"
    $stderr.print "正在处理页面 [1]"
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
  pid.each do |p|
    tie = Posts.new(max)
    tie.fetch(p)
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
