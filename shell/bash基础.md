# bash基础

man bash
https://www.gnu.org/software/bash/manual/
https://www.gnu.org/software/bash/manual/html_node/index.html
http://tiswww.case.edu/php/chet/bash/NEWS
https://www.tldp.org/LDP/Bash-Beginners-Guide/html/

## 切换shell

```shell
# restarted
chsh -s $(which zsh)
```

## getopts shift

https://my.oschina.net/leejun2005/blog/202376

## printf

### loginfo

bash (>=4.2): 
`printf "%(%Y-%m-%d %H:%M:%S)T %s\n" -1 "log message"`

## while/for stdin

```
while read line
do
  echo "$line"
done < "${1:-/dev/stdin}"

for file in *; do echo $file; done

# range
for i in {1..5}; do echo $i; done
END=5
for i in $(seq 1 $END); do echo $i; done

# array数组
declare -a arr=("element1" "element2" "element3")
for i in "${arr[@]}"; do echo "$i"; done
ARRAY=()
ARRAY+=('foo')
ARRAY+=('bar')
echo "${ARRAY[@]}"

# for loop construct array
args=()
for i in {1..5}; do
  args+=("$i")
done
echo "${args[@]}"

# while loop construct array
args=()
i=1
while : ; do
  args+=("$i")
  if [[ ${i} > 5 ]]; then break; fi
  i=$((++i))
done
echo "${args[@]}"

# 错误的数组初始化，因为 cat | while 是一个新的shell
s3files=()
cat test | while read s3file; do s3files=( "${s3files[@]}" "${s3file}" ); done
# 验证方法
echo $$ $BASHPID
cat test | while read f; do echo $$ $BASHPID; done
# 解决方法是用process substitution
s3files=()
while read s3file; do s3files+=("${s3file}"); done < <(cat test)
echo "${s3files[@]}"
# "${s3files[@]}" 与 "${s3files[*]}" 的区别是，前者是多个string，后者会被合并为一个string
```
https://stackoverflow.com/questions/1951506/add-a-new-element-to-an-array-without-specifying-the-index-in-bash
https://www.cyberciti.biz/faq/bash-iterate-array/
https://www.linuxjournal.com/content/bash-arrays

Each command in a pipeline is executed in its own subshell: `echo abc | { echo $BASHPID; }`
https://www.gnu.org/software/bash/manual/html_node/Pipelines.html#Pipelines

## Word Splitting & IFS

https://www.gnu.org/software/bash/manual/html_node/Word-Splitting.html#Word-Splitting
https://www.cnblogs.com/ziyunfei/p/4898318.html
http://kodango.com/understand-ifs

## 重定向

### Here Documents

`command << word`可以指定一段多行文本，这段文本作为命令的输入，输入的起止位置被特定单词界定。如下面的例子中用`111`定界起止位置：
```shell
cat << 111
abc
def
111
```

参考：
* [1]，P209
* man bash
* https://www.gnu.org/software/bash/manual/html_node/Redirections.html#Here-Documents
* https://stackoverflow.com/questions/2953081/how-can-i-write-a-heredoc-to-a-file-in-bash-script

### Here Strings

更进一步的，`<<<`可以从一个变量，或者一行后面的内容读取输入。如：
```shell
a="abc"
cat <<< $a
cat <<< 12345678
```

## 内置命令帮助信息

`help`命令显示`bash`内置命令的帮助信息。比如：`help cd; help exec`

`man bash`之 SHELL BUILTIN COMMANDS 。

## 变量

默认情况下，在脚本中定义的任何变量都是全局变量，在函数中定义的也是。用`local`修饰变量可以限制变量的作用域（[1], P344,345）；local变量在后续调用的函数中可见。
注意：`local`只能在函数中用，在全局区域用local修饰变量，会报错。
```shell
#!/bin/bash
function func1 {
	echo "in func1"
	echo $a
	b="this is var b"
}
a="test a var"
echo $a
func1
echo $b
```

