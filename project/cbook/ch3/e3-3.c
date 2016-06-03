/* 将a-c-z，0-9，A-Z等速记形式展开为完整的字符列表 */

/*
测试用例
a
abc
a-z
---a-z
!-ba-c
b-!a-cdd
a-c-e
A-C-E
0-3-5
0-z
a-Z
b-a

while(*s != 0)
	c = *s
	s++
	putchar(c)
	if c是字母数字 
		while s[0] == '-' && sameset(c, s[1]) && c <= s[1]
			输出 c-s[1]
			c = s[1]
			s += 2
 */
#include <stdio.h>
#include <ctype.h>

int 
sameset(char c1, char c2) {
	return islower(c1) && islower(c2) || isupper(c1) && isupper(c2) || isdigit(c1) && isdigit(c2) ;
}

void 
expand (char *s) {
	char c;
	while((c = *s++) != '\0') {
		putchar(c);
		if(isalnum(c)) {
			while(*s == '-' && c <= s[1] && sameset(c,s[1]) ) {
				for(char temp = c+1; temp <= s[1]; temp++) {
					putchar(temp);
				}
				c = s[1];
				s += 2;
			}
		}
	}
}

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: expand thisisatesta-z\n");
		return 0;
	}
	printf("%s\n", argv[1]);
	expand(argv[1]);
	putchar('\n');
}
