library(rstan)

stanmodel <- stan_model("vote.stan")
v <- c(727, 583, 137) # それぞれの得票数
standata <- list(N=length(v), v=v)
fit <- sampling(stanmodel, data=standata, chains=3, warmup=500, iter=1500, seed=1234)
ex <- extract(fit)
hist(apply(ex$p[,2:1], 1, diff), xlab="得票率差[%]", main="")
