#include <stdio.h>

/*统计空格、tab、换行的个数*/ 
void main() {
		int c, scnt, tcnt, enterCnt;

		scnt = tcnt = enterCnt = 0;
		while((c=getchar()) != EOF) {
				switch (c) {
					case ' ':
						scnt++;
						break;
					case '\t':
						tcnt++;
						break;
					case '\n':
						enterCnt++;
						break;
				}
		}
		printf("space is %d, tab is %d, enter is %d\n", scnt, tcnt, enterCnt);
}
