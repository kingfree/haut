require 'rubygems'
require 'nokogiri'
require "open-uri"
require "json"

class Contest
  attr_accessor :title, :time_b, :time_e, :vote_limit, :lz, :groups, :nums, :ima

  def initialize()
    @title = ""
    @time_b = ""
    @time_e = ""
    @vote_limit = 1
    @lz = ""
    @nums = 0
    @groups = Array.new
    @ima = Time.now
  end

  def output
    sort!
    puts "#{@title}"
    # puts "运营：#{@lz}"
    # puts "开始时间：#{@time_b}  结束时间：#{@time_e}"
    # puts "单人投票限制：#{@vote_limit}票"
    puts "当前时间：#{@ima} 投票总数：#{@nums}票"
    @groups.each do |e|
      puts ""
      puts "#{e.name} 共#{e.nums}票"
      i = 0
      e.charas.each do |f|
        i += 1
        puts "#{i}位 #{f.name} #{f.nums}票"
      end
    end
  end

  def sort!
    @nums = 0
    @groups.each do |e|
      e.sort!
      @nums += e.count!
    end
    @groups.sort_by! { |x| - x.nums }
  end

  def addgroup(group)
    @groups.push(group)
  end

  def find(name)
    @groups.each_index do |i|
      res = @groups[i].charas.find_index { |f| f.name == name }
      return i, res if res
    end
    i = @groups.find_index { |f| f.name == "无效" }
    n = Character.new(name)
    @groups[i].charas.push(n)
    return i, @groups[i].charas.length - 1
  end

  def add(name)
    a, b = find(name)
    @groups[a].charas[b].add
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

class Counter
  def initialize()
    @counter = Hash.new
    @black_name = Array.new
  end

  def addblack(name)
    return if @black_name.include?(t.author)
    name = name.to_s
    @black_name.push(name) if not @black_name.include?(name)
  end
end

class Post
  attr_accessor :aid, :author, :floor, :date, :text

  def initialize(aid, author, floor, date, text)
    @aid = aid.to_i
    @author = author
    @floor = floor.to_i
    @date = date
    @text = text
  end

  def output
    puts "#{@author}(#{@aid}) #{@floor}楼 #{@date}"
    puts "  #{@text}"
  end
end

class Posts
  attr_accessor :posts, :cnt, :comp

  def initialize(limit = 0)
    @posts = Array.new
    @cnt = Counter.new
    @limit = limit.to_i
    @comp = Contest.new
  end

  def output()
    puts @blacklist
    @posts.each { |t| t.output }
  end

  def count(t, tpl)
    t.text.scan(tpl) do |m|
      name = m.to_s
      @comp.add(name)
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
      @posts.push(t)
      if t.floor == 1
        comp.lz = t.author
      end
      if t.author == comp.lz
        if comp.time_b == "" and comp.time_e == ""
          comp.time_b = t.text.match(/开始时间：([0-9]+月[0-9]+日[0-9]+:[0-9]{,2})/)[1]
          comp.time_e = t.text.match(/结束时间：([0-9]+月[0-9]+日[0-9]+:[0-9]{,2})/)[1]
        end
        if t.text.match('出场阵容')
          g = Group.new("无效")
          t.text.scan(/(<<.+?>>|[^><]+?（.+?场）)/) do |uu|
            u = uu[0].to_s
            if u.match('场')
              comp.addgroup(g)
              g = Group.new(u)
            elsif u.match('<<')
              k = Character.new(u)
              g.addcharas(k)
            end
          end
          comp.addgroup(g)
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

    comp.title = page.css('.core_title_txt')[0].text

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
  # puts "输入帖子编号："
  pid = [2826568816, 2826568427, 2826569611]
  pid.each do |p|
    tie = Posts.new(max)
    tie.fetch(p)
    tie.comp.output
  end
end
