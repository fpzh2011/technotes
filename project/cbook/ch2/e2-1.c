/*打印常用数据类型的最大值、最小值*/ 
#include <stdio.h>
#include <limits.h>

void 
main() {
	printf("从limits.h读取最大、最小值\n");
	printf("sizeof(int)=%d, %d -> %d, %u\n", sizeof(int), INT_MIN, INT_MAX, UINT_MAX);
	printf("sizeof(char)=%d, %d -> %d, %u\n", sizeof(char), CHAR_MIN, CHAR_MAX, UCHAR_MAX);
	printf("sizeof(short)=%d, %d -> %d, %u\n", sizeof(short), SHRT_MIN, SHRT_MAX, USHRT_MAX);
	printf("sizeof(long)=%d, %ld -> %ld, %lu\n", sizeof(long), LONG_MIN, LONG_MAX, ULONG_MAX);
	
	printf("\n自行计算最大最小值\n");
	printf("sizeof(int)=%d, %d -> %d, %u\n", sizeof(int), 
		1 << (sizeof(int)*8 - 1), 
		(1 << (sizeof(int)*8 - 1)) ^ -1, 
		-1
		);
	//因为不确定char的长度，所以需要通过移位实现
	printf("sizeof(char)=%d, %d -> %d, %u\n", sizeof(char), 
		(char)(1 << (sizeof(char)*8 - 1)),	//char_min 
		(unsigned int)(~0) >> (sizeof(int)*8-sizeof(char)*8+1),
		(unsigned int)(~0) >> (sizeof(int)*8-sizeof(char)*8)
		);

	//查看char默认是否带符号
	char c = '\xff';
	signed char sc  = '\xff';
	unsigned char uc = '\xff';
	int a = sc, b = uc, i = c;
	printf("\nchar:%d, unsigned char:%d, signed char:%d\n", i, b, a);

}
