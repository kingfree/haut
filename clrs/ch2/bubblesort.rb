
def bubblesort(a)
  0.upto(a.length - 2) do |i|
    (a.length - 1).downto(i + 1) do |j|
      if a[j] < a[j - 1]
        a[j], a[j - 1] = a[j - 1], a[j]
      end
    end
  end
end

n = gets.to_i
a = Array.new(n) { |i| rand(n * 10) }
p a
bubblesort(a)
p a

