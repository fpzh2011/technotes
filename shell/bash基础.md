# bash基础

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

默认情况下，在脚本中定义的任何变量都是全局变量，在函数中定义的也是。用`local`修饰变量可以限制变量的作用域（[1], P344,345）。
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
# 周运算
date  --date="next Friday"
# 获取UTC时间戳
date +%s
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

## 数字转换

16进制转10进制：`echo $((16#7b))`
10进制转16进制：`echo "obase=16; 34" | bc`

## 参考资料

[1] Linux命令行与shell脚本编程大全（第2版）




## 问题

bash中，如果一个变量有tab或换行符，无论是echo，还是cat或者 <<< ，都会被转换为空格。有没有其它方法保留tab？



