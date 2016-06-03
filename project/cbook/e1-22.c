//尚未完成。这种实现方式太复杂，改用版本2实现。

/*将较长的输入行折成短一些的两行或多行。折行的位置在输入行的第DEFAULT_OUT_LEN 列之前的第一个空白之后。超长的连续空白不输出。*/ 

#include <stdio.h>
#include <stdlib.h>

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
ab abcd                    多行问题
ab                                    多行问题



未通过的：
1bcdefg 2bcdefg 3bcdefg 4bcdefg 5bcdefg 6bcdefg 7bcdefg 8bcdefg 9bcdefg abcdefg 
a b c
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

void 
foldline (char* buf, int maxlen) {
	char c;
	int index;			// buf的数据下标。
	int curLineLen; 	// L，当前输出行长度。包括当前判断字符。第一个字符为1。
	int isSpace;		// S，当前判断字符是否空白。1 空白，0 非空白。
	int spaceOccured;	// E，当前行之前是否输出过空白(不含当前字符)。1 输出过, 0 没有输出过。
	int lastSpaceIndex;	// 当前输出行最后一个空白字符的位置。从1开始计数。-1 表示当前输出行最近输出的是非空白字符。输出非空白字符后，重新设置该变量为-1。
	int firstNonSpace;	// 当前输出行中，尚未输出的非空白字符、最小的下标，从0开始。-1 表示没有尚未输出的非空白字符。

	curLineLen = isSpace = spaceOccured = index = 0;
	lastSpaceIndex = firstNonSpace =  -1;
	while((c = buf[index++]) != '\0') {	//此时，index指向下一个待判断字符，而不是当前正在判断的字符。
		curLineLen++;
		if(c == ' ' || c == '\t') {
			isSpace = 1;
		}
		else {
			isSpace = 0;
		}
		//主判断逻辑
		if(!isSpace && !spaceOccured) {
			putchar(c);
			lastSpaceIndex = firstNonSpace = -1;
		}
		else if (curLineLen < maxlen && isSpace) {
			lastSpaceIndex = index - 1;
			spaceOccured = 1;
			for(; firstNonSpace >= 0 && firstNonSpace < index-1; firstNonSpace++) { //输出尚未输出的非空白字符
				putchar(buf[firstNonSpace]);
			}
			firstNonSpace = -1;
		}
		else if (curLineLen >= maxlen && isSpace) {	//abc          或ab  c       
			putchar('\n');
			curLineLen = spaceOccured = 0;
			lastSpaceIndex = firstNonSpace = -1;
		}
		else {	// !isSpace && spaceOccured。a b c或a  bbbbbbbbbbbbbbbbbb或a          b  
			if(firstNonSpace < 0) {
				firstNonSpace = index -1;
			}
			for(; lastSpaceIndex >= 0 && lastSpaceIndex < index-1; lastSpaceIndex++) {	//需要输出超长单词之前的空白，如"  aaaaaaaaaaaaaaaaaaa"
				putchar(buf[lastSpaceIndex]);
			}
			lastSpaceIndex = -1;
			if(c == '\n') {
				putchar('\n');
				curLineLen = spaceOccured = 0;
				lastSpaceIndex = firstNonSpace = -1;
			}
			else if (curLineLen >= maxlen){
				//putchar('^');
				putchar('\n');
				for(; firstNonSpace >= 0 && firstNonSpace < index -1; firstNonSpace++) {
					putchar(buf[firstNonSpace]);
				}
				curLineLen = spaceOccured = 0;
				lastSpaceIndex = firstNonSpace = -1;
			}
		}
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
