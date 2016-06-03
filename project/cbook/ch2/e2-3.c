/*把由16进制数字组成的字符串（包含可选的前缀0x或0X）转换为与之等价的整数值。
 * 字符串中允许的值包括：0~9, a~f, A~F。
 */

#include <stdio.h>
#include <stdlib.h>
#include "../e1-16f.h"

int
htoi (char* s) {
	if(*s == '0' && (s[1] == 'x' || s[1] == 'X')) {
		s = s + 2;
	}
	char c;
	int sum = 0, i;
	for(; (c = *s) != '\0' && c != '\n'; s++) {
		if(c >= '0' && c <= '9') {
			i = c - '0';
		}
		else if (c >= 'a' && c <= 'f') {
			i = c - 'a' + 10;
		}
		else if (c >= 'A' && c <= 'F') {
			i = c - 'A' + 10;
		}
		else {
			i = 0;
		}
		sum = sum * 16 + i;
	}
	return sum;
}

void 
main () {
	int buflen = 1;
	char* buf = malloc(buflen);
	int n;
	
	while ((n = getoneline(&buf, &buflen)) >= 0) {
		int i = htoi(buf);
		printf("num by 10 is %d, num by 16 is :%x\n", i, i);
	}
	putchar('\n');
	free(buf);
}