`local -`可以将set设置的变量本地化，在函数内set的属性只在函数内有效。(Bash 4.4以上版本)

### 变量引用形式

```shell
v="this is a test"
echo $v
echo ${v}
echo ${1} # 命令行第一个参数
```

### delcare

相对于上述变量定义方式，declare可以赋予变量attribute。
https://unix.stackexchange.com/questions/254367/in-bash-script-whats-the-different-between-declare-and-a-normal-variable
https://codingstandards.iteye.com/blog/1150109
https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html#Bash-Builtins

## 函数与调试

如果一个脚本比较多的使用函数，可以单独将环境变量、函数定义放到 lib.sh 中，在 main.sh 中通过 `source lib.sh` 加载环境变量和函数等定义。

这样做的好处是，可以在shell中通过 `source lib.sh` 对单个函数进行调试，而不需要调用整个脚本逻辑。

参考`man bash`的`source filename`一节。

## printf 优于 echo

`printf "%s\n" "$a"`
如果变量a中包含空格、tab等空白字符，必须加引号。

man 1 printf
http://c.biancheng.net/cpp/view/1499.html
http://unix.stackexchange.com/questions/65803/why-is-printf-better-than-echo

如果变量a中包含tab字符，echo输出需要增加引号才能打印tab，如`echo "$a"`。
http://stackoverflow.com/questions/26379243/how-to-split-string-by-tab-in-bash

## read 命令

bash内置的read命令，会对`\`进行转义，并且将tab转换为空格。如果不需要转义，可以用`-r`选项（但是，仍然会舍弃每行首位的tab等空白字符）。

参考：
`man bash`的`read`一节
http://wiki.bash-hackers.org/commands/builtin/read
http://stackoverflow.com/questions/11564778/bash-special-characters-lost-when-reading-file

## 日期运算

### 字符串转换为日期

```shell
date_fmt="+%Y-%m-%d %H:%M:%S"
date -d "20160612 08:10:20"
date -d "20160612 08:10:20 3 days" "$date_fmt"
```

### 时间戳转换为日期

```shell
date -d @1465815203
date -d @1465815203 +%Y-%m-%d
```

可以使用date命令进行日期运算：

### 加减一定天数

```shell
date -d "+1 day 2009-11-15" +%Y-%m-%d
date --date="yesterday"
date --date="tomorrow"
date --date="today"
date --date="now"
date  --date="2 days ago"
date --date="1 day"
date --date="-1 day"
date --date='5 hour'
date --date='5 minute'
date --date='60 second'
date --date='10 week'
date --date='10 month'
date --date='10 year'
date --date="tomorrow 6 hour"

# 指定utc时区 https://unix.stackexchange.com/questions/104088/why-does-tz-utc-8-produce-dates-that-are-utc8
# utc-8
TZ=UTC+8 date
# utc+8 北京时区
TZ=UTC-8 date

