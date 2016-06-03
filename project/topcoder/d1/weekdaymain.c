/* 给定一个日期（非公元前），输出星期 */

#include <stdio.h>

#include "weekday.h"

int 
main() {
	date date1;
	while(scanf("%4d%2d%2d", &date1.year, &date1.month, &date1.day) == 3) {
		printdate(&date1);
		printf(" %s\n", weekday(&date1));
	}
	return 0;
}
