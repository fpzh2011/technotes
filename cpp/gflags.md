# gflags：C++命令行解析组件

https://github.com/gflags/gflags
https://gflags.github.io/gflags/
https://github.com/gflags/example
http://www.cnblogs.com/dkblog/archive/2012/02/15/2352315.html

## Ubuntu安装

`sudo apt install libgflags2v5 libgflags-dev`

## 定义flag

第一个参数是flag名，比如 languages ；第二个参数是flag的默认值；第三个参数是flag的帮助信息。这些在`-help`时都会输出。

```cpp
DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_string(languages, "english,french,german",
	"comma-separated list of languages to offer in the 'lang' menu");
```

**可以在 .cc 文件中DEFINE flag，在 .h 文件中DECLARE flag。**

## 支持的flag类型

DEFINE_bool: boolean
DEFINE_int32: 32-bit integer
DEFINE_int64: 64-bit integer
DEFINE_uint64: unsigned 64-bit integer
DEFINE_double: double
DEFINE_string: C++ string





