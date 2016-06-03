#include <stdio.h>

int 
main () {
	int day, month, year, i = -1, j = -1;
	char monthname[20];
	char line[100];
	char testbuf[100];
	testbuf[0] = '\0';

	while(fgets(line, 100, stdin) != NULL) {
		if((i = sscanf(line, "%d %s %d", &year,monthname, &day)) == 3) {
			printf("pattern1: %d %s %d\n", year, monthname, day);
		}
		else if((j = sscanf(line, "%d/%d/%d", &year, &month, &day)) == 3) {
			printf("pattern2: %d %d %d\n", year, month, day);
		}
		sscanf(line, "%s", testbuf);
		printf("invalid, i = %d, j = %d, the rest is: %s\n", i, j, testbuf);
	}
	return 0;
}
