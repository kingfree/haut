# -*- coding: utf-8 -*-

import sys
import json
import re
# import jieba
# import MeCab

g_all = r'(\/\/.+\n)|(\@[^\s\&\:\)\@]+:)|(\#[^\#]+\#)|(http\:\/\/[a-zA-Z0-9\_\/\.\-]+)'

f = file("statistical.json")
s = json.load(f, encoding="utf-8")
f.close
r = '\n'.join(s).encode('utf-8')
t = re.sub(g_all, " ", r).strip()
print t
# m = MeCab.Tagger ("-Ochasen")
# print m.parse (t)
