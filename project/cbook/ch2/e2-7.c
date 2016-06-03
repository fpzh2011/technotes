/* invert(x,p,n) 返回对x执行下列操作后的结果值：
 * 将x中从右侧0位起算、第p位开始的n个二进制位求反，x的其余各位保持不变。
 */

#include <stdio.h>

int 
invert (unsigned int x, int p, int n) {
	size_t s = sizeof(int) * 8;
	return x ^= ~0u >> (s - n) << p;
}

void 
main () {
	printf("%x\n", invert(85,2,3));
}
