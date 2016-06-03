/*删除注释。因为要考虑'"'，所以要考虑'，从而要考虑'\''，从而要考虑\ */ 
#include <stdio.h>
#include <stdlib.h>

#include "e1-16f.h"

/*
测试用例
aaaa
aaa / * \  
aaaa//
aaaa // ljlafiek akf ak
aa "afja "
 */

int
isspecial(char c) {
	if(c == '\'' || c == '"' || c == '/') {
		return 1;
	}
	else {
		return 0;
	}
}

void
delcomment() {
	int buflen = 1;
	char* buf = malloc(buflen);
	
	enum {NORMAL, STRING, CH, BLOCKCOMMENT, LINECOMMENT} state;
	state = NORMAL;
	while (getoneline(&buf, &buflen) >= 0) {
		char *line = buf, c;
		while((c = *line) != '\0') {
			if(state == NORMAL && ! isspecial(c)) {
				putchar(c);
				line++;
			}
			if ((c = *line) != '\0' && state == NORMAL && isspecial(c)) { //有可能是blockcomment，所以normal的条件不能少
				if(c == '"') {
					state = STRING;
					line++;
				}
				else if(c == '\'') {
					state = CH;
					line++;
				}
				else if(c == '/' && line[1] == '/') { //linecomment
					putchar('\n');
					break;
				}
				else if(c == '/' && line[1] == '*') {
					state = BLOCKCOMMENT;
					line = line + 2;
				}
				else {
					putchar(c);
					line++;
				}
			}
			if(state != NORMAL && state != LINECOMMENT && *line != '\0') {
				switch (state) {
					case STRING: case CH:
						putchar(c = *(line - 1)); //c为'或"
						for(; *line != c && *line != '\0' || *line == c && *(line-1) == '\\'; line++) {
							putchar(*line);
						}
						state = NORMAL;
						break;
					case BLOCKCOMMENT:
						for(; *line != '\0' && ! (*line == '*' && *(line+1) == '/'); line++)
							;
						if(*line != '\0') { //没有到字符串末尾，肯定是找到了注释的末尾
							state = NORMAL;
							line = line + 2;
						}
						break;
				}
			}
		}
		printf(">>>>%d\n", state);
	}
	free(buf);
}

void
main () {
	delcomment();
}
