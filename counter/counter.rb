require 'nokogiri'
require "open-uri"
require "json"
require "scanf"

TIMEFMT = "%m月%d日 %H:%M"
MUKOU = "无效票"
NISE = "伪票"

ALIAS = {
  "<<Saber>>" => "<<阿尔托莉亚>>"
}

ESCAPECHARS = /[<>【】]/
ESCAPEGRPS = /(出场阵容|[<>【】：:]|（(主|客)场）)/
CONTFORM = {
  "联赛" => {
    :begin_time => /开始时间：(?<month>[0-9]+)月(?<day>[0-9]+)日(?<hour>[0-9]+):(?<min>[0-9]{,2})/,
    :end_time => /结束时间：(?<month>[0-9]+)月(?<day>[0-9]+)日(?<hour>[0-9]+):(?<min>[0-9]{,2})/,
    :time_inc => '开始时间',
    :deban_inc => '出场阵容',
    :item_name => /(<<.+?>>|[^><]*)/,
    :group_inc => '',
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
    :deban_inc => '组】',
    :item_name => /(【[0-9]+\..+?】|【[0-9]+组】)/,
    :group_inc => '组】',
    :chara_inc => 'by',
    :ticket_name => /【[0-9]+\..+?】/,
    :need_pre => '【投票】',
    :need_suf => '',
    :banmul => false,
    :vote_limit => 12,
    :level_limit => 8,
    :blacklist => '',
    :whitelist => ''
  }
}

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
    @blacklist = Hash.new
    @whitelist = Hash.new
  end

  def clear
    @title = ""
    @lz = ""
    @time_b = nil
    @time_e = nil
    @nums = 0
    @people = 0
    @groups.clear
    @ghash.clear
    @ima = Time.now
  end

  def addpeople
    @people += 1
  end

  def buildblacklist(file)
    @blacklist.clear
    return if !File.exist?(file)
    f = open(file)
    while u = f.gets and !u.nil?
      @blacklist[u.force_encoding(Encoding::UTF_8).strip] = true
    end
    f.close
  end

  def buildwhitelist(file)
    @whitelist.clear
    return if !File.exist?(file)
    f = open(file)
    while u = f.gets and !u.nil?
      @whitelist[u.force_encoding(Encoding::UTF_8).strip] = true
    end
    f.close
  end

  def inblack?(user)
    return false if @blacklist.empty?
    return @blacklist.key?(user)
  end

  def inwhite?(user)
    return true if @whitelist.empty?
    return @whitelist.key?(user)
  end

  def settime!(s, t)
    @time_b = match_to_time(s)
    @time_e = match_to_time(t)
  end

  def output(file = STDOUT)
    count
    file.printf "\n%s\n", @title
    # file.puts "运营：#{@lz}"
    file.printf "开始时间：%s  结束时间：%s\n", @time_b.strftime(TIMEFMT), @time_e.strftime(TIMEFMT)
    file.printf "当前时间：%s  有效投票：%d票 / %d人\n", @ima.strftime(TIMEFMT), @nums, @people
    tmp = Hash.new
    @ghash.each do |k, e|
      next if e.nums == 0
      tmp[k] = sprintf("\n%s 共%d票\n", e.name, e.nums)
      i, j = 0, 0
      a = e.sort
      a.each do |kk, f|
        f.count
        i += 1
        j = i if i <= 1 or f.nums != a[i - 2][1].nums
        tmp[k] += sprintf("%2d位 %3d票 %s%s\n", j, f.nums, f.name.gsub(ESCAPECHARS, ""),
          ALIAS.key?(f.name) ? " (" + ALIAS[f.name].gsub(ESCAPECHARS, "") + ")" : "") # 输出时也不需要特殊标记，去掉
      end
      file.write(tmp[k]) unless k == NISE or k == MUKOU
    end
    file.write(tmp[MUKOU])
    # file.write(tmp[NISE])
  end

  def info(chara, file = STDOUT)
    @ghash[haschara?(chara)].charas[chara].output(file) if haschara?(chara)
  end

  def count
    @nums = 0
    @ghash.each do |k, e|
      @nums += e.nums if e.name != MUKOU and e.name != NISE
    end
    @nums
  end

  def addgroup(group)
    @ghash[group.name] = group
  end

  def rname(name) # 别名
    if ALIAS.value?(name)
      return ALIAS.rassoc(name)[0]
    end
    name
  end

  def haschara?(name)
    name = rname(name)
    @ghash.each do |k, v|
      next if k == NISE or k == MUKOU
      return k if v.charas.key?(name)
    end
    false
  end

  def addt(user, name, grp = "")
    if grp == MUKOU or grp == NISE
      name.gsub!(ESCAPECHARS, "") # 这些票不需要特殊标记来验证，可以去掉
      if @ghash[grp].charas.key?(name)
        @ghash[grp].charas[name].add(user)
      else
        @ghash[grp].add(name)
        @ghash[grp].charas[name].add(user)
      end
    else
      @ghash[grp].charas[rname(name)].add(user)
    end
  end

