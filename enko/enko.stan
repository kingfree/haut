# enko.stan
data{
  int N;                           # 総投票数
  int n_char;                      # キャラ総数
  int n_cv;                        # 声優総数
  int n_bd;                        # アニメ総数
  int<lower=0> vote[n_char];       # 得票数
  real<lower=2, upper=5> bd[n_bd]; # 円盤売上を常用対数
  int<lower=0> id_anime[n_char];   # アニメのindex
  int<lower=0> id_cv[n_char];      # 声優のindex
}
parameters {
  real b;                          # 切片
  real b_bd;                       # アニメ寄与度
  real b_cv[n_cv];                 # 声優寄与度
}
transformed parameters {
  simplex[n_char] p_char;
  for(i in 1:n_char)
    p_char[i] <- inv_logit(b + b_cv[ id_cv[i] ] + b_bd*bd[ id_anime[i] ]);
  
  p_char <- p_char/sum(p_char);    # 総和 1
}
model {
  vote ~ multinomial(p_char);
}
