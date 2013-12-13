
def insertion_sort!(a)
  for j in 1..a.length - 1
    key = a[j]
    # Insert a[j] into the sorted sequence a[0..j - 1]
    i = j - 1
    while i >= 0 and a[i] > key
      a[i + 1] = a[i]
      i = i - 1
    end
    a[i + 1] = key
  end
end

n = gets.to_i
a = Array.new(n) { |i| rand(n * 10) }
p a
insertion_sort!(a)
p a

