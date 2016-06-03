/*打印输入中各个ascii字符出现频度的直方图*/ 

#include <stdio.h>

//声明e1-13f.c中定义的函数
extern void vgraph(int*);
extern void printdata(int*);

void main() {
	int c;
	int chs[129]; //只处理ascii字符

	int i;
	for(i = 0; i <= 128; i++) {
		chs[i] = 0;	//初始化各个ch的计数为0
	}
	while((c = getchar()) != EOF) {
		if(c < 128) {
			chs[c]++;
		}
	}

	vgraph(chs);
	printf("\n\n");
	printdata(chs);
}
