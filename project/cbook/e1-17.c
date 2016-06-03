/*打印长度大于80个字节的输入行*/ 

#include <stdio.h>
#include <stdlib.h>

#include "e1-16f.h"

void
main() {
	int buflen = 1;
	char* buf = malloc(buflen);
	int n;
	
	while ((n = getoneline(&buf, &buflen)) >= 0) {
		if(n > 80) {
			printf("count is %d, the input is :%s", n, buf);
		}
	}
	putchar('\n');
	free(buf);
}
