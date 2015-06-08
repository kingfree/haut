#!/bin/sh
read -p "输入成绩:" score
if [[ "$score" -gt "100" ]]; then
    echo "成绩不能超过100！"
elif [[ "$score" -lt "0" ]]; then
    echo "成绩不能为负数！"
elif [[ "$score" -lt "60" ]]; then
    echo "不及格"
elif [[ "$score" -lt "80" ]]; then
    echo "良好"
elif [[ "$score" -lt "100" ]]; then
    echo "优秀"
elif [[ "$score" = "100" ]]; then
    echo "完美"
else
    echo "成绩无效！"
fi
