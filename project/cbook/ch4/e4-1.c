/* strrindex(s, t) 返回字符串t在s中最右边出现的位置。如果s中不包含t，则返回-1。*/

#include <stdio.h>
#include <string.h>

/* 测试用例
abcd abcd
abcd bcd
bcdaa bcd
abcd bc
*/

// 两个字符串都是从右向左扫描
int 
strrindex (char *s, char *t) {
	int slen = strlen(s);
	int tlen = strlen(t);
	int i, j, k;
	for(i = slen - 1; i >= tlen - 1; i--) {
		for(k = i, j = tlen - 1; j >= 0 && s[k] == t[j]; j--, k--) 
			;
		if(j < 0) {
			return i - tlen + 1; 
		}
	}
	return -1;
}

int 
main(int argc, char *argv[]) {
	if(argc < 3) {
		printf("Usage: %s string pattern\n", argv[0]);
		return 0;
	}
	printf("%d\n", strrindex(argv[1], argv[2]));
	return 0;
}
