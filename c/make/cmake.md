# cmake

## 安装 install

下载地址:
* https://cmake.org/download/
* https://cmake.org/files/
* https://github.com/Kitware/CMake/releases

如果之前没有装过cmake:
```shell
./bootstrap
make
make install
```
如果是覆盖安装:
```shell
cmake .
make      
make install
```
https://cmake.org/install/

## 动态库

https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake

## 打印变量

```
message(STATUS "missing components: ${missingComponents}")
```
https://cmake.org/cmake/help/latest/command/message.html
https://blog.csdn.net/Zhanganliu/article/details/99850603

## demo

https://www.hahack.com/codes/cmake/
```cmake
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo1)

# 指定生成目标。第一个参数Demo是生成的可执行文件名，后面是源文件列表

add_executable(Demo main.cc MathFunctions.cc)

# 添加math子目录。目录下必须有CMakeLists.txt。例如，submodule方式引用的另一个repo，有自己的cmake配置。
# 注意，不是赋值给变量。
add_subdirectory(math)

# 查找当前目录下的所有源文件，并将名称保存到DIR_SRCS变量。这个变量名没有特殊含义，只要与add_executable保持一致即可
# 注意：这种模式，应该不需要目录下有CMakeLists.txt
aux_source_directory(. DIR_SRCS)
# 指定生成目标，输入是源文件目录
add_executable(Demo ${DIR_SRCS})

# 添加链接库
# 可执行文件main需要连接一个名为MathFunctions的链接库。这个链接库的指令，可以在子目录的cmake配置中
target_link_libraries(Demo MathFunctions)
```

子目录：
```cmake
aux_source_directory(. DIR_LIB_SRCS)

# 生成静态链接库。这个就是target_link_libraries用到的静态库
add_library (MathFunctions ${DIR_LIB_SRCS})
```

概要：
* `cmake PATH`会生成编译需要的Makefile等文件，输入是 PATH/CMakeLists.txt
* 命令不区分大小写
* 命令由命令名称、小括号和参数组成，参数之间使用空格进行间隔
* 符号`#`后面的内容是注释
* 如果有多个源文件目录，根目录和每个子目录都需要有一个CMakeLists.txt

## 变量

* CMAKE_INSTALL_PREFIX: 安装目录，一般默认是 /usr/local/bin

## 设置选项option

 cmake3 -DBUILD_TESTS=OFF ..

## 注意事项

https://www.zhihu.com/question/58949190/answer/173438323

## 参考资料

https://zh.wikipedia.org/wiki/CMake

VS Code与CMake
https://zhuanlan.zhihu.com/p/52874931




## 疑问

如果包含一个CMake的子目录，必须把该目录下的文件编译为静态库吗？

静态链接，是把整个静态库都拷贝到可执行文件吗，还是只拷贝需要的函数？
但是直接调用的函数可能引用其它库函数，需要分析调用关系。



