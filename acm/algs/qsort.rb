
def qsort(a)
  return [] if a.empty?
  x, *xs = *a
  l, r = xs.partition {|y| y < x}
  qsort(l) + [x] + qsort(r)
end

n = gets.to_i
a = Array.new(n) {|i| rand(n * 10) }
p a
p qsort(a)
p a.sort
