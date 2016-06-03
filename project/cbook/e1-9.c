#include <stdio.h>

/*将输入复制到输出，并将多个连续空格合并为一个空格*/ 
void main() {
	int c;
	int skip = 0; // 当前（前一个字符）是否是空格

	while((c = getchar()) != EOF) {
		if (c != ' ') {
			putchar(c);
			skip = 0;
		}
		else if(skip == 0) {
			putchar(c);
			skip = 1;
		}

	}
}
