

## 模块

模块是对象，并且所有的模块都有一个内置属性`__name__`。一个模块的 __name__ 的值取决于如何应用该模块。
如果 import 一个模块，那么模块__name__ 的值通常为模块文件名，不带路径或者文件扩展名。
如果像一个标准的程序那样直接运行模块，__name__ 的值将是`__main__`。

http://www.cnblogs.com/xuxm2007/archive/2010/08/04/1792463.html

```python
if __name__ == '__main__':
	statements
```

这段代码的主要作用主要是让该python文件既可以独立运行，也可以当做模块导入到其他文件。
当导入到其他的脚本文件的时候，此时__name__的名字其实是导入模块的名字，不是__main__, main代码里面的就不执行了。

http://www.cnblogs.com/herbert/archive/2011/09/27/2193482.html

## 不带换行输出

```python
#!/usr/bin/python

import sys

for line in sys.stdin:
	#print 1, line
	sys.stdout.write(line)
```

## string

https://docs.python.org/2/library/string.html

substr
http://stackoverflow.com/questions/663171/is-there-a-way-to-substring-a-string-in-python

len(str)
http://www.tutorialspoint.com/python/string_len.htm

子串查找 find()

## 逐行读取文件

http://stackoverflow.com/questions/8009882/how-to-read-large-file-line-by-line-in-python

## json

```python
# -*- coding:utf-8 -*-
import json

### 从字符串解析
j = json.loads('{"one" : "1", "two" : "2", "three" : "3"}')
print j['two']

### 获取key
print j.keys()

### 从文件解析
with open('data.json') as data_file:    
	data = json.load(data_file)
```

https://docs.python.org/2/library/json.html
http://stackoverflow.com/questions/7771011/parse-json-in-python
http://stackoverflow.com/questions/2835559/parsing-values-from-a-json-file-in-python

## 获取对象类型

```python
if j[scan_type].__class__.__name__ != 'dict' :
	continue
```

## 异常处理

http://www.cnpythoner.com/post/104.html
https://docs.python.org/2/reference/compound_stmts.html#try
https://docs.python.org/2/library/exceptions.html

## 命令行参数解析

2.7以前的版本用 optparse ；2.7及以后的版本用 argparse 。
rhel6的版本是2.6.6

```python
# -*- coding:utf-8 -*-

import optparse

parser = optparse.OptionParser()
parser.add_option("-q", "--query", 
	action="store", dest="query", # dest定义了options的一个成员变量
	help="query string", default="akdfjak")
parser.add_option("-a", "--add", 
	action="store", dest="add",
	help="query string", default="tttt")
options, args = parser.parse_args();
print "Query string:", options.query
print options.add
```

http://stackoverflow.com/questions/20063/whats-the-best-way-to-grab-parse-command-line-arguments-passed-to-a-python-scri
http://stackoverflow.com/questions/3217673/why-use-argparse-rather-than-optparse

## 常量

Python中貌似在语法上不能直接定义一个符号为常量（类似C++的const），尽管PEP 8定义了常量的命名规范为大写字母和下划线组成。

http://bbs.chinaunix.net/thread-580685-1-1.html
http://code.activestate.com/recipes/414140/
http://www.malike.net.cn/blog/2013/11/03/python-constants/

## 注释

`#`后面的字符被认为是注释。`#`可以在一行的任何位置。

放在模块、函数、类顶端的字符串，会被Python自动封装，成为对象的`__doc__`属性（学习手册，P385，P97）。这些文档字符串可以用3个引号包起来多行文本，如：
```python
"""
This is mutiple 
line document.
"""
```

### 中文注释

在第一行添加`# -*- coding:utf-8 -*-`，必须是第一行。
如果要在第一行使用`#!/usr/bin/python`，可以在其它代码文件中使用中文注释，再import。或者用 python my.py。

http://15975604853.blog.163.com/blog/static/1182880352012116574235/
http://blog.csdn.net/MONKEY_D_MENG/article/details/5758572
https://www.python.org/dev/peps/pep-0263/

## 显示一个对象的帮助信息

