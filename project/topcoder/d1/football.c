/*
给定一个正整数列表，如2 3 7，用户输入一个整数input，判断列表中的数能否相加构成input。

Create a class called Football. In football, scores are incremented by either
2, 3, or 7 points. Given a numerical input (integer between 1 and 75)
representing a final score, calculate the number of all possible combinations
of (2, 3, 7) which add up to that score. The output should be the number of
combinations found. Here are a couple of examples:

input | output | combinations
1       0
4       1        (2, 2)
9       3        (2, 7), (2, 2, 2, 3), (3, 3, 3)
11      3        (2, 2, 7), (2, 2, 2, 2, 3), (2, 3, 3, 3)

Here is the method signature (be sure your method is public:
int fetchCombinations(int input)

目前的算法不支持负值。
构造一个动态规划的树，数的根为null，值为0。
除了null节点外：每个节点的值只能是num中的可选数值；每个节点的子节点必须大于等于父节点。
从根到叶子节点的path就构成一个可能的组合。path的值是path中各个节点值的合计。
如果path的值小于用户输入值input，则在path基础上继续构造子节点；如果path的值等于input，则输出path的值；如果path的值大于input，则终止该path的探测。
                   root(0)
                  /     |  \
                 2      3   7
                /\...  /\   \
               2  3    3 7   7
              /.\
             2...7
在具体实现上，因为直接输出结果，不需要汇总、分析所有结果，所以使用栈stack来遍历这个树，尽管它是虚拟的、没有物理存在的。
初始状态下，stack空，path指向root。

遍历函数traverse：
	如果ssum > input，直接返回
	遍历大于等于父节点的各个数值 n
		if ssum + n < input then traverse(path n)
		else if ssum + n = input then print(path, n)
*/

#include <stdio.h>
#include <stdlib.h>

#define STACK_DEPTH 10

void print (int pp, int n);

static int input;				//用户输入的数值
static int path[STACK_DEPTH];	//栈，代表当前的数字排列。path[0] = 0，代表root

static const int num[] = {2,3,7};				//可选的数字
static const int nc = sizeof(num)/sizeof(int);	//可选数字个数

/*
	遍历状态树
		遍历大于等于父节点的各个数值 n
			if ssum + n < input then traverse(path n)
			else if ssum + n = input then print(path, n)
	pp path的栈顶指针，即下一个将要入栈的位置
	ssum path的当前值，即各个节点值的合计
	parentValue 父节点的值
*/
void 
traverse (int pp, int ssum, int parentValue) {
	int currNodeValue;	//正在考察的节点的值
	for(int i = 0; i < nc; i++) {
		currNodeValue = num[i];
		if(currNodeValue >= parentValue) {
			if((ssum + currNodeValue) == input) {
				print(pp, currNodeValue);
			}
			else if ((ssum + currNodeValue) < input) {
				if(pp >= STACK_DEPTH) {
					printf("stack overflow.\n");
					exit(2);
				}
				path[pp] = currNodeValue;
				traverse(pp+1, ssum+currNodeValue, currNodeValue);
			}
		}
	}
}

/* 初始化执行环境，调用遍历函数 */
void 
fetchCombinations () {
	path[0] = 0;
	traverse(1, 0, 0);
} 

/*
	pp path的栈顶指针，即下一个将要入栈的位置。不打印pp节点
	n 将要入栈的值。
*/
void 
print (int pp, int n) {
	putchar('(');
	for(int i = 1; i < pp; i++) {
		printf("%d ", path[i]);
	}
	printf("%d)\n", n);
}

int 
main () {
	while(scanf("%d", &input) == 1) {
		printf("input %d: \n", input);
		fetchCombinations();
	}
}
