/*
给定两个字符串p和s，将s中的p删除。
每次删除从s的开始位置扫描p，如果发现p，则删除，并再次从s的开始位置扫描，知道没有p被发现。忽略大小写差异。
示例：
p	s	结果
"string strSTRINGing" -> ""
"ccccstring ssssssssss" -> "cccc ssssssssss"
"ccccstrinstringgsssssssssss" -> "ccccsssssssssss"
"ccccstring stringssssssssss" -> "cccc ssssssssss"

This program takes as input two strings. The first of which is the search
string and the second is the buffer to search. The object of the program is to
remove all traces of the search string in the search buffer. The program will
perform the search starting at the beginning of the buffer and continue until
either the end of the buffer is reached or the search string is found. If the
search string is found, the string should be removed, and the search should
restart again from the beginning of the buffer. This process should repeat
until a full search through the buffer produces no more matches.

Requirements :

1. the search should be completely case-insensitive
(ie "StRinG" should remove the phrase "sTrINg")
2. it should be a multi-pass filter
(ie "string" should remove the phrase "strSTRINGing"

Here is the method signature :

public String strip(String str, String buf);

We will check to make sure the input to this problem is valid.

Here are some examples assuming the search string is "StRinG" :

"ccccstring ssssssssss" -> "cccc ssssssssss"
"ccccstrinstringgsssssssssss" -> "ccccsssssssssss"
"ccccstring stringssssssssss" -> "cccc ssssssssss"
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
字符串ps是s的一个子串。将ps开始的tlen个字符从s中删除。slen是s的长度。 
 */
static void 
delete (char *s, char *ps, size_t slen, size_t tlen) {
	memmove(ps - tlen, ps, s + slen -ps + 1);
}

/*
如果s不包含p，返回NULL；否则，返回第一个p的末尾'\0'的下一个位置。
 */
static char* 
contain (char *s, char *p) {
	char *ps, *pp;
	for(ps = s, pp = p; *s != '\0'; ps = ++s, pp = p) {
		while(*ps != '\0' && *pp != '\0' && toupper(*ps) == toupper(*pp)) {
			ps++;
			pp++;
		}
		if(*pp == '\0') {
			return ps;
		}
	}
	return NULL;
}

char* 
strip (char *p, char *s) {
	if(p == NULL || s == NULL || *p == '\0') {
		return s;
	}
	char *psm;	//指向ps子串末尾、在s中的下一个字符
	size_t slen = strlen(s), tlen = strlen(p);
	while(psm = contain(s, p)) {
		delete(s, psm, slen, tlen);
	}
	return s;
}

int 
main (int argc, char *argv[]) {
	if(argc < 3) {
		printf("Usage: %s pattern string\n", argv[0]);
		return 0;
	}
	char *p = contain(argv[2], argv[1]);
	printf("%d %s\n", p-argv[2], p);

	printf("%s\n", strip(argv[1], argv[2]));
}
