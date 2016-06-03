/*将tab字符替换为适当数目的空格。*/ 

#include <stdio.h>
#include <stdlib.h>

#include "e1-16f.h"

void
detab (char s[]) {
	const int tabspace = 8;	//tab所占字符数目
	int space, len = 0, count = 0;
	char c;
	while((c = s[len++]) != '\0') {
		if(c != '\t') {
			putchar(c);
			count++;
		}
		else {
			space = tabspace - count % tabspace;
			int j;
			for(j = 0; j < space; j++) {
				putchar(' ');
			}
			count += space;
		}
	}
}

void
main () {
	int buflen = 1;
	char* buf = malloc(buflen);
	int n;
	
	while ((n = getoneline(&buf, &buflen)) >= 0) {
		detab(buf);
	}
	free(buf);
}
