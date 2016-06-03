/* 研究数组初始化的汇编代码 */

int 
f (void) {
	int a[8];
	return a[3];
}

int 
g (void) {
	int a[8] = {};
	return a[7];
}
