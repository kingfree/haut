library(rstan)
# 下のデータを enko_top.csv で保存
dat <- read.csv("enko_top.csv", stringsAsFactors=TRUE)
anime <- c(dat$anime) # アニメindex
cv <- c(dat$cv)       # 声優index
stanmodel <- stan_model("enko.stan")
standata <- list(N=sum(dat$vote),
                 n_char=nrow(dat),
                 n_cv=length(unique(cv)),
                 n_bd=length(unique(anime)),
                 vote=dat$vote,
                 bd=log(dat$bd[!duplicated(dat$anime)], 10),
                 id_anime=anime,
                 id_cv=cv)

standata

fit <- sampling(stanmodel, data=standata, chains=3, warmup=500, iter=1500, seed=1234)
ex <- extract(fit)
cv_name <- levels(dat$cv)
char_name <- as.character(dat$character)

# キャラ解析
y <- apply(ex$p_char, 2, quantile, c(0.025, 0.5, 0.975))
y <- t(y)
rownames(y) <- char_name
idx1 <- y[,1] > dat$vote/sum(dat$vote) # 下限が実データより上
idx2 <- dat$vote/sum(dat$vote) > y[,3] # 上限が実データより下

matplot(y, type="n", ylab="得票率 [95%信用区間]", ylim=c(0, 0.1))
for(i in seq(nrow(dat))) segments(i, y[i, 1], y1=y[i, 3], lwd=3, col=(idx1+idx2*2)[i]+1)
lines(dat$vote/sum(dat$vote), lty=3)

# 声優解析
y <- apply(ex$b_cv, 2, quantile, c(0.025, 0.5, 0.975))
y <- t(y)
rownames(y) <- cv_name

# 切片とアニメ寄与度
t(mapply(quantile, list(ex$b, ex$b_bd)))
