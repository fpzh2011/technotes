/* 计算从命令行输入的逆波兰表达式的值。 如 2 3 4 + * 将计算 2*(3+4) */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s expr...\n", argv[0]);
		return 0;
	}

	int a[100], sp = 0;
	
	for(int i = 1; i < argc; i++) {
		if(isdigit(*argv[i])) {
			a[sp++] = atoi(argv[i]); //数值入栈
		}
		else if (sp >= 2){
			int r, m = a[sp-2], n = a[sp-1]; //出栈
			sp -= 2;
			switch (*argv[i]) { 
				case '+':
					r = m + n;
					break;
				case '-':
					r = m - n;
					break;
				case '*':
					r = m * n;
					break;
				case '/':
					r = m / n;
					break;
				default:
					printf("bad oprator: %d %c %s\n", i, *(argv[i]), argv[i]);
					return 1;
			}
			a[sp++] = r;
		}
		else {
			printf("bad expr\n");
			return 2;
		}
	}
	if(sp == 1) {
		printf("%d\n", a[0]);
	}
	else {
		printf("bad expr");
		return 3;
	}

	return 0;
}
