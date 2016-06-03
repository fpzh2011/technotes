/* rightrot(x,n) 将x循环右移n位，即从最右端移出的位，将从最左端移入。*/

#include <stdio.h>

int 
rightrot (unsigned int x, int n) {
	while(n-- > 0) {
		if(x & 1) {
			x = (x >> 1) | (1 << (sizeof(int)*8-1));
		}
		else {
			x >>= 1;
		}
	}
	return x;
}

int
main () {
	printf("r(11,1)=%x\n", rightrot(11,1));
	printf("r(32,2)=%x\n", rightrot(32,2));
	printf("r(38,3)=%x\n", rightrot(38,3));
	return 0;
}
