#include <stdio.h>

/*以每行一个单词的形式打印输入*/ 

//状态值：是否在单词内
#define IN 1
#define OUT 0

void main() {
	int c;
	int state = OUT;

	while((c = getchar()) != EOF) {
		if(c != ' ' && c != '\n' && c != '\t') {
			putchar(c);
			state = IN;
		}
		else if (state == IN) {
			putchar('\n');
			state = OUT;
		}
	}
}
