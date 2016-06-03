/*将空格替换为最少数量的tab和空格，并保持单词之间的间隔不变。*/ 

#include <stdio.h>
#include <stdlib.h>

#include "e1-16f.h"

//abcde   ghia        	skfks       akdf a
void
entab (char s[]) {
	const int tabspace = 8;	//tab所占字符数目
	int len = 0, space = 0, count = 0, i;
	char c;
	while((c = s[len++]) != '\0') {
		count++;
		if(c != ' ' && c != '\t') {
			for(i = 0; i < space; i++, count++) {
				//putchar(' ');
				putchar(space+'0');
			}
			putchar(c);
			space = 0;
		}
		else if (c == '\t') {
			putchar('\t');
			space = 0;
			count = count + tabspace - count % tabspace;
		}
		else if (c == ' ' && (count % tabspace) == 0) {
			putchar('\t');
			space = 0;
		}
		else {
			space++;
		}
	}
}

void
main () {
	int buflen = 1;
	char* buf = malloc(buflen);
	int n;
	
	while ((n = getoneline(&buf, &buflen)) >= 0) {
		entab(buf);
	}
	free(buf);
}
