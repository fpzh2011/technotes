#include <stdio.h>

int 
main () {
	int day, month, year, i = -1, j = -1;
	char monthname[20];
	char line[100];
	char testbuf[100];
	testbuf[0] = '\0';

	if((j = scanf("%d/%d/%d", &year, &month, &day)) == 3) {
		printf("pattern2: %d-%d-%d\n", year, month, day);
	}
	else if((i = scanf("%d %s %d", &year, monthname, &day)) == 3) {
		printf("pattern1: %d-%s-%d\n", year, monthname, day);
	}
		
	scanf("%s", testbuf);
	printf("i = %d, j = %d, the rest is: %s\n", i, j, testbuf);
	
	return 0;
}
