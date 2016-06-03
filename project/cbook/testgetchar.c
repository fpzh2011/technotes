#include <stdio.h>

/*
 * 测试getchar如何读取汉字
 * usage: echo 中国 | testgetchar
 * */
void main() {
	int c ;
	while((c= getchar()) != EOF) {
		printf("%x\n", c);		
	}
}
