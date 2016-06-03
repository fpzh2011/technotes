#include <stdio.h>

/*查看EOF的值（-1）*/ 
int main() {
		int c;
		while ((c=getchar()) != EOF) {
				;
		}
		printf("EOF is: %d", c);
}
