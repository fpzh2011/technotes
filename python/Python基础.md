# Python基础

## python3

```shell
sudo apt-get install python3-pip
```

## 关键字

### 打印关键字列表

```python
import keyword
print keyword.kwlist
```

### 关键字is

`is` tests for identity, not equality. That means Python simply compares the memory address a object resides in.
https://stackoverflow.com/questions/2987958/how-is-the-is-keyword-implemented-in-python

### 关键字del

`del`语句只是删除变量名对对象的引用，并不会立即释放内存或触发gc。
https://docs.python.org/2.0/ref/del.html
https://stackoverflow.com/questions/14969739/python-del-statement
https://stackoverflow.com/questions/1316767/how-can-i-explicitly-free-memory-in-python

### 关键字exec

`exec`语句解析一个字符串、或文件，并执行其中的Python代码。
https://docs.python.org/2.0/ref/exec.html
https://stackoverflow.com/questions/2220699/whats-the-difference-between-eval-exec-and-compile-in-python

### 关键字or

`or`可以实现类似`coalesce`的操作：`s = s1 or 'foo'`
https://docs.python.org/2/reference/expressions.html#boolean-operations
https://stackoverflow.com/questions/4978738/is-there-a-python-equivalent-of-the-c-sharp-null-coalescing-operator

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
    
#print('....', end='') #Python3
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
    json.dumps(data, indent=2, sort_keys=True)
```

https://docs.python.org/2/library/json.html
http://stackoverflow.com/questions/7771011/parse-json-in-python
http://stackoverflow.com/questions/2835559/parsing-values-from-a-json-file-in-python

## 动态类型

### 判断类型

```python
b = True
print type(b) == bool
# 判断对象是否某个类型（如基类object）的实例
isinstance(b, object)
# 判断类A是否是object的子类
issubclass(A, object)
```

### 获取对象类型名称

```python
if j[scan_type].__class__.__name__ != 'dict' :
    continue
# 判断类型
if not isinstance(j[scan_type], dict):
    continue
# 打印完整类型名称
o = j[scan_type]
print o.__module__ + "." + o.__class__.__name__
```

## 异常处理

http://www.cnpythoner.com/post/104.html
https://docs.python.org/2/reference/compound_stmts.html#try
https://docs.python.org/2/library/exceptions.html

### 环境管理器（context manager），with/as语句

Python学习手册，P851。

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

argparse的例子：
```python
import argparse
parser = argparse.ArgumentParser(description='输入IP C段地址，生成trace path graph的dot格式文件')
parser.add_argument('--ipdb', default='abc', required=True, help='IP库文件')
args = parser.parse_args()
print args.ipdb
```

http://stackoverflow.com/questions/20063/whats-the-best-way-to-grab-parse-command-line-arguments-passed-to-a-python-scri
http://stackoverflow.com/questions/3217673/why-use-argparse-rather-than-optparse
http://python.usyiyi.cn/python_278/library/argparse.html
http://blog.xiayf.cn/2013/03/30/argparse/

## 常量

Python中貌似在语法上不能直接定义一个符号为常量（类似C++的const），尽管PEP 8定义了常量的命名规范为大写字母和下划线组成。

学习手册，P308。
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

### 命名管理、本地变量、私有变量

学习手册，P307。
* `_x`不会被`from module import *`导入。
* `__x__`是系统定义的变量名，对解释器有特殊意义。
* `__x`是类的本地变量（前面有两个下划线），系统会自动在变量名前添加类名。如果父类定义了`__x`，子类也不能访问。

```python
class A:
    def __init__(self):
        self._a = 9
class B(A):
    def __init__(self):
        A.__init__(self)
        self.b = self._a
b = B()
print(b.b)
```

http://stackoverflow.com/questions/1301346/what-is-the-meaning-of-a-single-and-a-double-underscore-before-an-object-name

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

## 数据转换

### 字符串与数值的转换

```python
print str(10) + " is ten"
print int('999') + 1
```

### IP地址转换

以下只支持紧凑格式，不支持`192.168.004.054`这种格式。
```python
import netaddr
print int(netaddr.IPAddress('192.168.4.54'))
#3232236598
print str(netaddr.IPAddress(3232236598))
#192.168.4.54
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

## 数据类型分类

有多种方法对Python的类型进行分类。

