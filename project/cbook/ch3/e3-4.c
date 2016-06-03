/* itoa 并能支持最大负整数 */

#include <limits.h>
#include <string.h>
#include <stdio.h>

char*
reverse (char *s) {
	int i, j;
	char c;

	for(i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
	return s;
}

char* 
itoa (int i, char *str) {
	int sign;
	char *s = str;
	sign = i;
	if(i < 0) {
		if(i == INT_MIN) { //如果是最小负数，乘-1会溢出。先加1再乘-1,最后字符再加1。 
			i = -(i + 1);
		}
		else {
			i = -i;
		}
	}
	for(*s = '0'; i != 0; i /= 10) {  //保存个位
		*s++ = i % 10 + '0';
	}
	if(sign == INT_MIN) {  //如果是最小负数，个位加1
		(*str)++;
	}
	if(sign < 0) {
		*s++ = '-';
	}
	*s = '\0';
	return reverse(str);
}

int 
main () {
	char s[100];
	printf("%s\n", itoa(3, s));
	printf("%s\n", itoa(-3,s));
	printf("%s\n", itoa(-1234,s));
	printf("%s\n", itoa(3943,s));
	printf("%s\n", itoa(INT_MIN, s));
	printf("%s\n", itoa(INT_MAX, s));
}
