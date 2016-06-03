/* setbits(x, p, n, y)，该函数返回对x执行下列操作后的结果值：
 * 将x中从右侧0位起数第p位开始的n个二进制位设置为y中最右边n位的值，x的其余各位保持不变。
 */

#include <stdio.h>

/* 输出整数的二进制格式 */
void 
printb (int i) {
	putchar('0');	//如果i==0，输出一个0。
	unsigned int c = 1 << (sizeof(int)*8 - 1); //最高位为1，其余为0 
	for(; c != 0 && (c & i) == 0; c = c >> 1) //定位到i的最高非0位
		;
	for(; c != 0; c = c >> 1) { //从最高非0位开始输出
		/*
		if((c & i) == 0) {
			putchar('0');
		}
		else {
			putchar('1');
		}
		*/
		putchar('0' + ((c&i) != 0));
	}
	putchar('\n');
}

int 
setbits(unsigned int x, unsigned int p, unsigned int n, unsigned int y) {
	size_t s = sizeof(int) * 8;
	y &= (~0u) >> (s - n); //保留y的最右n位，其他位置0
	y <<= p;
	//printf("convert y is %d ", y);
	//printb(y);

	x &= ~( (~0u) >> (s - n) << p ); //x的修改位置为0，其他位保持不变
	//printf("convert x is %d ", x);
	//printb(x);
	return x | y;
}


void 
main () {
	int x = 769, p = 3, n = 4, y = 83;
	printf("x = %d is:", x);
	printb(x);
	printf("y = %d is:", y);
	printb(y);
	printf("p = %d, n = %d\n", p, n);
	printf("result is: %d ", setbits(x, p, n, y));
}
