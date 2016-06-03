/*第1章，练习16的函数。处理任意长度的输入行*/

#include <stdio.h>
#include <stdlib.h>

#include "e1-16f.h"

/*
从标准输入读入数据到缓冲区s，lim是缓冲区长度。如果缓冲区长度不够，新建一个缓冲区，并更新缓冲期长度。
返回读入的字符数，不包括结尾的'\0'
*/ 

#define DEFAULT_BUF_LEN 1

int
getoneline (char** s, int* lim) {
	if(s == NULL) {
		return;
	}
	else if(*s == NULL || *lim < 0) {
		if(*s != NULL) {
			free(*s);
		}
		*s = malloc(DEFAULT_BUF_LEN);
		*lim = DEFAULT_BUF_LEN;
	}
	
	char* buf = *s;
	int c, count = 0;
	while((c = getchar()) != EOF) {
		if(count >= (*lim)-1) {	//这些语句可以用函数realloc简化
			int newbuflen = (*lim) * 2;
			char* newbuf = malloc(newbuflen);
			copydata(newbuf, buf, *lim);
			free(buf);
			buf = newbuf;
			*lim = newbuflen;
		}
		buf[count] = c;
		count++;
		if(c == '\n') {
			break;
		}
	}
	buf[count] = '\0';
	*s = buf;
	if(count <= 0) {
		count = -1;
	}
	return count;
}

/*从缓冲区from拷贝数据到to，'\0'为终止符。*/
void 
copystr (char to[], char from[]) {
	if(to == NULL || from == NULL) {
		return;
	}
	
	int i = 0; 
	while((to[i] = from[i]) != '\0') {
		 i++;
	}
} 

/*从缓冲区from拷贝len字节的数据数据到to*/
void 
copydata (char to[], char from[], int len) {
	if(to == NULL || from == NULL) {
		return;
	}
	
	while(--len >= 0) {
		 to[len] = from[len];
	}
} 



