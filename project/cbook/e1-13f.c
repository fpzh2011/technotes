#include <stdio.h>

/*打印直方图，水平方向*/

//最大数据长度
#define MAXLEN 128 

void count(int *a);
void hgraph(int *a);
void vgraph(int *a);
void printdata(int *a);
void vprint(int c);

/*水平直方图。如果存在超长的单词，应该考虑对输出=的长度进行转换。这时就需要记录最大（最小）长度。*/
void hgraph(int *nch) {
	int i, j;
	for(i = 1; i <= MAXLEN; i++) {
		if(nch[i] != 0) {
			printf("%3d", i);
			for(j = 0; j < nch[i]; j++) {
				putchar('=');
			}
			putchar('\n');
		}
	}

}

/*垂直直方图*/
void vgraph(int *nch) {
	int i, j;
	int maxcount = 0;
	for(i =- 0; i <= MAXLEN; i++) {
		if(nch[i] > maxcount) {
			maxcount = nch[i];
		}
	}

	// 打印直方图坐标点。i 每个值的打印高度；j 打印的数组下标
	for(i = maxcount; i >= 1; i--) {
		for(j = 1; j <= MAXLEN; j++) {
			if(nch[j] == 0) {	/*如果值为0，就不打印*/ 
				continue;
			}
			if(nch[j] < i){
				vprint(' ');
			}
			else {
				vprint('|');
			}
		}
		putchar('\n');
	}

	for(j = 1; j <= MAXLEN; j++) {
		if(nch[j] != 0) {
			printf("%5d", j);
		}
	}
	putchar('\n');
}

/*垂直直方图，单个点打印。每个点横向占5个字符。*/
void vprint(int c) {
	putchar(' ');
	putchar(' ');
	putchar(' ');
	putchar(' ');	
	putchar(c);
}


/*打印数组的原始数值，用于调试*/ 
void printdata(int *nch) {
	int i;
	for(i = 1; i <= MAXLEN; i++) {
		if(nch[i] != 0) {
			printf("%d  %d\n", i, nch[i]);
		}
	}
}