按对象是否可变，可以分为：
* 不可变类型，如数字、字符串、元组、不可变集合。
* 可变类型，如列表、字典、可变集合。

按类型操作等特征，可分为：
* 数字，包括整数、浮点数、二进制、分数等。
* 序列，包括字符串、列表、元组等。
* 映射，如字典。

### 字典

https://docs.python.org/2/library/stdtypes.html#mapping-types-dict
https://docs.python.org/2/glossary.html#term-hashable

`dict`的key必须是`hashable`。`set`也是这样。
如果字符串作为key，是按值区分distinct的，而不是按对象引用。如下面的例子，值相同的两个字符串，作为key初始化dict，dict只保留一个key：
```python
# python会缓存较短的字符串。所以测试的字符串必须很长。
a1 = 'a'*600
a2 = 'a'*600
a1 is a2
a1 == a2
d = {a1:1, a2:2}
len(d)
```
如果自定义类作为key，而没有重载`object.__hash__()`函数，则两个不同对象会是不同的key：
```python
class C(object):
    pass
c1 = C()
c2 = C()
d = {c1: 1, c2:2}
len(d)
```

### 字符串

字符串str是字节序列，unicode是字符序列，unicode可以根据编码方式（如utf-8）转为字符串str。
字符串在Python中具有不可变性。Python学习手册，P94。
`\0`不表示字符串的结束。Python学习手册，P172。
```python
a = r'\t'
print a, a.__class__.__name__
a = r'中国'
print a, a.__class__.__name__
a = u'中国'
print a, a.__class__.__name__
```

### unicode码转换

```python
import json
s = '{"msg": "\u65e0\u6b64\u7b80\u5386!", "code": 500}'
j = json.loads(s)
print j.get('msg').__class__.__name__
print j.get('msg')
```

如果用`print u'中国'`重定向到文件，会报错UnicodeEncodeError。
如果用`open('1.txt', 'w')`然后write unicode，也会报错。
用`io.open('1.txt', 'w')`就正常运行。

### 整数

