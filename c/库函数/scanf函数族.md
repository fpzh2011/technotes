# scanf函数族

## 测试环境

rhel7，gcc 4.8.2

## scanf

如果scanf只有部分匹配成功，比如`"%d %f %d"`，只有前两个`"%d %f"`匹配成功，那么下一次调用scanf函数将从上一次转换的最后一个字符的下一个字符开始继续搜索。

如下面的代码，输入`1234 456 ttt 222 333 444 555 666 777 888`，输出中，`the rest is: 333`。

	#include <stdio.h>

	int 
	main () {
		int day, month, year, i = -1, j = -1;
		char monthname[20];
		char line[100];
		char testbuf[100];
		testbuf[0] = '\0';

		if((j = scanf("%d/%d/%d", &year, &month, &day)) == 3) {
			printf("pattern2: %d-%d-%d\n", year, month, day);
		}
		else if((i = scanf("%d %s %d", &year, monthname, &day)) == 3) {
			printf("pattern1: %d-%s-%d\n", year, monthname, day);
		}
		
		scanf("%s", testbuf);
		printf("i = %d, j = %d, the rest is: %s\n", i, j, testbuf);
	
		return 0;
	}

### 输入与格式串不匹配的情况

如果scanf的fmt格式串中要求整数，但输入字符串，则不读入数据，直接返回。

下面的代码编译后运行，输入abc123，i没有被赋值，scanf返回0；输入123abc可以给i赋值123。

	#include <stdio.h>
	
	int 
	main () {
		int i = 0, j;
		while(1 == (j = scanf("%d", &i)) ) {
			printf("i = %d, j = %d\n", i, j);
		}
		printf("end: i = %d, j = %d\n", i, j);
	}

## sscanf

C99标准中说，sscanf和fscanf是一致的。但是在部分匹配成功的情况下，存在一些细微差异。

如果部分匹配成功，下一次调用sscanf函数时，仍将从字符串的第一个字符开始继续搜索，就像没有调用过sscanf一样。

如下面的代码，输入`1234 456 ttt 222 333 444 555 666 777 888`，输出中，`the rest is: 1234`。

	#include <stdio.h>

	int 
	main () {
		int day, month, year, i = -1, j = -1;
		char monthname[20];
		char line[100];
		char testbuf[100];
		testbuf[0] = '\0';

		while(fgets(line, 100, stdin) != NULL) {
			if((i = sscanf(line, "%d %s %d", &year,monthname, &day)) == 3) {
				printf("pattern1: %d %s %d\n", year, monthname, day);
			}
			else if((j = sscanf(line, "%d/%d/%d", &year, &month, &day)) == 3) {
				printf("pattern2: %d %d %d\n", year, month, day);
			}
			sscanf(line, "%s", testbuf);
			printf("invalid, i = %d, j = %d, the rest is: %s\n", i, j, testbuf);
		}
		return 0;
	}

因为fscanf/scanf面向的是IO，已经读入并赋值了，就不适合再退回；而sscanf面向的是字符串数组，函数只认字符串的开始地址，而不会考虑IO问题。

### 参考资料

	[1] C99，fscanf函数说明的第6项，例18。
	[2] C程序设计语言（K&R第二版，P138/140） ISNB：978-7-111-12806-9

## %c vs %s

%s可以读入字符串。%10s可以指定读入的最大长度为10，如果遇到空白就提前结束。

如果要读入指定个数的、含空白的字符，可以用%c或%4c。

## 在字符串中引用宏定义的常量

scanf 用 %s 读入字符串时，可能碰到这种情况：我们用宏定义了缓冲区长度LEN、并利用LEN定义了一个缓冲区 char buf[LEN]，我们希望在scanf的格式串中也能用上宏定义，而不是人工再手写一个长度。

初步的思路是使用预处理的“#”符号。因为“#”符号只适用于带参数的宏，所以我们也要定义一个带参数的宏。

像下面代码那样直接通过常量宏和“#”符号，无法得到想要的结果。

	#define LEN 6
	#define S(x) #x

	int 
	main () {
		char *s = S(LEN);
	}

运行 `gcc -E test.c > test.i; cat test.i` 得到如下的结果。LEN没有被替换为6，而是直接放入字符串。

	int
	main () {
	 char *s = "LEN";
	}

通过下面的宏定义，就可以实现将宏定义的常量值放到字符串中。

核心是，在这个场景下，不能让带“#”符号的宏直接出现在代码中，这样会直接把形参放到字符串中。应该通过一种间接的方式引用带“#”符号的宏。

预处理时，代码中的宏NUM_TO_S看上去是一个普通宏，它的参数也是一个宏LEN，所以先将LEN替换为6，再去解析NUM_TO_S(6)。

	#define LEN 6
	#define TO_STRING(x) #x
	#define NUM_TO_S(x) TO_STRING(x)

	int 
	main () {
		char *s = NUM_TO_S(LEN);
	}

预处理后：

	int
	main () {
	 char *s = "6";
	}

### 参考资料

	https://gcc.gnu.org/onlinedocs/cpp/Stringification.html
	http://stackoverflow.com/questions/17314796/max-string-length-using-scanf-ansi-c
	http://stackoverflow.com/questions/2240120/glibc-glib-and-gnulib
	http://c-faq.com/stdio/scanfvwid.html