# 周运算
date  --date="next Friday"
# 获取UTC时间戳
date +%s
# 将特定日期转换为UTC时间戳
date -d "2009-11-15" +%s
date -d "2009-11-15 00:01" +%s
date -d "2009-11-15 00:00:01" +%s
# 获取年月日时分秒
date_fmt="+%Y-%m-%d %H:%M:%S"
date "$date_fmt"
```

参考：
http://www.cyberciti.biz/tips/linux-unix-get-yesterdays-tomorrows-date.html
http://stackoverflow.com/questions/11144408/convert-string-to-date-in-bash
http://www.tutorialarena.com/blog/get-unix-timestamp-in-bash.php

## 集合运算

以下命令的输入都需要排序。

uniq
comm

## 常用命令

### sort 

* -k2 按第2个字段排序。
* -n 按数字排序。默认按字符串排序。
* -r 按desc排序。默认按asc排序。
* -t 指定字段分隔符。

### join

类似数据库的join操作，连接两个文本文件。如`join -1 11 file1 -2 21 file2`
* 两个文件都必须是排序的。
* 如果制定分隔符为tab，使用选项`-t $'\t'`。默认分隔符是空白符。
* `-1`选项指定第一个文件的连接字段，`-2`选项制定第二个文件的连接字段。默认值都是1。
* 两个文件中有一个可以设定为`-`，即从标准输入读取。
* `-o`选项可以指定输出格式，格式串是逗号分隔的`FILENUM.FIELD`样式的字符串。FILENUM是文件编号，1或2，。FIELD是字段序号，从1开始。

### iconv utf-8 转码 gb18030

`iconv -f GB18030 -t UTF-8 1.md > 2.md`

## awk

`BEGIN { FS = "\x01" }`

### sum numbers in lines

`awk '{ sum += $1 } END { print sum }' file`

### 根据变量值过滤列、复合条件

`awk -v begin="${begin}" -v end="${end}" -F "/" '$3 >= begin && $3 <= end' | head`

### 删除最后一列

`awk 'NF{NF-=1};1' <in >out`
https://unix.stackexchange.com/questions/234432/how-to-delete-the-last-column-of-a-file-in-linux

### 打印单引号 single quotes

`awk 'BEGIN {FS=" ";} {printf "'\''%s'\'' ", $1}'`
https://stackoverflow.com/questions/9899001/how-to-escape-a-single-quote-inside-awk

## {}语句块 block

```
# echo和}前面必须带空格，否则会解析为命令 {echo 
flock -n 3 || { echo abc; exit 1; }
# 或者
flock -n 0 || {
  echo abc
  exit 1
}
```
https://unix.stackexchange.com/questions/390329/statement-blocks-mechanism-in-shell-scripting
https://stackoverflow.com/questions/21246552/bash-command-groups-why-do-curly-braces-require-a-semicolon

## flock

flock的command必须是PATH中的文件，不能是shell内置命令或者脚本内的函数。

https://blog.famzah.net/2013/07/31/using-flock-in-bash-without-invoking-a-subshell/

## script dir 脚本目录

`DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"`
https://stackoverflow.com/questions/59895/get-the-source-directory-of-a-bash-script-from-within-the-script-itself

## 数字转换

16进制转10进制：`echo $((16#7b))`
10进制转16进制：`echo "obase=16; 34" | bc`

## 控制字符、任意字符

```shell
x=$'\1'
echo ${x} > test
od -x test
```

## 变量替换、命令替换 Parameter expansion

```shell
echo $v
echo ${v}
echo ${11}
echo `date`
echo $(date)
# 第4个及后续参数
${@:4}  ${parameter:offset}
```

http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_06_02
`man bash` Parameter expansion
https://superuser.com/questions/935374/difference-between-and-in-shell-script
https://stackoverflow.com/questions/9449778/what-is-the-benefit-of-using-instead-of-backticks-in-shell-scripts
https://stackoverflow.com/questions/457120/whats-the-difference-between-and

### 默认值

`a=${parameter:-word}`
https://unix.stackexchange.com/questions/122845/using-a-b-for-variable-assignment-in-scripts

## substr

```shell
echo 1234567890 | cut -c 2-7
echo 1234567890 | head -c 4
```

## 日志迁移