Python对整数的大小在逻辑上没有限制，只有一些物理限制，如不能超过内存大小。
较小的整数，Python会处理为[Plain Integer Objects](https://docs.python.org/2/c-api/int.html)，用`i.__class__.__name__`显示为`int`。普通整数的值域与系统平台相关，如CentOS7_64上最大是2的64次方减1。
如果数字变得更大，Python会自动转换为[Long Integer Objects](https://docs.python.org/2/c-api/long.html)，用`i.__class__.__name__`显示为`long`。

```python
i = 1 << 62
print i, i.__class__.__name__
# 按64位有符号整数，1左移63位变为-1，但是Python自动转为long
i = 1 << 63
print i, i.__class__.__name__
```

但是`float`是有最大值的。如下代码会报错：
```python
import sys
# 显示float最大值
sys.float_info
# 超过值域的赋值会报错
i = pow(10,1000)
f = i + 0.1
```

http://stackoverflow.com/questions/9860588/maximum-value-for-long-integer
http://stackoverflow.com/questions/3477283/what-is-the-maximum-float-in-python
http://stackoverflow.com/questions/3477283/what-is-the-maximum-float-in-python

### 序列

序列包括字符串、列表和元组（学习手册，P92）。

序列内置支持的操作包括索引访问、slice切片、迭代器、合并、重复等。
```python
s = 'abcde'
print s[2]
print s[1:3]    # 相当于C++的 begin/end迭代器，3不被包含
print s[:-1]    # -1相当于 len(s)-1
for c in s:
    print c
# 合并与重复
a = [1,2,3]
b = a * 2        # 重复
c = a + b        # 合并
```

Python貌似没有数组类型。list就是可以动态扩展的数组。

### 列表解析

学习手册，P367-370，P495-500，

在Python3中，map、filter、reduce都是可迭代对象；而在Python2中都不是可迭代对象，map、filter直接返回列表，reduce返回计算结果。（学习手册P489）
可以用`itertools.imap`和`itertools.ifilter`返回generator object。`reduce`还没有发现可以返回generator object的版本。

```python
a = [(1, 2, 3)]
# b是元组
for b in a:
    print b
# b,c,d 解开元组
for b,c,d in a:
    print b,c,d
'''
变量个数要么是1，要么与元组的元素个数相同，否则错误
for b,c in a:
    print b,c
'''

a = [[1,2,3]]
# b是列表
for b in a:
    print b
# b,c,d 解开列表
for b,c,d in a:
    print b,c,d
'''
变量个数要么是1，要么与列表的元素个数相同，否则错误
for b,c in a:
    print b,c
'''
```

### None类型

[None](https://docs.python.org/2/library/constants.html#None)是[types.NoneType](https://docs.python.org/2/library/types.html#types.NoneType)类型的singleton对象。任何对`None`的赋值、添加属性等操作都会报错；手动创建`types.NoneType`对象也会报错。

`None`和`__debug__`是真正的常量，都不能进行任何修改。而`True`和`False`是可修改的。

判断一个变量是否是`None`，应该用`is`或`is not`。`==`虽然也可以，但是对singleton对象不推荐使用。
https://stackoverflow.com/questions/23086383/how-to-test-nonetype-in-python
https://www.python.org/dev/peps/pep-0008/#programming-recommendations

### float

`nan, inf`这些值都是[IEEE 754](https://en.wikipedia.org/wiki/IEEE_floating_point)定义的，和1、2、3一样都是可存储为float的值。所以，下面的这些语句都是合理、合法的。
```pytyhon
x = float('NaN')
x = float('inf')
x = -x
x = float('-inf')
float('inf') == float('inf') # Fasle
float('inf') is float('inf') # Fasle
import math
math.isnan(x)
```

## 自增操作

Python不支持`++`操作符，但支持`a += 1`操作符。

## 类class

### 静态属性
类的静态属性（数据成员）。直接对类的属性进行赋值，就可以创建类的静态属性，如`A.a = 1`，或者在类定义的顶层赋值，如`calss A: x = 1`（P699）。

### 静态方法、类方法

类的静态方法，在`def`语句前一行添加`@staticmethod`修饰符（P691,796）。参考：https://docs.python.org/2/library/functions.html#staticmethod

类的类方法，在`def`语句前一行添加`@classmethod`修饰符。

[静态方法与类方法的区别](http://blog.csdn.net/handsomekang/article/details/9615239)：
* `@classmethod`不需要表示对象自身的`self`参数，但第一个参数需要是表示类自身的`cls`参数。
* `@staticmethod`不需要表示对象自身的`self`参数，也不需要表示类自身的`cls`参数，就跟调用普通函数一样。
示例如下：
```python
class B(object):
    def static_foo(self):
        print('staitc foo in B')

class A(B):
    bar = 1
    def foo(self):
        print 'foo'
        self.static_foo()

 
    @classmethod
    def class_foo(cls):
        print 'class_foo'
        print cls.bar
        cls.static_foo()
        cls().foo()
a = A()
a.foo()
A.static_foo()
A.class_foo()
```

### 抽象超类

P695-697
Python2中，类如果包含用[@abstractmethod](https://docs.python.org/2/library/abc.html)修饰的成员函数，就是成员类，该成员函数的具体实现由子类提供。

Python中有三种方法实现，一种是在方法中使用`assert`语句，第二种是在方法中抛出`NoImplementedError`异常，但这两种方法都可以创建对象，只是不能调用方法。
第三种方法利用`@abstractmethod`，如果没有实现所有`@abstractmethod`修饰的方法，无法创建对象。示例如下：
```python
# abc: abstract base class
from abc import ABCMeta, abstractmethod

class C():
    # __metaclass__ 的设置是必需的，否则仍可以创建对象
    __metaclass__ = ABCMeta

    @abstractmethod
    def f(self):
        pass
```

### 新式类

继承自`object`的类是新式类（P779）。
CentOS7_64, Python2.7.5下，不必显示声明，类自动继承object。

## python exception stack trace问题

```python
# test_B.py
class Base(object):
    def f1(self):
        print('Base f1')

    def f(self):
        self.f1()
        print('Base f')

# test_D.py
class D(Base):
    def f1(self, a):
    #def f1(self):
        print('D f1')

# test_main.py
if __name__ == '__main__':
    d = D()
    d.f()

# python test_main.py
```

## 装饰器decorator

类似Java的注解。P805, 979。

python学习手册
* ch31, ch37, ch38

## 元类

P808，1046

## import和from

一个py文件就是一个模块。模块是Python最大的程序结构。（学习手册，P63，P68）
注意：如果要导入a.py中的类a，需要`import a.a`，而不是`import a`。

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

## 日志

如果程序将结果输出到标准输出，而又想将错误信息与结果分开，可以用这种方式（学习手册，P317）：
```python
import sys
print >> sys.stderr, "error info"
print "normal output" 
```

如果需要详细的日志输出，可以用logging组件。参考：
http://blog.csdn.net/fxjtoday/article/details/6307285
http://crazier9527.iteye.com/blog/290018
https://docs.python.org/2/library/logging.html

[多线程输出同步](https://docs.python.org/2/library/logging.html#thread-safety)
[multiprocessing的日志同步](https://stackoverflow.com/questions/641420/how-should-i-log-while-using-multiprocessing-in-python)

[API文档](https://docs.python.org/2.6/library/logging.html)
```python
# -*- coding:utf-8 -*-

import logging
import logging.handlers

logger = logging.getLogger("test-log")
logger.setLevel(logging.INFO)
handler = logging.FileHandler("my.log",  encoding='utf8')
# TimedRotatingFileHandler
#handler = logging.handlers.RotatingFileHandler("my.log", maxBytes=100*1024*1024, encoding='utf8')
# Console
# handler = logging.StreamHandler(sys.stdout)

# 输出级别，时间，[模块，文件，函数，行号]，消息
formatter = logging.Formatter("%(levelname)s %(asctime)s [%(module)s %(filename)s %(funcName)s %(lineno)d] %(message)s")
handler.setFormatter(formatter)
logger.addHandler(handler)
logger.info(u"abc 中国")
```

如果输出到console且未指定stream，默认输出到stderr。
https://docs.python.org/2/howto/logging.html
https://docs.python.org/2/library/logging.html#logging.basicConfig

## 循环

Python中的for只能处理序列（学习手册，P343）。当Python运行for循环时，会逐个将序列中的元素赋值给目标。注意，是赋值，不是引用（学习手册，P344，P353）。
Python中没有类似C/C++中的for循环，如：
```c
for(int i = 0; i < N; ++i) {
    //do something ...
}
```
Python实现类似功能，可以用range函数。

### for循环

for循环可用于任何可迭代的对象（学习手册P361）。可迭代对象是序列观念的通用化。

## 迭代协议 迭代器

学习手册（P362）
迭代协议：有`__next__`方法的对象，用于迭代工具时（如for语句），会逐个前进到下一个结果，而在一系列结果的末尾时，会引发StopIteration异常。
在Python中，所有迭代工具都是在每次迭代中调用`__next__`，并捕捉StopIteration异常来确定何时离开。

### 迭代环境

下列情况会应用迭代协议（学习手册P371）：
* for语句
* 列表解析
* in成员关系测试
* map、zip、sorted、enumerate、filter、reduce、sum、any、all、max、min等内置函数

## sort 排序

https://www.cnblogs.com/ajianbeyourself/p/5395653.html

稳定排序
https://docs.python.org/2/howto/sorting.html#sort-stability-and-complex-sorts
https://docs.python.org/2/library/stdtypes.html#iterator-types

## 程序结构

学习手册P280
if/while等语句，没有括号表示语句块，而是用缩进表示语句块，缩进的结束就是代码块的结束。

## 作用域

### LEGB原则

学习手册，P418, P427

Python没有语句块作用域。在try、if等语句块中定义的变量，作用域是Local，即所在函数内部。所以，不需要在try之前显示定义变量。

### global

学习手册，P422，423，425

首先，global不是指应用全局，而是指模块作用域，或文件作用域，即在一个模块内的“全局”。

其次，在函数内，只有对全局变量 X 赋值时，才必须用global进行声明，否则 X 就是函数内的局部变量了。

全局变量 X 在函数运行前可能不存在。如果这样的话，函数内的赋值语句将自动在模块中创建全局变量 X。

在模块B中修改模块A的全局变量，尽量通过模块A的函数接口进行修改。

## 生成器函数

运行下面的例子可以看到，在生成器函数中：
* for之前的语句只执行一次，在第一次迭代时执行，即调用next时执行，而不是在调用gensquares时执行。
* for内部的语句，在每次迭代时都执行，无论是在yield前面还是后面。
* for后面的语句，在迭代结束时执行一次。如果迭代不结束，就不执行。

```python
#!/usr/bin/python

def gensquares(N):
    print "in gen"
    for i in range(N):
        print "before yield, in for"
        yield i ** 2
        print "in for, after yield", i, N
    print "outer for", i, N

for i in gensquares(5):
    print i

x = gensquares(10)
print next(x)
print x.send(77)
print x.send(88)
print x.send(99)
print next(x)
```


### 参考资料

http://stackoverflow.com/questions/710551/import-module-or-from-module-import
http://stackoverflow.com/questions/9439480/from-import-vs-import

## 内置函数

https://docs.python.org/2/library/functions.html

## 函数参数传递

调用函数时，使用`a = f(*tup)`的形式，tup是一个元组，`*tup`将元祖解包，将各个元素作为参数传递给函数f（学习手册，P456）。
`a = f(**d)`这种形式，d是一个字典，`**`将字典d解包，以关键字方式传递参数给函数f。（学习手册，P456）

定义函数时，使用`def f(*args): print(args)`的形式，在函数 f 中，args是一个元组，元组的每个元素，对应调用方传递的一个参数。（学习手册，P455）
`def f(**args): print(args)`这种形式，args是一个字典，`**`和`*`类似，但是`**`只对以关键字方式传递的参数有效。（学习手册，P455）

## 多个返回值

```python
a, b = b, a #swap
[a, b, c] = (1, 2, 3)
```
参考 Python学习手册 P294，序列赋值。

## numpy

`numpy`依赖的数值计算库：
* lapack
* blas
* atlas

检查线性代数库是否可用：
`ldd /usr/local/lib/python2.7/dist-packages/numpy/core/multiarray.so`
https://stackoverflow.com/questions/37184618/find-out-if-which-blas-library-is-used-by-numpy/37190672
https://stackoverflow.com/questions/9000164/how-to-check-blas-lapack-linkage-in-numpy-scipy

## ipython

安装：`sudo pip install ipython && sudo apt-get install ipython3`

运行`ipython`，CentOS7报错：
```
Traceback (most recent call last):
  File "/usr/bin/ipython", line 7, in <module>
    from IPython import start_ipython
  File "/usr/lib/python2.7/site-packages/IPython/__init__.py", line 49, in <module>
    from .terminal.embed import embed
  File "/usr/lib/python2.7/site-packages/IPython/terminal/embed.py", line 18, in <module>
    from IPython.terminal.interactiveshell import TerminalInteractiveShell
  File "/usr/lib/python2.7/site-packages/IPython/terminal/interactiveshell.py", line 94, in <module>
    class TerminalInteractiveShell(InteractiveShell):
  File "/usr/lib/python2.7/site-packages/IPython/terminal/interactiveshell.py", line 139, in TerminalInteractiveShell
    highlighting: \n %s""" % ', '.join(get_all_styles())
  File "/usr/lib64/python2.7/site-packages/pygments/styles/__init__.py", line 79, in get_all_styles
    for name, _ in find_plugin_styles():
  File "/usr/lib64/python2.7/site-packages/pygments/plugin.py", line 62, in find_plugin_styles
    for entrypoint in iter_entry_points(STYLE_ENTRY_POINT):
  File "/usr/lib64/python2.7/site-packages/pygments/plugin.py", line 45, in iter_entry_points
    import pkg_resources
  File "/usr/lib/python2.7/site-packages/pkg_resources/__init__.py", line 72, in <module>
    import packaging.requirements
  File "/usr/lib/python2.7/site-packages/packaging/requirements.py", line 59, in <module>
    MARKER_EXPR = originalTextFor(MARKER_EXPR())("marker")
TypeError: __call__() takes exactly 2 arguments (1 given)
```
[解决方案](http://stackoverflow.com/questions/42029545/pip-is-error-typeerror-call-takes-exactly-2-arguments-1-given)：
将`/usr/lib/python2.7/site-packages/packaging/requirements.py`中的`MARKER_EXPR = originalTextFor(MARKER_EXPR())("marker")`改为`MARKER_EXPR = originalTextFor(MARKER_EXPR)("marker")`。

### ipython的优点

[优点](https://itacey.gitbooks.io/learning_ipython/content/%E7%AC%AC%E4%B8%80%E7%AB%A0.html)：
* 可以运行部分Shell命令，tab补全
* 语法高亮
* 自动补全

其它参考资料：
http://mindonmind.github.io/2013/02/08/ipython-notebook-interactive-computing-new-era/

## jupyter

web版ipython

### 配置初始化

`jupyter notebook --generate-config`
初始化文件`~/.jupyter/jupyter_notebook_config.py`
https://jupyter-notebook.readthedocs.io/en/stable/public_server.html

### 设置口令

`jupyter notebook password`

### jupyter支持多用户的方案：

* 各个用户分配不同目录，口头约定
* 不同用户配置不同的端口、目录 https://blog.csdn.net/JJwho/article/details/75102045
* https://github.com/jupyterhub/jupyterhub

### jupyterhub

https://mohanamuraligurunathan.blogspot.com/2018/04/jupyterhub-offline-install-without.html

## limits

### 最小浮点正数

```python
import numpy as np
import sys
np.nextafter(0,1)
sys.float_info.min * sys.float_info.epsilon
sys.float_info.min * sys.float_info.epsilon == np.nextafter(0,1)
1.0/(sys.float_info.min*0.27)
```

## redis

python-redis可以自动重连（本地测试过）。
但在docker环境下，redis必须在应用之前启动、可用。否则需要重启应用。原因不明。

## mysql

Python中最常用的MySQL库是[MySQL-python](https://pypi.python.org/pypi/MySQL-python/)。CentOS7下的安装命令为`sudo yum install MySQL-python`。

http://mysql-python.sourceforge.net/MySQLdb.html
http://mysql-python.sourceforge.net/MySQLdb-1.2.2/
https://stackoverflow.com/questions/372885/how-do-i-connect-to-a-mysql-database-in-python
https://www.quora.com/Whats-the-best-MySQL-library-for-Python

貌似不支持PreparedStatement: https://stackoverflow.com/questions/1947750/does-python-support-mysql-prepared-statements

查询参数 https://stackoverflow.com/questions/775296/python-mysql-parameterized-queries

```python
import MySQLdb
db = MySQLdb.connect('host', 'user', 'pwd', 'database', connect_timeout=5, charset='utf8mb4')
cursor = db.cursor()
cursor.execute('select name from resume where name is not null limit 1')
row = cursor.fetchone()
# 查询参数
c.execute("SELECT * FROM foo WHERE bar = %s AND baz = %s", (param1, param2))
# 字典形式
c.execute("""SELECT * FROM records WHERE id = %(id)s""", {"id": 2})
# 但两种形式，都不支持None转为null

# fetch size || cursor
# https://stackoverflow.com/questions/337479/how-to-get-a-row-by-row-mysql-resultset-in-python
import MySQLdb.cursors
MySQLdb.connect(user="root", passwd="root", db="sxs", cursorclass = MySQLdb.cursors.SSCursor)
```

### mysql connector

https://dev.mysql.com/doc/connector-python/en/
https://dev.mysql.com/doc/connector-python/en/connector-python-example-connecting.html
https://dev.mysql.com/doc/connector-python/en/connector-python-connectargs.html
https://dev.mysql.com/doc/connector-python/en/connector-python-api-mysqlcursorprepared.html

注意事项：
* 创建连接时，如果不制定pool相关参数，不会使用连接池。
* 连接池会检查连接是否有效、并尝试一次重连。
* 如果不是连接池，需要自己检查连接是否有效。

## 线程

一般推荐使用threading库，thread库声明为deprecated。
http://www.python-course.eu/threads.php

http://www.runoob.com/python/python-multithreading.html
http://python.jobbole.com/81546/
https://pymotw.com/2/threading/
http://effbot.org/zone/thread-synchronization.htm
https://docs.python.org/2/library/threading.html
https://docs.python.org/2/library/thread.html
utf8mb4
https://dev.mysql.com/doc/connector-python/en/connector-python-api-cext-character-set-name.html
https://stackoverflow.com/questions/26532722/how-to-encode-utf8mb4-in-python

### gil

如果使用 CPython ，因为[gil](https://docs.python.org/2/glossary.html#term-global-interpreter-lock)的存在，同一时间只有一个物理线程执行，这时多线程比较适合 I/O 较多的场景，比如 Web 应用。

计算密集型任务如果想摆脱gil的限制，有以下几种方式：
* 使用[multiprocessing](https://docs.python.org/2/library/multiprocessing.html#module-multiprocessing)
* 使用JPython等实现。
* 使用C扩展处理计算瓶颈。
* 使用Lua计算。

### 线程安全

以下操作是线程安全的：
```python
d = {}
d['a'] = 1
l = []
l.sort()
```
https://docs.python.org/2/faq/library.html#what-kinds-of-global-value-mutation-are-thread-safe
https://blog.louie.lu/2017/05/19/深入-gil-如何寫出快速且-thread-safe-的-python-grok-the-gil-how-to-write-fast-and-thread-safe-python/
https://opensource.com/article/17/4/grok-gil

[google guide](https://google.github.io/styleguide/pyguide.html#Threading)建议**不要依赖内置类型操作的原子性。**

### 多线程I/O输出同步问题

简单的可以考虑[logging](https://docs.python.org/2/library/logging.html#thread-safety)

[multiprocessing的日志同步](https://stackoverflow.com/questions/641420/how-should-i-log-while-using-multiprocessing-in-python)

## date

date转utc timestamp
https://stackoverflow.com/questions/5067218/get-utc-timestamp-in-python-with-datetime
```python
import pytz
import time
mytz = pytz.timezone('Asia/Shanghai')
dt = mytz.normalize(mytz.localize(datetime.datetime.now(), is_dst=False))
int(time.mktime(dt.timetuple()))
```

## 调用 shell bash script

https://docs.python.org/3.5/library/subprocess.html
https://stackoverflow.com/questions/13745648/running-bash-script-from-within-python
https://stackoverflow.com/questions/26236126/how-to-run-bash-commands-inside-of-a-python-script
https://stackoverflow.com/questions/20415522/running-a-bash-script-from-python
https://stackoverflow.com/questions/3777301/how-to-call-a-shell-script-from-python-code

## 异步io

异步web/net框架
https://github.com/tornadoweb/tornado

## url

https://my.oschina.net/guol/blog/95699
https://stackoverflow.com/questions/2506379/add-params-to-given-url-in-python

## excel

### xlwt

`xlwt`只支持Excel 2003及之前的版本，不能写入超过65535行、256列的数据。

http://www.python-excel.org/
https://zhuanlan.zhihu.com/p/23998083
http://wenqiang-china.github.io/2016/05/13/python-opetating-excel/

## 第三方包管理

Python中管理第三方包有两种方式，`pip`和`easy_install`。

### pip

* upgrade: `sudo pip install --upgrade numpy`
* PIP_INDEX_URL=https://mirrors.aliyun.com/pypi/simple

### Anaconda

https://zh.wikipedia.org/wiki/Anaconda_(Python发行版)
https://www.jianshu.com/p/16df00d65ecd
http://devopspy.com/python/conda-vs-pip/
https://conda.io/docs/user-guide/concepts.html
术语 https://conda.io/docs/glossary.html

清华大学镜像：
```shell
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes
```

R:
```shell
conda install -c r r-essentials
```
https://zhuanlan.zhihu.com/p/25430471

## ipython

在ipython中运行python脚本文件：`run test.py`

## __future__

学习手册，P593
https://docs.python.org/2/library/__future__.html

## 其它

### setup.py vs requirements.txt

http://pyzh.readthedocs.io/en/latest/python-setup-dot-py-vs-requirements-dot-txt.html
https://caremad.io/posts/2013/07/setup-vs-requirement/
https://pip.pypa.io/en/stable/user_guide/#requirements-files
http://blog.csdn.net/orangleliu/article/details/60958525

### virtualenv

https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001432712108300322c61f256c74803b43bfd65c6f8d0d0000

pyenv,pipenv

```
pip install --upgrade virtualenv
python3 -m pip install --user --upgrade virtualenv

virtualenv -p python3 --no-site-packages envname
python3 -m virtualenv -p `which python3` env
virtualenv --no-site-packages project_name
source venv/bin/activate
deactivate
```

### UML

```
brew install graphviz # suport svg etc.
pip install pylint
cd phoenix
pyreverse -A -k -S -o svg strategies
```

### and or logic

`and/or`是逻辑操作，可以用于判断`True/False`。
但是，`and/or`表达式的返回值，并不一定是`True/False`，而是评估的表达式的值。
```python
'a' and 'b'
'' and 'a'
'' or 'a'
```

https://docs.python.org/2/reference/expressions.html#boolean-operations
https://blog.csdn.net/niuniuyuh/article/details/71213887
http://www.diveintopython.net/power_of_introspection/and_or.html

## http server

```shell
python -m SimpleHTTPServer 8080
python3 -m http.server 8080
```

## 问题

如何将异常信息打印到stderr？





## 参考资料

Python 学习手册
