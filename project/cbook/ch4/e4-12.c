/* 递归实现 itoa 。无法处理最大负数。*/

#include <stdio.h>
#include <stdlib.h>

char* 
itoa2 (int i) {
	if(i < 0) {
		putchar('-');
		i = -i;
	}
	if(i / 10 != 0) {
		itoa2(i/10);
	}
	putchar(i % 10 + '0');
}

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s testInteger\n", argv[0]);
		return 0;
	}
	int i = atoi(argv[1]);
	itoa2(i);
	putchar('\n');
}
