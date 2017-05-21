# gcc基础说明

## 命令行选项

	-c 只编译或汇编，不链接。为每个源文件生成object文件。将源文件的.c或.s替换为.o作为输出文件名。
	-S 生成汇编代码
	-o 指定输出文件名（包括编译、链接等）。
	-masm=intel 生成intel指令的汇编代码（默认是AT&T指令）
	-m32 -m64 32/64位指令
	-I 指定头文件的搜索目录。这里指定的头文件优先于标准系统include目录。
	-E 只进行预处理，处理 #include、#define 等预处理指令，不编译。
	-L 指定链接库目录
	-static 静态链接。需要安装静态库。rhel7的dvd中好像没有静态库，可以用centos的glibc-static-2.17-78.el7.x86_64.rpm

对编译或链接后的文件，反汇编可以用`objdump -d a.out`

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







##参考资料

	[1] C程序设计语言（K&R第二版）,P75， ISNB：978-7-111-12806-9

