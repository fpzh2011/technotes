/*删除每个输入行末尾的空格及制表符，并删除完全是空白的行*/ 

#include <stdio.h>
#include <stdlib.h>

#include "e1-16f.h"

int 
rtrim (char s[]) {
	//定位到字符串末尾
	int i = -1;
	while(s[++i] != '\0')
		;
	//清除末尾的空白
	int c;
	while((c=s[--i]) == ' ' || c == '\t' || c == '\n')
		;
	s[i+1] = '\0';
	return i;
}

void
main () {
	int buflen = 1;
	char* buf = malloc(buflen);
	int n;
	
	while ((n = getoneline(&buf, &buflen)) >= 0) {
		int i = rtrim(buf);
		if(i >= 0) {
			printf("count is %d, the input is :%s\n", i, buf);
		}
	}
	putchar('\n');
	free(buf);
}
