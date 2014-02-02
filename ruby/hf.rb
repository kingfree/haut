# 统计单词个数

class HashFold1 # 单线程
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

class HashFold # 多线程
  def hash_merge(hash, k, v)
    if hash.key?(k)
      hash[k] = self.fold(hash[k], v)
    else
      hash[k] = v
    end
  end

  def start(inputs)
    hash = nil
    inputs.map do
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