nohup scp -C serving-sxs-app-01:/var/data/nginx/log/access.log.old /dev/stdout | bzip2 -ckz --quiet /dev/stdin > access1.bz2 &
tar czf - /var/data/docker/volumes/syncdata_mysql_data | ssh devops@172.16.1.212 "cd /var/data && tar zxf -"
tar -cz train/adapter/output/*/stat.csv | ssh server4 "cat >/home/ubuntu/log-stat-20190103/4.tar.gz"

## 查看进程关系

```shell
ps afjx
ps ef | grep defunct
```

## 变量的正则表达式比较

v必须用引号包起来，正则表达式不能用引号包起来。
```shell
v=abc.gz
if [[ "$v" =~ \.gz ]]
then
  echo "gz file"
else
  echo "not gz"
fi
```
man bash
https://unix.stackexchange.com/questions/340440/bash-test-what-does-do

## 获取文件名后缀

```shell
v=abc.gz
echo "${v##*.}"
```
man bash
https://unix.stackexchange.com/questions/147560/explain-this-bash-script-echo-1
https://zhidao.baidu.com/question/268947343.html

## grep

### 搜索前后n行

```shell
grep -5 pattern log.txt
grep -C 5 pattern log.txt
grep -A 5 -B 5 pattern log.txt
```

### perl模式

比如查找 tab 字符: `grep -P '\t' test.txt`

### 搜索0/null字符

```shell
# 后面不能用管道衔接more等命令，否则遇到0字符就中断输出。
grep -a -P "\x00" xdx.csv

# 删除null byte
tr -d "\000" < xdx.csv
sed 's/\x0//g' xdx.csv
```

## 添加换行符

`sed 's/$/\r/' test.txt | od -cx`

## 压缩

```shell
# plzip
tar cf - *.json | plzip -n 8 -k -9 -o sxs.tar
plzip -d sxs.tar.lz
```

## web压力测试

```shell
# apt install apache2-utils
ab -c 10 -n 1000 -t 3600 \
	-e c10.csv -k -l -q -r -s 5 \
	-p body.json \
	-T "Content-Type:application/json;charset=utf-8" \
	"http://localhost:8000/interns"
```

## curl/proxy

```
export http_proxy="http://39.134.10.4:8080/"
export https_proxy="14.118.253.118:6666/"
curl "http://www.sina.com.cn/"
curl "https://www.baidu.com/"
curl --socks5-hostname localhost:1080 http://www.baidu.com/
```

## set

`set -eou pipefail`
http://www.ruanyifeng.com/blog/2017/11/bash-set.html

## 文件描述符与重定向

`echo -e 'abc\ndef' > test.txt | grep abc`
各种shell的惯例是，把文件描述符0/1/2与stdin/stdout/stderr关联；但这与内核无关。如果不遵循这种惯例，很多unix程序将无法工作。(apue v3, ch3.2, p49)

内核使用3种数据结构表示打开的文件：(apue v3, ch3.10, p60/61)
* 每个进程在进程表中有一项，其中包括文件描述符表。每个文件描述符占文件描述符表的一项。文件描述符项中，一个字段是指向内核**文件表**项的指针。
* 内核为所有打开的文件维持一张**文件表**。文件表项包含指向文件v节点(或i节点)结构的指针。
* v节点
* 可能有多个文件描述符指向同一个文件表项；也可能有多个文件表项指向同一个v节点。fork后，父子进程的不同文件描述符指向同一个文件表项。(p62, p184 ch8.3)

## coprocess co-process

https://www.gnu.org/software/bash/manual/html_node/Coprocesses.html
https://unix.stackexchange.com/questions/86270/how-do-you-use-the-command-coproc-in-various-shells

## tmux

tmux new zjh-server  # new session
tmux a -t zjh-server

## 杂项

```shell
# 每10秒统计一次进程17555: io, mem, cpu, horizontal
nohup pidstat -d -h -r -u -p 11844 10 > pidstat_sort.txt &

# thread count 线程数统计 https://stackoverflow.com/questions/268680/how-can-i-monitor-the-thread-count-of-a-process-on-linux
ps -hH 4481 | wc -l
```

`&&` and `||` have equal precedence
https://www.gnu.org/software/bash/manual/html_node/Lists.html#Lists

## 注意事项

* 不要用`HOME`作为base目录，因为home文件系统的容量可能有限
* 不要用`./`等相对路径，这样无法在任意目录调用脚本

## 参考资料

[1] Linux命令行与shell脚本编程大全（第2版）




## 问题

bash中，如果一个变量有tab或换行符，无论是echo，还是cat或者 <<< ，都会被转换为空格。有没有其它方法保留tab？

函数只能通过echo返回结果。能否返回巨大的数据，比如几十兆的数据？或者这种最好通过全局变量？或者用Process Substitution？


