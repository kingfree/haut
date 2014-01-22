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
    # printf "开始时间：%s  结束时间：%s\n", @time_b.strftime(TIMEFMT), @time_e.strftime(TIMEFMT)
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
    @groups.each_index do |i|
      next if @groups[i].name == MUKOU and grp == ""
      res = @groups[i].charas.find_index { |f| f.name == name }
      return i, res if res
    end
    return add_mukou(name) if grp == MUKOU
    return nil, nil
  end

  def add_mukou(name)
    i = @groups.find_index { |x| x.name == MUKOU }
    n = Character.new(name)
    @groups[i].charas.push(n)
    return i, @groups[i].charas.length - 1
  end

  def add(name, grp = "")
    a, b = find(name, grp)
    return false if a == nil and b == nil
    @groups[a].charas[b].add
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
    @date = date
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
    return false
  end

  def tryadd(votes)
    i = 0
    votes.each do |m|
      return i if comp.add(m)
      i += 1
    end
    return nil
  end

  def count(t, tpl)
    t.text.scan(tpl) { |m| t.votes.push(m.to_s) }
    unless mukou(t)
      i = tryadd(t.votes)
      t.votes.delete_at(i) if i != nil
    end
    t.votes.each { |m| @comp.add(m, MUKOU) }
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
      @posts.push(t)
      if t.floor == 1
        @comp.lz = t.author
      end
      if t.author == @comp.lz
        if @comp.time_b == nil
          @comp.settime!(
            t.text.match(/开始时间：([0-9]+月[0-9]+日[0-9]+:[0-9]{,2})/)[1],
            t.text.match(/结束时间：([0-9]+月[0-9]+日[0-9]+:[0-9]{,2})/)[1]
          )
        end
        if t.text.match('出场阵容')
          g = Group.new(MUKOU)
          t.text.scan(/(<<.+?>>|[^><]+?（.+?场）)/) do |uu|
            u = uu[0].to_s
            if u.match('场')
              @comp.addgroup(g)
              g = Group.new(u)
            elsif u.match('<<')
              k = Character.new(u)
              g.addcharas(k)
            end
          end
          @comp.addgroup(g)
        end
      else
        count(t, /<<.+?>>/)
      end
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
  puts "还没有统计白名单和黑名单，结果仅供参考"
  pid.each do |p|
    tie = Posts.new(max)
    tie.fetch(p)
    tie.comp.output
  end
end
