# encoding: utf-8
import re
import unicodedata
g = open("enko.txt", "rU")
pat = ["\d+位", "\d+票", "[票|\s{2}\S].*?\s\(", "\(.+\)"]
res = map(lambda x: re.compile(x), pat)

w0 = open("enko1.txt", "w")
a = 1
tmp = "start"
while a < 1000 and len(tmp) > 0:
  tmp = g.readline().strip().replace("　", " ")
  if "票" in tmp:
    ex = map(lambda x: re.findall(x, tmp)[0], res)
    f = [ex[0].replace("位", ""),
    ex[1].replace("票", ""),
    re.sub("\s.*?\(", "", re.sub(r".*票\s", "", ex[2])),
    ex[3][1:-1]
    ]
  else:
    ex = map(lambda x: re.findall(x, tmp)[0], res[2:])
    f = [f[0], f[1], re.sub("\s.*?\(", "", re.sub(r".\s{2,}", "", ex[0])), ex[1][1:-1]]

  a += 1
  w0.write(",".join(f) + "\n")

w0.close()
