#include <stdio.h>

/*将输入复制到输出，将tab替换为\t，将退格替换为\b，将\替换为\\*/ 
/*如果转换规则再多，得考虑用数组*/ 
void main() {
	int c;

	while((c = getchar()) != EOF) {
		switch (c) {
			case '\t':
				putchar('\\');
				putchar('t');
				break;
			case '\b':
				putchar('\\');
				putchar('b');
				break;
			case '\\':
				putchar('\\');
				putchar('\\');
				break;
			default:
				putchar(c);
		}
	}
}