end

class Group
  attr_accessor :name, :charas

  def initialize(name)
    @name = name
    @charas = Hash.new
    @nums = 0
  end

  def add(name)
    @charas[name] = Character.new(name)
  end

  def sort
    @charas.sort_by {|k, x| [-x.nums, x.name] }
  end

  def nums
    @nums = 0
    @charas.each {|k, x| @nums += x.nums }
    @nums
  end
end

class Character
  attr_accessor :name, :users, :nums

  def initialize(name, num = 0)
    @name = name
    @nums = num
    @users = Array.new
  end

  def add(user)
    @users.push(user)
    @nums += 1
  end

  def count
    @nums = @users.length
  end

  def output(file = STDOUT)
    file.printf("%s(%d票)： ", @name, count)
    @users.each {|v| file.printf("@%s ", v)}
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
  attr_accessor :posts, :comp, :logfile

  def initialize(logf = STDERR)
    @posts = Array.new
    @rules = Hash.new
    @comp = Contest.new
    @did = Hash.new
    @logfile = logf
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

  def count(t, tpl)
    flag = 0
    t.text.scan(tpl) {|m| t.votes.push(m.to_s) } # 把票放进数组里
    l = Array.new
    if x = mukou(t) # 如果投票者非法，即投的是伪票
      t.votes.each do |v|
        @comp.addt(t.author, v, x == "重" ? MUKOU : NISE)
      end
    else # 投票者合法
      x = "无" # 无效票
      if @rules[:banmul] and l.length > @rules[:vote_limit] # 多投Ban掉
        t.votes.each do |v|
          @comp.addt(t.author, v, MUKOU) # 记录无效票
        end
      else # 未限制多投或投票数符合要求
        t.votes.each_index do |i|
          break if flag >= @rules[:vote_limit]
          if k = @comp.haschara?(t.votes[i])
            @comp.addt(t.author, t.votes[i], k) # 正常计数
            flag += 1
            t.votes[i] = nil
          end
        end
        t.votes.delete(nil)
        t.votes.each do |v|
          @comp.addt(t.author, v, MUKOU) # 记录无效票
        end
      end
    end
    if !t.votes.empty?
      logfile.printf "[%s] @%s : ", x, t.author
      t.votes.each {|v| logfile.printf "%s ", v }
      logfile.printf "\n"
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
          u = uu[0].to_s.strip
          if u.include?(@rules[:chara_inc])
            g.add(u) # 规定的有效票
          elsif u.include?(@rules[:group_inc])
            @comp.addgroup(g)
            g = Group.new(u.gsub(ESCAPEGRPS, ''))
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

  def parseit(url, pid, pn, lastpn)
    if pn >= lastpn or !readcache(pid, pn)
      $stderr.print "新"
      page = Nokogiri::HTML(open(pageurl(url, pid, pn)))
      bg, ed = parse(page)
      writecache(pid, pn, bg, ed) if pn < lastpn
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
    clear
    page = Nokogiri::HTML(open("#{url}#{pid}"))

    @comp.title = page.css('.core_title_txt')[0].attr('title')
    CONTFORM.each do |k, v|
      if @comp.title.include?(k)
        @comp.buildblacklist(v[:blacklist]) unless @rules[:blacklist] == v[:blacklist]
        @comp.buildwhitelist(v[:whitelist]) unless @rules[:whitelist] == v[:whitelist]
        @rules = v
        break
      end
    end
    $stderr.printf "\n%s\n", @comp.title
    logfile.printf "\n%s\n", @comp.title

    pager = page.css('.l_posts_num').css('.l_reply_num')[0].text
    lastpn = pager.match(/共\s*([0-9]+?)\s*页/)[1].to_i
    $stderr.print "共#{lastpn}页 "

    for pn in 1..lastpn
      $stderr.print "[#{pn}"
      logfile.print "[#{pn}]\n"
      parseit(url, pid, pn, lastpn)
      $stderr.print "]"
    end

    logfile.print "完成\n"
    $stderr.print " 完成\n"
  end
end

if $0 == __FILE__
  out = ARGV[0] ? open(ARGV[0], "w") : STDOUT # 输出比赛结果
  inf = ARGV[1] ? open(ARGV[1], "w") : STDERR # 输出无效票详情
  cha = ARGV[2] ? "<<#{ARGV[2]}>>" : "<<真红>>" # 真红13骑士
  char = cha.force_encoding(Encoding::UTF_8)
  ls = Posts.new(inf)
  pid = Array.new
  while line = $stdin.gets and line[0] != '0'
    pid.push(line.to_i)
  end
  pid.each do |pt|
    ls.fetch(pt)
    ls.comp.output(out)
    ls.comp.info(char)
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
