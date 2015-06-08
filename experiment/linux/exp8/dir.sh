#!/bin/sh
read -p "输入目录:" dir
echo "下面是目录'$dir'中当前用户具有读权限的文件"
ls -l $dir | grep "^.r" # 正则表达式
