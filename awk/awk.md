# awk基础

## 查看版本

`awk --version`

## 数组初始化

只有gawk才支持数组，传统的awk不支持数组。Ubuntu16下安装gawk后，会将awk指向gawk。

gawk3.1.7 (CentOs 6)不支持多维数组，4.1.3 (Ubuntu16) 可以支持多维数组。

http://stackoverflow.com/questions/14063783/initialize-an-array-in-awk
http://www.grymoire.com/Unix/Awk.html

## 关联数组

环境：gawk 4.1.3 (Ubuntu16)

示例脚本注意以下几点：


```awk
#!/usr/bin/awk -f

# 准备二维数组
BEGIN {
	# SUBSEP 是默认的split分隔符
	x = SUBSEP
	# 用变量 x 连接各个字符串值
	v_i = "0" x "1" x "2"
	# split结果放到数组 a_i 中
	split(v_i, a_i, x)
	ps = "111" x "222" x "333"
	split(ps, v_p, x)
	min_p = 0
	max_p = 20
	#准备二维数组
	for (type in a_i) 
		for (j = min_p; j <= max_p; j++) {
			a_p[type][j] = 0
		}
	for (type in a_i) 
		for (j in v_p) {
			a_pr[type][j] = 0
		}
}

# 对每一行统计一个ip
{
	a_p[$1][$4]++
	a_pr[$1][$2]++
}

# 打印ip个数统计结果
END {
	# 打印统计
	print "p stat:"
	for (type in a_i) 
		for (j = min_p; j <= max_p; j++) {
			print type"\t"j"\t"a_p[type][j]
		}
	print "pr stat:"
	# 打印统计
	for (type in a_i) 
		for (j in v_port) {
			print type"\t"v_pr[j]"\t"a_pr[type][j]
		}
}
```

## if语句

注意以下几点：
* if语句的条件部分如果分为多行，每行最后需要有斜杠。
* 几个逻辑操作符：与、或、非。

```awk
#!/usr/bin/awk -f

{
	if (\
		(! ($1 < "2")) && \
		(\
			( $2 == "aaa" && ($4 == "10" || $4 == "16") )\
				||\
			( $2 == "bbb" ) \
				||\
			$4 == "7" || $4 == "11" || $4 == "12"\
				||\
			( $2 == "ccc" && $4 == "10") \
		)\
	)
	{ print $1"\t"$2"\t"$3"\t"$4"\t"$5 } 
}
```


