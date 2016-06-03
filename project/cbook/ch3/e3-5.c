/* itob(n, s, b)  将n转换为以b为底的数。并将转换结果以字符串形式保存到s。
 * 如itob(n, s, 16) 转换为16进制数。
 * 支持最大负整数 */

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
itoa (int i, char *str, int b) {
	int sign, m;
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
	for(*s = '0'; i != 0; i /= b) {  //保存个位
		m = i % b;
		if(m < 10) {
			*s++ = m + '0';
		}
		else {
			*s++ = m + 'a' - 10;
		}
	}
	if(sign == INT_MIN) {  //如果是最小负数，个位加1，并逐位进位
		char *ps, maxc;
		if(b <= 10) {
			maxc = '0' + b - 1;
		}
		else {
			maxc = 'a' + b - 11; 
		}
		for(ps = str; *ps == maxc && ps < s; ps++) {
			*ps = '0';
		}
		if(ps == s) {
			*s++ = '1';
		}
		else {
			(*ps)++;
		}
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
	printf("%s\n", itoa(3, s, 16));
	printf("%s\n", itoa(-3,s, 16));
	printf("%s\n", itoa(-1234,s, 16));
	printf("%s\n", itoa(3943,s, 16));
	printf("int_min: %d %x\n", INT_MIN, INT_MIN);
	printf("%s\n", itoa(INT_MIN, s, 16));
	printf("%s\n", itoa(INT_MAX, s, 16));
	printf("%s\n", itoa(INT_MIN, s, 10));
}
