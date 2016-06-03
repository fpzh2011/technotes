# automake基础说明

## 基本步骤

### 创建c文件

vi helloworld.c

	#include <stdio.h>
	
	int main(int argc, char** argv){
		printf("%s", "Hello, Linux World!\n");
		return 0;
	}

### autoscan

运行 `autoscan` 来根据目录下的源代码生成一个configure.scan的模板文件。重命名文件：`mv configure.scan configure.ac`（旧版本autoconf文件名为configure.in）。
修改文件configure.ac，只保留如下内容：

	AC_INIT(helloworld.c)
	AM_INIT_AUTOMAKE(helloworld, 1.0)

	AC_PROG_CC
	AC_OUTPUT(Makefile)
	
新版autoconf推荐的configure.ac使用下面的形式，AC_INIT中带版本号，AM_INIT_AUTOMAKE宏不带任何参数。
参见 http://www.gnu.org/software/automake/manual/automake.html#Modernize-AM_005fINIT_005fAUTOMAKE-invocation

	AC_INIT(helloworld, 1.0, zhengjh@sinosure.com.cn)
	AC_CONFIG_SRCDIR([helloworld.c])
	AM_INIT_AUTOMAKE
	
	AC_PROG_CC
	AC_OUTPUT(Makefile)

confiugre.ac调用一系列autoconf宏来测试程序需要的或用到的特性是否存在，以及这些特性的功能。主要宏的说明如下：

	AC_PREREQ	声明autoconf要求的版本号
	AC_INIT	定义软件名称、版本号、联系方式
	AM_INIT_AUTOMAKE	必须要的，参数为软件名称和版本号
	AC_CONFIG_SCRDIR	用来侦测所指定的源码文件是否存在, 来确定源码目录的有效性
	AC_CONFIG_HEADER	用于生成config.h文件，以便autoheader命令使用
	AC_PROG_CC	指定编译器，默认GCC
	AC_CONFIG_FILES	生成相应的Makefile文件，不同文件夹下的Makefile通过空格分隔。例如：AC_CONFIG_FILES([Makefile, src/Makefile])
	AC_OUTPUT	用来设定configure所要产生的文件，如果是Makefile，configure会把它检查出来的结果带入Makefile.in文件产生合适的Makefile

### 执行命令aclocal和autoconf

执行 `aclocal` 生成文件aclocal.m4 和 autom4te.cache。

执行 `autoconf` 生成文件configure。

### 新建Makefile.am

`vi Makefile.am`（每行不能以tab等空白开始）


	AUTOMAKE_OPTIONS=foreign
	bin_PROGRAMS=helloworld
	helloworld_SOURCES=helloworld.c

Makefile.am是一种比Makefile更高层次的规则。只需指定要生成什么目标，它由什么源文件生成，要安装到什么目录等构成。

automake会根据我们写的Makefile.am来自动生成Makefile.in。Makefile.am中定义的宏和目标会指导automake生成指定的代码。

常见的文件编译类型有下面几种：

	PROGRAMS 表示可执行文件
	LIBRARIES 表示库文件
	LTLIBRARIES 这也是表示库文件，前面的LT表示libtool
	HEADERS 头文件
	DATA 数据文件，不能执行。

### 运行automake

运行 `automake --add-missing`，**automake会根据你写的Makefile.am来自动生成Makefile.in**。

Makefile.am可以看看zabbix的例子。

目前automake支持三种目录层次：flat、shallow和deep。

1) flat指的是所有文件都位于同一个目录中。就是所有源文件、头文件以及其他库文件都位于当前目录中，且没有子目录。Termutils就是这一类。

2) shallow指的是主要的源代码都储存在顶层目录，其他各个部分则储存在子目录中。
就是主要源文件在当前目录中，而其它一些实现各部分功能的源文件位于各自不同的目录。automake本身就是这一类。

3) deep指的是所有源代码都被储存在子目录中；顶层目录主要包含配置信息。
就是所有源文件及自己写的头文件位于当前目录的一个子目录中，而当前目录里没有任何源文件。 GNU cpio和GNU tar就是这一类。

### 执行configure生成Makefile

运行 `./configure` 生成文件**Makefile**。

### 使用Makefile编译代码

运行 `make` 编译代码，生成`helloworld`可执行文件。

## 参考资料

	http://www.jellythink.com/archives/1056
	http://www.laruence.com/2009/11/18/1154.html
	https://www.ibm.com/developerworks/cn/linux/l-makefile/
	http://coolshell.cn/articles/3790.html

