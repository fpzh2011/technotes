# getchar、fgets、getline等输入函数

getchar、fgets、getline都在stdio.h中声明。但是getline应该不属于标准C。

## getchar

### 基本逻辑

getchar() 只是简单的从标准输入读取一个字节（不是字符），没有根据本地字符集进行国际化字符处理。
参考下面代码：

	#include <stdio.h>

	/*
	 * 测试getchar如何读取汉字
	 * usage: echo 中国 | testgetchar
	 * */
	void main() {
		int c ;
		while((c= getchar()) != EOF) {
			printf("%x\n", c);		
		}
	}

`echo 中国 | testgetchar` 输出为（多行合并为一行）：e4 b8 ad e5 9b bd a （utf-8每个字是3字节）。

### 实现与性能

getchar 即 getc(stdin) ，一般被实现为宏，所以执行速度较快。

## fgets

char *gets(char *s) 存在缓冲区溢出隐患，应该禁止使用。蠕虫病毒就是利用这一漏洞传播的。

char *fgets(char *s, int size, FILE *stream) 。由用户传入一个缓冲区指针和缓冲区大小。fgets在读入(size-1)个字符、或遇到换行符、或文件结束后返回s，如果出错返回NULL。

使用fgets需要自己处理超长行。

## getline

getline和getdelim不属于C标准，是glibc的扩展实现。

当`(*lineptr)`为NULL时，由getline申请内存，并由调用者管理`(*lineptr)`指向的内存的释放。

如果调用者分配的缓冲区长度`(*n)`不够，getline调用realloc重新申请内存，并更新`(*lineptr, *n)`

getdelim可以指定行分隔符。

出错时，两个函数都返回 -1 。

	#include <stdio.h>
	ssize_t getline(char **lineptr, size_t *n, FILE *stream);
	ssize_t getdelim(char **lineptr, size_t *n, int delim, FILE *stream);


## 参考资料

	man 3 fgets
	man 3 getline
	C专家编程，P42，Ch2.3.3

