# C++日志组件

## g3log

不支持glog的 PLOG 类操作，输出errno信息。

https://github.com/KjellKod/g3log
http://www.open-open.com/lib/view/open1408677978459.html
http://www.codeproject.com/Articles/288827/g2log-An-efficient-asynchronous-logger-using-Cplus
https://bitbucket.org/KjellKod/g2log














## google log 框架

glog是google的 c++ 日志框架。

glog的 PLOG 支持输出 errno信息。

https://github.com/google/glog
https://www.cppfans.org/1566.html
http://www.itnose.net/detail/6076924.html
http://www.cnblogs.com/tianyajuanke/archive/2013/02/22/2921850.html
http://www.cnblogs.com/davidyang2415/p/3861109.html
http://blog.csdn.net/u010443572/article/details/43876095
http://gmd20.blog.163.com/blog/static/168439232015022112242552/


### 安装

git clone https://github.com/google/glog.git

./configure && make && sudo make install

### 使用

```cpp
#include <glog/logging.h>

int main(int argc, char* argv[]) {
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);
	LOG(INFO) << "Found " << 15 << " cookies";
}
```

运行 `g++ -c test.cc & g++ -o test -lglog -pthread test.o /usr/local/lib/libglog.so & ./test`

如果没有特别指定，日志文件保存在 `/tmp/<program name>.<hostname>.<user name>.log.<severity level>.<date>.<time>.<pid>`。
同时还会创建一个软链接，如`/tmp/test.INFO`。

### 日志信息级别

INFO, WARNING, ERROR, and FATAL.

默认情况下，ERROR和FATAL在输出到日志文件的同时，也输出到stderr。

### debug模式

？？？？？？？？
默认情况下，glog会输出debug信息。使用 `#define NDEBUG` 可以禁止输出debug信息。

### 类似perror

PLOG() and PLOG_IF() and PCHECK() 在输出日志的同时，输出errno的状态。




