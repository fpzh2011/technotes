# autoconf、automake手册摘要

## autoconf

### CH1 

autoconf最主要的目标，不是自动生成配置脚本，而是使配置脚本无痛、可移植、可预期。
对autoconf最大的抱怨是input，而不是运行。

autoconf、automake、libtools等的目标是构建可移植的软件包。

### CH2

2.1 autoconf解决的重要问题，是系统相关的build和runtime发现。
Makefile.am

### 3.1.3 Standard ‘configure.ac’ Layout

标准的configure.ac文件布局。

	Autoconf requirements
	AC_INIT(package, version, bug-report-address )
	information on the package
	checks for programs
	checks for libraries
	checks for header files
	checks for types
	checks for structures
	checks for compiler characteristics
	checks for library functions
	checks for system services
	AC_CONFIG_FILES([file...])
	AC_OUTPUT

### CH4

init autoscan等输入什么、输出什么

### CH5 

各种检测如何做

### 16.2 Compilers and Options

如果要指定编译选项，可以通过 `./configure CC="gcc -Wpedantic" CFLAGS=-g LIBS=-lposix` 实现。
或者参考 `AM_CFLAGS CFLAGS`。

## automake

### 1 Introduction

`automake`一般与`autoconf`配合使用。完成`autoconf`的文件编辑后，手工编辑`Makefile.am`文件。
`automake`读取`Makefile.am`并输出`Makefile.in`。

`Makefile.am`中基本都是一系列`make`变量定义。这里的变量也就是make macros。

### 2 An Introduction to the Autotools

安装`automake`后的示例：/usr/local/share/doc/automake/amhello-1.0.tar.gz。但Ubuntu下似乎没有这个文件。

### 2.2.2 Standard Makefile Targets

如 install, clean...

### 2.2.6 Parallel Build Trees (a.k.a. VPATH Builds)

The GNU Build System distinguishes two trees: the source tree, and the build tree.
source tree是包含configure文件的目录。build tree是运行configure的目录。
也就是说，我们可以在一个目录下保存源文件、configure等文件，在另一个目录下执行xx/.../yyy/configure。

### 2.4.2 amhello’s configure.ac Setup Explained

configure.ac中，AM_开头的宏是automake宏。
`AM_INIT_AUTOMAKE([-Wall -Werror foreign])`，-Wall and -Werror ask automake to turn on all warnings and report them as errors.
`foreign`选项让automake不必严格遵循GNU标准。

### 2.4.3 amhello’s Makefile.am Setup Explained

Makefile.am中以`_PROGRAMS`结尾的变量列出要build的程序，也称为`primary`。可以是可执行程序、静态库、动态库等。
其他的`primary`如_SCRIPTS, _DATA, _LIBRARIES。

`bin_PROGRAMS`中的`bin`告诉automake，生成的程序放到bindir变量指向的目录，一般是prefix/bin。

_PROGRAMS列表中列出的每个项如prog，automake自动寻找列出相应源文件的另一个变量prog_SOURCES。
如果_SOURCES没有被定义，默认的源文件是prog.c（CH8.1.1）。

以##开头的行是automake的注释。

### 5 Creating a Makefile.in

automake假定只在项目的根目录有一个configure.ac文件。automake也必须在项目的根目录运行。

### 8.1.1 Defining program sources

多个程序可以在一个目录中构建，也可以共享源文件。

### 8.5 Default _SOURCES

下面的例子构建3个程序，源文件扩展名是.cpp（默认是.c）。

	check_PROGRAMS = test1 test2 test3
	AM_DEFAULT_SOURCE_EXT = .cpp

### 常用变量

`SUBDIRS = src . tests` 指出make在处理当前目录前，需要递归处理的子目录，常用于项目的根目录。
在上述例子中，make处理的顺序是：src子目录、当前目录、tests子目录。
（CH2.4.3，P25；CH7.1）

## 参考资料

	GNU Automake (manual, version 1.15, 31 December 2014)
	https://www.lrde.epita.fr/~adl/autotools.html
	https://www.sourceware.org/autobook/


