# gflags：C++命令行解析组件

https://github.com/gflags/gflags
https://gflags.github.io/gflags/
https://github.com/gflags/example
http://www.yeolar.com/note/2014/12/14/gflags/
http://www.cnblogs.com/dkblog/archive/2012/02/15/2352315.html
http://blog.csdn.net/irwin_chen/article/details/8798315

## Ubuntu安装

`sudo apt install libgflags2v5 libgflags-dev`

## 定义flag

第一个参数是flag名，比如 languages ；第二个参数是flag的默认值；第三个参数是flag的帮助信息。这些在`-help`时都会输出。

```cpp
DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_string(languages, "english,french,german",
	"comma-separated list of languages to offer in the 'lang' menu");

DECLARE_int32(port);
```

DEFINE 宏有3个参数：flag名字，默认值，help字符串。

访问 flag 全局变量时，需要在flag名称前增加前缀`FLAGS_`。

gflags的flag通过全局变量实现，全局变量可以分散在各个文件中，而不必集中在main函数所在的文件中。
**可以在 .cc 文件中DEFINE flag，在 .h 文件中DECLARE flag。**

## 使用flag

在命令行使用未定义的flag会在执行时失败。如果需要允许未定义的flag，可以使用 `--undefok` 来去掉报错。
`--` 可以用于结束flag。重复指定flag使用最后的一个。

## 支持的flag类型

DEFINE_bool: boolean
DEFINE_int32: 32-bit integer
DEFINE_int64: 64-bit integer
DEFINE_uint64: unsigned 64-bit integer
DEFINE_double: double
DEFINE_string: C++ string

## 示例

```cpp
#include <iostream>
#include <chrono>

#include <gflags/gflags.h>

DECLARE_bool(big_menu);

DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");

DEFINE_string(languages, "english,french,german",
	"comma-separated list of languages to offer in the 'lang' menu");

bool ValidateBigMenu(const char* flagname, bool value) {
	return ! value;
} 

int main(int argc, char* argv[]) {
	google::SetUsageMessage("this is a sample usage.\n");
	bool valid = google::RegisterFlagValidator(&FLAGS_big_menu, &ValidateBigMenu);
	google::ParseCommandLineFlags(&argc,&argv,true);
	std::cout << FLAGS_big_menu << std::endl;
	std::cout << FLAGS_languages << std::endl;
	return 0;
}
```



