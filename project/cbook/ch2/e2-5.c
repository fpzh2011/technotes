/* 将字符串s2中的任一字符在字符串s1中第一次出现的位置作为结果返回。如果s1中不包含s2中的字符，返回-1。*/

#include <stdio.h>

int 
any (const char* s1, const char* s2) {
	if(s1 == NULL || s2 == NULL || *s2 == '\0') {
		return -1;
	}
	int i;
	char c;
	const char *p;
	for(i = 0; (c = s1[i]) != '\0'; i++) {
		for(p = s2; *p != '\0' && c != *p; p++) 
			;
		if(*p != '\0') {
			break;
		}
	}
	if(*p == '\0') {
		return -1;
	}
	return i;
}

void 
main (int argc, char *argv[]) {
	if(argc < 3) {
		fprintf(stderr, "Usage: any string pattern\n");
	}
	printf("%d\n", any(argv[1], argv[2]));
}
