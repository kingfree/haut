# 统计单词个数

class HashFold # 单线程
  def start(inputs)
    hash = {}
    inputs.each do |input|
      self.map(input) do |k, v|
        if hash.key?(k)
          hash[k] = self.fold(hash[k], v)
        else
          hash[k] = v
        end
      end
    end
    hash
  end
end

class HashFold2 # 多线程，Linux only
  def hash_merge(hash, k, v)
    if hash.key?(k)
      hash[k] = self.fold(hash[k], v)
    else
      hash[k] = v
    end
  end

  def start(inputs)
    hash = nil
    inputs.map do |input|
      p, c = IO.pipe
      fork do
        p.close
        h = {}
        self.map(input) do |k, v|
          hash_merge(h, k, v)
        end
        Marshal.dump(h, c)
      end
      c.close
      p
    end.each do |f|
      h = Marshal.load(f)
      if hash
        h.each do |k, v|
          hash_merge(hash, k, v)
        end
      else
        hash = h
      end
    end
    hash
  end
end

class HashFold3 # 进程池，Linux only
  class Pool
    def initialize(hf, n)
      pool = n.times.map {
        c0, p0 = IO.pipe
        p1, c1 = IO.pipe
        fork do
          p0.close
          p1.close
          loop do
            input = Marshal.load(c0) rescue exit
            hash = {}
            hf.map(input) do |k, v|
              hf.hash_merge(hash, k, v)
            end
            Marshal.dump(hash, c1)
          end
        end
        c0.close
        c1.close
        [p0, p1]
      }
      @inputs = pool.map{|i, o| i}
      @outputs = pool.map{|i, o| o}
      @ichann = @inputs.dup
      @queue = []
      @results = []
    end

    def flush
      loop do
        if @ichann.empty?
          o, @ichann, e = IO.select([], @inputs, [])
          break if @ichann.empty?
        end
        break if @queue.empty?
        Marshal.dump(@queue.pop, @ichann.pop)
      end
    end
    private :flush

    def push(obj)
      @queue.push obj
      flush
    end

    def fill
      t = @results.size == 0 ? nil : 0
      ochann, i, e = IO.select(@outputs, [], [], t)
      return if ochann == nil
      ochann.each do
        c = ochann.pop
        begin
          @results.push Marshal.load(c)
        rescue => e
          c.close
          @outputs.delete(c)
        end
      end
    end
    private :fill

    def result
      fill
      @results.pop
    end
  end

  def initialize(n = 2)
    @pool = Pool.new(self, n)
  end

  def hash_merge(hash, k, v)
    if hash.key?(k)
      hash[k] = self.fold(hash[k], v)
    else
      hash[k] = v
    end
  end

  def start(inputs)
    inputs.each do |input|
      @pool.push(input)
    end
    hash = {}
    inputs.each do |input|
      @pool.result.each do |k, v|
        hash_merge(hash, k, v)
      end
    end
    hash
  end
end

class WordCount < HashFold
  STOP_WORDS = %w(a an and are as be for if in is it of or the to with)

  def map(document)
    open(document) do |f|
      for line in f
        line.gsub!(/[!#"$%&\'()*+,-.\/:;<>=?@\[\]\\^_{}\|~"]/, " ")
        for word in line.split
          word.downcase!
          next if STOP_WORDS.include?(word)
          yield word.strip, 1
        end
      end
    end
  end

  def fold(c1, c2)
    return c1 + c2
  end
end  

WordCount.new.start(ARGV).sort_by{|x| x[1]}.reverse.take(20).each do |k, v|
  print k, ": ", v, "\n"
end

