/* 宏定义swap */

#include <stdio.h>

#define swap(t, x, y) { \
	(t) = (x); \
	(x) = (y); \
	(y) = (t); \
}

int 
main () {
	int x = 1, y = 2, t;
	printf("x=%d, y=%d\n", x, y);
	swap(t, x, y);
	printf("x=%d, y=%d\n", x, y);
	return 0;
}
