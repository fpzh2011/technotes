/* 将 s1 中任何与字符串s2 中字符匹配的字符都删除 */

/*
测试用例
abcdaaaa aaa
abcd ff
abcdaaadaag aa
aaa aaa
aaf aa
faaf aa
aaaa aa
aa aaaaa
 */

#include <stdio.h>

void
squeeze (char *s1, char *s2) {
	if(s1 == NULL || s2 == NULL || *s2 == '\0') {
		return;
	}
	int i, j, k;
	i = j = 0;
	while(s1[j] != '\0') {
		for(k = 0; s2[k] != '\0' && s1[j+k] == s2[k]; k++)
			;
		if(s2[k] == '\0') {
			j += k;
		}
		else {
			s1[i++] = s1[j++];
		}
	}
	s1[i] = '\0';
}

void 
main (int argc, char* argv[]) {
	if(argc < 3) {
		fprintf(stderr, "Usage: squeeze str pattern\n");
		return;
	}

	squeeze(argv[1], argv[2]);
	printf("%s\n", argv[1]);
}