`dir`命令可以显示一个对象支持的属性、操作函数等信息。`help`可以显示一个对象或对象方法的详细帮助信息。

```python
s = "abc"
dir(s)
help(s.strip)
```

## http请求

http://stackoverflow.com/questions/645312/what-is-the-quickest-way-to-http-get-in-python
http://www.oschina.net/code/snippet_54100_7485

## 变量

Python中没有变量声明、初始化的概念。但是，变量在使用之前，至少要赋一次值（学习手册，P126）。

http://stackoverflow.com/questions/11007627/python-variable-declaration

## try catch 与作用域

Python中没有block作用域的概念，只有函数、外包、全局、内置作用域。

```
#!/usr/bin/python

try:
	a = "abc"
except:
	pass
print a
```

http://stackoverflow.com/questions/17195569/using-a-variable-in-a-try-catch-finally-statement-without-declaring-it-outside
http://stackoverflow.com/questions/291978/short-description-of-python-scoping-rules
Python学习手册，P419

## 标准io stdin stdout stderr

```python
sys.stdout.write('Dive in')
```

http://www.diveintopython.net/scripts_and_streams/stdin_stdout_stderr.html

## 字符串与数值的转换

```python
print str(10) + " is ten"
print int('999') + 1
```

## 嵌套字典（多维关联数组）初始化

对每一维的每个值，都要显示赋值`{}`。所以，下面第一个for语句必不可少。
```python
stat_port = {}
for i in idctype:
	stat_port[i] = {}
	stat_port[i]['80'] = 0
for i in idctype:
	for j in port:
		print i, j
		stat_port[i][j] = 0
```
也可以动态创建：
```python
if not v_idctype in stat_port.keys():
	stat_port[v_idctype] = {}
```

因为字典`stat_port['1']`的value可能是任何类型，如果是字典类型，需要通过一个空字典显式告诉编译器。

## 序列

序列包括字符串、列表和元组（学习手册，P92）。

序列内置支持的操作包括索引访问、slice切片、迭代器等。
```python
s = 'abcde'
print s[2]
print s[1:3]	# 相当于C++的 begin/end迭代器，3不被包含
print s[:-1]	# -1相当于 len(s)-1
for c in s:
	print c
```

## 自增操作

Python不支持`++`操作符，但支持`a += 1`操作符。

## import和from

一个py文件就是一个模块。模块是Python最大的程序结构。（学习手册，P63，P68）

`import`和`from sys import exit`语句都可以导入一个模块。import一个模块的时候，会导入该模块的全部内容，如果该模块有全局语句，这些语句在导入的最后一步会被执行（学习手册，P65）。
import会复制imported模块的所有变量到importing模块中来（学习手册，P63，64）。下面第2个例子可以看到，函数也是一种变量。
即使使用`from sys import exit`这样的语句，也会导入整个模块，并执行模块中的全局语句。
参考下面的例子（运行`python test2.py`）：
```python
# test1.py
print "test1"
a = 1
```

```python
# test2.py
#from test1 import a
import test1
print test1
print test1.a + 1
```

从上面的例子可以看到，模块也是一个对象。

每个文件（模块）都是一个独立的命令空间。不同模块中同名的变量在各自的命名空间中，彼此不冲突。from导入缩短变量的名字；但也存在名字冲突、覆盖的可能。

函数也是一种变量：
```python
# test2.py
import test1
print test1
print test1.a + 1
print test1.printtest
test1.printtest()
test1.printtest = 112233
print test1.printtest
```

import导入的模块，只会执行一次。比如test模块被test1、test2导入，test1、test2被另一个文件main.py导入，运行main.py时，test.py的全局语句只会被执行一次。
```python
# test.py
print "in test.py"

# test1.py
import test
print "in test1.py"

# test2.py
import test
print "in test2.py"

# main.py
import test1
import test2
print "in main"
```

### 模块别名

```python
import module as mo
print mo.foo
```

### 参考资料

http://stackoverflow.com/questions/710551/import-module-or-from-module-import
http://stackoverflow.com/questions/9439480/from-import-vs-import




## 问题

如何将异常信息打印到stderr？





## 参考资料

Python 学习手册
