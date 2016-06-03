/*
给定两个整数source, target。
将source按十进制位数分成若干位的组合、形成几个新的整数，如19967可以分成{1,99,67}或{1,9,967}等。
source中各位数字必须从左向右读取，其相对位置不能改变。
拆分数的各种组合，合计值最大、且小于等于target的，作为结果返回；如果没有小于等于target的，返回0。

*tosplit = 19967, target = 1000 -> 19 + 967 -> return = 986
*tosplit = 19967, target = 100 -> 19 + 9 + 67 -> return = 95
*tosplit = 22215, target = 225 -> 2 + 2 + 215 -> return = 219
*tosplit = 22235, target = 1 -> return = 0
31 2
24 3
23 5
23 6
11 3
1234 1234
1234 235
31 3
30 3
210 3
1119 5

Implement a class IntSplit, which contains a method maxSplit. maxSplit takes
two ints as parameters:  a source and a target.  The function should split the
source between digits any number of times so the sum of all the pieces is as
close to, without exceeding, the target as possible and return this sum.  If it
is impossible to not exceed the target, the function should return 0.

For example, if source is 19967 and target is 1000, source should be split
between the 9's:  19 + 967 = 986 and the function should return 986.  Note if
the integer were split between the 1 & 9 and between the 6 & 7, the result
would be 1 + 996 + 7 = 1004, which is greater than the target, and therefore is
not considered.

The method signature is (be sure your method is public):
int maxSplit(int source, int target)

TopCoder will ensure the following:
- Both source and target are between 0 and 99999999, inclusive.

NOTES:
*The returning value can equal the target, it just cannot be greater than the
target.
*The digits cannot be rearranged and the digits must be read from left to right.
*The solution must run in under 6 seconds.

Examples:
*tosplit = 19967, target = 1000 -> 19 + 967 -> return = 986
*tosplit = 19967, target = 100 -> 19 + 9 + 67 -> return = 95
*tosplit = 22215, target = 225 -> 2 + 2 + 215 -> return = 219
 */

//gcc -lm splitint.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*===================================================================*/

//默认的split数组长度
#define DEFAULT_LEN 20

/*
用于记录一个split的数字构成。
component[0]指向第一个进来的数字（即最右侧的数字）。
component[tail]指向该split的最左侧的数字。
数组最大到component[len-1]。
*/
typedef struct {
	int *component;
	int tail;
	int len;
} split;

//新建一个split，只包含整数n
static split*
newSplitByInt (int n) {
	split *p = (split *)malloc(sizeof(split));
	if(NULL == p) {
		fprintf(stderr, "cannot allocate memory for split.\n");
		exit(1);
	}
	if(NULL == (p->component = malloc(sizeof(int) * DEFAULT_LEN) ) ) {
		fprintf(stderr, "cannot allocate memory for split component.\n");
		exit(1);
	}
	p->component[0] = n;
	p->tail = 0;
	p->len = DEFAULT_LEN;
	return p;
}

//销毁split
static void
destroySplit (split *p) {
	if(NULL == p) {
		return;
	}
	if(NULL != p->component) {
		free(p->component);
		p->component = NULL;
	}
	free(p);
}

//将p的数组扩容。如果失败，保持p的内容不变
static void
expandSplit (split* p) {
	int *pc;
	if(NULL == p || p->len <= 0 || NULL == (pc = malloc(sizeof(int) * p->len * 2) ) ) {
		return;
	}
	if(NULL != p->component) {
		free(p->component);
	}
	p->component = pc;
	p->len *= 2;
}

//将整数n添加到split
static split *
addToSplit (split *p, int n) {
	if(NULL == p) {
		return NULL;
	}
	if(p->tail >= (p->len - 1)) {
		expandSplit(p);
		if(p->tail >= (p->len - 1)) {	//expand失败
			fprintf(stderr, "cannot allocate memory for split expand.\n");
			exit(1);
		}
	}
	p->component[++(p->tail)] = n;
	return p;
}

//计算split中component的和
static int
calcSplitSum (const split *p) {
	if(NULL == p) {
		return 0;
	}
	int sum = 0;
	for(int i = 0; i <= p->tail; i++) { 
		sum += p->component[i];
	}
	return sum;
}

//打印split中的数据
static void
printSplit(const split *p) {
	if(NULL == p) {
		printf("null split, result: 0\n");
		return;
	}
	printf("result: %d ", calcSplitSum(p));
	for(int i = 0; i <= p->tail; i++) {
		printf("comp: %d ", p->component[i]);
	}
	putchar('\n');
}

/*===================================================================*/

//a[0]-a[n]构成一个十进制数字。如a[]={1,2,3}，则返回123。
static int
mergeInt (const int *a, int n) {
	if(NULL == a || n < 0) {
		return 0;
	}
	int sum = 0;
	for(int i = 0; i <= n; i++) {
		sum = sum * 10 + a[i];
	}
	return sum;
}

/*
数组a存储数字0-9。a[0]是最左侧的数字，a[n]是最右侧的数字。
target是针对数组a的。如果a之前还有前缀，已经减去前缀的值。
*/
split*
calcMaxSplit (const int a[], int n, int target) {
	int max = 0, m, sum;
	split *p = NULL, *ptmp;
	if(NULL == a || *a > target || n < 0) {
		return NULL;
	}
	else if(0 == n) {
		return newSplitByInt(*a);	//在最右侧的数字上，构造第一个split
	}
	else if((sum = mergeInt(a, n)) <= target) {
		return newSplitByInt(sum);
	}
	for(int i = 0; i <= n-1; i++) {
		m = mergeInt(a, i);	//将a[0]至a[i]拼成一个数字
		if(m > target) { //m已经大于target了，再往下计算只会越来越大，所以直接返回
			return p;
		}
		if(
			NULL == (ptmp = calcMaxSplit(a+i+1, n-i-1, target-m)) || 
			(sum = m + calcSplitSum(ptmp)) > target ) {
				destroySplit(ptmp);
				continue;		//这里是用break，还是continue，还不确定。break效率更高，但可能漏掉有效结果。continue更保险。
		}
		if(sum > max) {
			max = sum;
			destroySplit(p);
			p = addToSplit(ptmp, m);
		}
	}
	return p;
}

void
splitInt (int source, int target) {
	if (source <= 0 || target <= 0 || source < target) {
		fprintf(stderr, "error input.\n");
		return;
	}
	int n = (int)(log10((double)source)) + 1;	//source的十进制位数
	int *a = (int*)malloc(n * sizeof(int));
	if(NULL == a) {
		fprintf(stderr, "cannot allocate source array.\n");
		exit(1);
	}
	for(int i = n-1, temp = source; i >= 0; i--) {
		a[i] = temp % 10;
		temp /= 10;
	}
	split *s = calcMaxSplit(a, n-1, target);
	printSplit(s);
	destroySplit(s);
	free(a);
}

int 
main () {
	int source, target;
	while(scanf("%d %d", &source, &target) == 2) {
		splitInt(source, target);
	}
}
