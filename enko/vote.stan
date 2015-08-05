# vote.stan
data {
  int<lower=0> N;       # 候補者数
  vector<lower=0>[N] v; # 得票数
}
parameters {
  simplex[N] p;         # 得票確率
}
model {
  p ~ dirichlet(v);
}
