#!/bin/sh
i=0
sum=0
while [[ true ]]; do
    if [[ "$i" -ge "5" ]]; then
        break
    fi
    let "i+=1"
    read -p "输入第 $i 个数: " x
    let "sum+=x"
done
echo "求和: $sum"
