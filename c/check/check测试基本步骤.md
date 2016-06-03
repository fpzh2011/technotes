# check测试基本步骤

## 需要的基础组件

Autoconf、Automake、Libtool、Check。

check项目文档 http://libcheck.github.io/check/doc/check_html/index.html

## 创建目录结构

project

projec/src

project/tests

	.
	|-- Makefile.am
	|-- README
	|-- configure.ac
	|-- src
	|   |-- Makefile.am
	|   |-- main.c
	|   |-- money.c
	|   `-- money.h
	`-- tests
		|-- Makefile.am
		`-- check_money.c

## 安装包

rehl7下，check包只有动态链接库，没有.m4/.h等文件。需要安装check-devel和check包。

## 创建基本的文件

项目根目录下的`Makefile.am`只有简单的一行：`SUBDIRS = src . tests` 。注意tests要放到最后。

将 check_demo.tar.gz 解压到项目目录。并作如下修改：

configure.ac

	AC_PREREQ 修改autoconf的版本，可选。
	AC_INIT 修改包的名字、版本、bug反馈邮件。
	AC_CONFIG_SRCDIR 逐个列出 .c 文件。
	Checks for libraries. 列出需要额外列出的库文件。通过ldd可以看到，pthread和rt库是check需要的；math库是money demo需要的。
	AC_CONFIG_FILES 检验一下，一般不用修改。

src/Makefile.am

	lib_LTLIBRARIES 修改libtool库文件名。
	_SOURCES 修改源文件和头文件名。

tests/Makefile.am

	TESTS 修改TESTS的名字。
	check_PROGRAMS 修改check的名字。
	_SOURCES 修改_SOURCES变量名，以及 .c 源文件和 .h 头文件。
	_LDADD 修改_LDADD变量名，以及 .la 库文件名字。

file命令显示，“.la”文件是libtool库文件。

### 测试命令

	autoreconf --install
	./configure CC="gcc -Wpedantic -Wall -std=c11" CFLAGS=-g 
	make
	make check

### 避免大量中间文件

在源目录中可以只保留configure.ac和Makefile.am文件。
测试时，运行check这个脚本，将目录拷贝到/tmp临时目录，在/tmp下进行测试。

这样的缺点是，如果项目比较大，每次需要拷贝大量文件，每次都需要从头到尾进行构建，可能比较耗时。

## 测试static函数

http://stackoverflow.com/questions/593414/how-to-test-a-static-function



源文件被构建为一个库，main.c 仅仅作为库的客户端，这一点可以从Makefile.am中看到。

autoreconf --install
libtoolize: Consider adding `AC_CONFIG_MACRO_DIR([m4])' to configure.ac and
libtoolize: rerunning libtoolize, to keep the correct libtool macros in-tree.
libtoolize: Consider adding `-I m4' to ACLOCAL_AMFLAGS in Makefile.am.

