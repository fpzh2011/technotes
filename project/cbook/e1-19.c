/*对每一行输入，倒序输出该行*/ 

#include <stdio.h>
#include <stdlib.h>

#include "e1-16f.h"

void 
reverse (char s[]) {
	//定位到字符串末尾
	int len = -1;
	while(s[++len] != '\0')
		;
	len--;
	
	int i, temp;
	for(i = 0; i <= len/2; i++) {
		temp = s[i];
		s[i] = s[len-i];
		s[len-i] = temp;
	}
}

void
main () {
	int buflen = 1;
	char* buf = malloc(buflen);
	int n;
	
	while ((n = getoneline(&buf, &buflen)) >= 0) {
		reverse(buf);
		printf("count is %d, the input is :%s\n", n, buf);
	}
	putchar('\n');
	free(buf);
}
