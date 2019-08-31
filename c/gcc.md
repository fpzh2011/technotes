# gcc基础说明

## 命令行选项

* `-c` 只编译或汇编，不链接。为每个源文件生成object文件。将源文件的.c或.s替换为.o作为输出文件名。
* `-S` 生成汇编代码
* `-o` 指定输出文件名（包括编译、链接等）。
* `-masm=intel` 生成intel指令的汇编代码（默认是AT&T指令）
* `-m32 -m64` 32/64位指令
* `-I` 指定头文件的搜索目录。这里指定的头文件优先于标准系统include目录。
* `-E` 只进行预处理，处理 #include、#define 等预处理指令，不编译。
* `-L` 指定链接库目录
* `-llibrary`或`-l library` 指定需要的链接库，如`-liconv`。用gcc链接静态库的时候，`-l`要放在xxx.c后面。
* `-static` 静态链接。需要安装静态库。rhel7的dvd中好像没有静态库，可以用centos的glibc-static-2.17-78.el7.x86_64.rpm
* `-shared` 编译源代码为动态链接库，如gcc -fPIC -shared test.c -o libtest.so
* `-fPIC` 用于编译阶段，告诉编译器产生与位置无关代码(Position-Independent Code),则产生的代码中，没有绝对地址，全部使用相对地址，故而代码可以被加载器加载到内存的任何位置，都可以正确的执行。这正是共享库所要求的，共享库被加载时，在内存的位置不是固定的。
* `-v` 输出verbose详细信息

对编译或链接后的文件，反汇编可以用`objdump -d a.out`

参考：
* `man gcc`

## C标准支持

参考 `man gcc` 的 -std 一节。在rhel7的gcc-4.8.2下，默认的C标准是gnu90，是ISO C90的变种。

该版本的gcc还没有完全直尺C99和C11。但gcc-5.3基本完整支持C99和C11。

	-std=c99
	-std=c89
	-ansi
	-std=c11

## include

### 默认搜索路径

`#include <stdio.h>` 只在系统目录查找头文件。

`#include "zjh.h"` 会先到包含源代码的目录查找头文件，如果没找到，再到系统目录查找头文件。一般用于用户定义的头文件。

系统路径如：/usr/include:/usr/local/include:/usr/lib/gcc-lib/i386-linux/2.95.2/include。不同版本的系统也不一样。

参考资料[1]。

### 命令行指定参数

`gcc -I /home/my/include test.c`

### 环境变量

gcc也会尝试搜索以下两个变量指定的目录：C_INCLUDE_PATH 和 CPLUS_INCLUDE_PATH

## 查看预处理结果

`gcc -v -E test.c -o test.i`

	-v 在命令行显示详细的预处理步骤
	-E 指定源文件
	-o 指定预处理结果文件

## 优化模式

gcc不像MSVC那样有"release mode" 和 "debug mode"。可以开启 `-g` 模式在目标文件中包含debug信息。

http://stackoverflow.com/questions/1534912/how-to-build-in-release-mode-with-optimizations-in-gcc
http://stackoverflow.com/questions/1079832/how-can-i-configure-my-makefile-for-debug-and-release-builds

### 编译优化选项

貌似默认是 `-O2`

http://stackoverflow.com/questions/11546075/is-optimisation-level-o3-dangerous-in-g?rq=1
https://gcc.gnu.org/onlinedocs/gcc-3.3.5/gcc/Optimize-Options.html
http://www.network-theory.co.uk/docs/gccintro/gccintro_49.html
https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
http://stackoverflow.com/questions/796162/g-compiler-flags-optimization-and-flags-for-making-a-static-library-c

## 反汇编、饭编译

objdump -d 或 objdump -D （反汇编全部内容）

## 编译GCC

下载 [GCC](ftp://mirrors.kernel.org/gnu/gcc/gcc-4.9.4/gcc-4.9.4.tar.bz2) 和 [签名文件](ftp://mirrors.kernel.org/gnu/gcc/gcc-4.9.4/gcc-4.9.4.tar.bz2.sig)

下载 [公钥文件](https://ftp.gnu.org/gnu/gnu-keyring.gpg) [参考资料](http://stackoverflow.com/questions/15331015/how-to-verify-downloaded-file-with-sig-file)

验证下载文件：`gpg --verify --keyring ./gnu-keyring.gpg gcc-4.9.4.tar.bz2.sig`

## 静态库

静态库用`ar`命令创建。
http://tldp.org/HOWTO/Program-Library-HOWTO/static-libraries.html

## ld链接

### ld

编译后的链接工具。

### LD_LIBRARY_PATH

* 指示给dynamic loader的动态链接库目录列表
* 程序加载执行时(`ld.so`)，系统会把`LD_LIBRARY_PATH`添加到默认动态链接库目录列表前面
* `ld`链接时，也会搜索`LD_LIBRARY_PATH`目录列表，放在`-L`指定目录的后面
`man 8 ld.so`
http://xahlee.info/UnixResource_dir/_/ldpath.html
http://osr507doc.sco.com/en/tools/ccs_linkedit_dynamic_dirsearch.html

### LD_RUN_PATH

链接截断linker用的动态链接库目录列表，搜索次序在系统默认路径之前。
http://osr507doc.sco.com/en/tools/ccs_linkedit_dynamic_dirsearch.html

### LD_PRELOAD

预先加载的共享库目录列表，搜索顺序在用户定义、系统默认等库搜索路径之前，可以覆盖其它库的函数。
`man 8 ld.so`

### rpath

elf或so文件中记录的动态链接库库搜索路径，链接时确定。
https://en.wikipedia.org/wiki/Rpath

### 目录优先级

https://stackoverflow.com/questions/7967848/use-rpath-but-not-runpath

### 库详细解读

Shared Libraries是在程序启动时加载的(?)；动态链接库不一定在程序启动时加载，而是在执行到库代码时加载。
http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html

## configure

`./configure --help`
https://www.cnblogs.com/taskiller/archive/2012/12/14/2817650.html

##参考资料

	[1] C程序设计语言（K&R第二版）,P75， ISNB：978-7-111-12806-9

