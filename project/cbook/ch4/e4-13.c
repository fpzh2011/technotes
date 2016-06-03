/* 递归实现字符串 reverse 函数 */

#include <stdio.h>

/*直接输出*/ 
void 
reverse2 (char s[]) {
	if(*s == '\0') {
		return;
	}
	reverse2(s + 1);
	putchar(*s);
}

int  
main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s string\n", argv[0]);
		return 0;
	}
	reverse2(argv[1]);
	putchar('\n');
	return 0;
}
