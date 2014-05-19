class Node
  attr_accessor :权值, :符号, :左子, :右子, :父亲

  def initialize(参数 = {})
    @权值 = 参数[:权值] || 0
    @符号 = 参数[:符号] || ''
    @左子 = 参数[:左子] || nil
    @右子 = 参数[:右子] || nil
    @父亲 = 参数[:父亲] || nil
  end

  def 遍历(&块)
    遍历结点('', &块)
  end

  def 遍历结点(码, &块)
    yield(self, 码)
    @左子.遍历结点(码 + '0', &块) unless @左子.nil?
    @右子.遍历结点(码 + '1', &块) unless @右子.nil?
  end

  def 叶?
    @符号 != ''
  end

  def 初始化?
    @符号 == ''
  end

  def 根?
    初始化? and @父亲.nil?
  end

end

class NodeQueue
  attr_accessor :结点, :根

  def initialize(字串)
    频率 = {}
    字串.each_char do |c|
      频率[c] ||= 0
      频率[c] += 1
    end
    @结点 = []
    频率.each do |c, w|
      @结点 << Node.new(:符号 => c, :权值 => w)
    end
    生成树
  end

  def 生成树
    while @结点.size > 1
      有序列 = @结点.sort { |a,b| a.权值 <=> b.权值 }
      新结点 = []
      2.times { 新结点 << 有序列.shift }
      有序列 << 合并结点(新结点[0], 新结点[1])
      @结点 = 有序列
    end
    @根 = @结点.first
  end

  def 合并结点(结点1, 结点2)
    左子 = 结点1.权值 > 结点2.权值 ? 结点2 : 结点1
    右子 = 左子 == 结点1 ? 结点2 : 结点1
    结点 = Node.new(
      :权值 => 左子.权值 + 右子.权值,
      :左子 => 左子,
      :右子 => 右子
    )
    左子.父亲 = 右子.父亲 = 结点
    结点
  end

end

class Huffman
  attr_accessor :根, :查找, :输入, :输出

  def initialize(输入)
    @输入 = 输入
    @根 = NodeQueue.new(输入).根
    @输出 = 编码字串(输入)
  end

  def 查找
    return @查找 if @查找
    @查找 = {}
    @根.遍历 do |结点, 码|
      @查找[码] = 结点.符号 if 结点.叶?
    end
    @查找
  end

  def 编码(字)
    self.查找.invert[字]
  end

  def 解码(码)
    self.查找[码]
  end

  def 编码字串(字串)
    码 = ''
    字串.each_char do |c|
      码 += 编码(c) 
    end
    码
  end

  def 解码字串(码)
    码 = 码.to_s
    字串 = ''
    子码 = ''
    码.eachchar do |位|
      子码 += 位
      unless 解码(子码).nil?
        字串 += 解码(子码)
        子码 = ''
      end
    end
    字串
  end

  def to_s
    @输出
  end

  def [](字)
    编码(字)
  end

end

class String
  def huffman
    @huffman ||= Huffman.new(self)
  end
end

puts 测 = "Hello Huff".huffman
测.查找.each do |码, 字|
  puts "#{码} : #{字}"
end
