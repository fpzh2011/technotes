//尚未完成

/*将较长的输入行折成短一些的两行或多行。折行的位置在输入行的第DEFAULT_OUT_LEN 列之前的第一个空白之后。超长的连续空白不输出。*/ 

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e1-16f.h"

#define DEFAULT_OUT_LEN 10

/*
测试用例
通过的：
abcd
abcdefg ab
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa    
ab c          
a  bbbbbbbbbbbbbbbbbb
ab abcd                    
ab                                    



                              全部空行
1bcdefg 2bcdefg 3bcdefg 4bcdefg 5bcdefg 6bcdefg 7bcdefg 8bcdefg 9bcdefg abcdefg 
a b c
a b c 
a            
a b c d e f g h i 
ab ab ab ab de de de abc
bc  abc  abc    def    def    def
abc          abc    abc    abc    abc


    aaaaaaaaaaaaaaaaaaaaaaa
a          b  
*/

/*
	是否折行需要考虑3个重要状态：
		L：当前输出行的长度是否超限。1 超限，0 不超限
		S：当前字符是否空白。1 是空白，0 不是空白
		E：当前行之前是否已经出现过空白。1 有，0 没有
	对于所有情况，可以用三位的二进制数LSE表示。如010表示当前行不超限、当前字符是空白、当前行之前没有出现过空白。
	以下的每个逻辑判断，都可以用一个或若干个二进制数表示。
*/

int
min(int a, int b){
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

void 
foldline (char* buf, int maxOutputLen) {
	char c;
	int curLineStart, nextLineStart, tempNext;
	int len = strlen(buf);

	curLineStart = nextLineStart = 0;
	while(curLineStart < len) {
		nextLineStart = min(curLineStart + maxOutputLen, len - 1);
		if (isspace(buf[nextLineStart])) {	//下一行首字节是空白
			//后退到第一个非空白
			for(tempNext = nextLineStart - 1; curLineStart <= tempNext && isspace(buf[tempNext]); tempNext--)
				;
			for(; curLineStart <= tempNext; curLineStart++) {	//输出curLine
				putchar(buf[curLineStart]);
			}
			if(curLineStart - 1 == tempNext) {
				putchar('\n');
			}
			for(; nextLineStart <= len && isspace(buf[nextLineStart]); nextLineStart++) //nextLine前进到第一个非空白字符
				;
		}
		else {	//下一行第一个字节不是空白 
			//后退到第一个空白
			for(tempNext = nextLineStart - 1; curLineStart <= tempNext && ! isspace(buf[tempNext]); tempNext--)
				;
			if(curLineStart <= tempNext) { //后退存在空白。输出curLine，重新设置nextLineStart
				nextLineStart = tempNext + 1;
				for(; curLineStart <= tempNext && isspace(buf[tempNext]); tempNext--)	//清空空白
					;
				for(; curLineStart <= tempNext; curLineStart++) {		//输出curLine
					putchar(buf[curLineStart]);
				}
				if(curLineStart - 1 == tempNext) {
					putchar('\n');
				}
			}
			else {				//后退不存在空白。nextLineStart向前推进找到第一个空白或\0，输出。
				for(; nextLineStart <= len && ! isspace(buf[nextLineStart]); nextLineStart++)
					;
				for(; curLineStart < nextLineStart; curLineStart++) {
					putchar(buf[curLineStart]);
				}
				if(nextLineStart >= len) {
					break;
				}
			}
		}
		curLineStart = nextLineStart;
	}
}

void
main () {
	int buflen = 1;
	char* buf = malloc(buflen);
	int n;
	
	while ((n = getoneline(&buf, &buflen)) >= 0) {
		foldline(buf, DEFAULT_OUT_LEN );
	}
	free(buf);
}
