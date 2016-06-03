/*
给定一个三角形整数序列，从顶点到底的路径中，找到一条合计值最大的路径。


Implement a class TriTravel, which contains a method bestWayDown.  The
method analyzes a set of Integers organized as a triangle and finds the path
from the top to the bottom of the triangle with the maximum possible sum of
numbers passed on route (including both the starting and ending number).  The
method returns an int that is this maximum sum.  The method is passed an int[]
that contains the elements in the triangle.  The elements of the int[]
correspond to the triangle as follows.  The numbers in this triangle are the
index of the elements in the int[]:
        0
      1     2
    3     4     5
  6     7     8     9
10    11    .  .  .  .  . 
     .     .     .
     .     .     .
     .     .     .
For example, the triangle
    1
  6  7
 4 -1 6
5 8  9 0
is represented by the int[]{1,6,7,4,-1,6,5,8,9,0}
The path starts at the element in the top row.  Each step in the path is
either diagonally down one row to the right or diagonally down one row to the
left.  For instance, given the triangle above, the maximum sum route down is 1
-> 7 -> 6 -> 9 and the method returns the sum, 23.

Here is the method signature:
public int bestWayDown(int[] triValues);
*triValues is an int[] of elements between -100 and 100, inclusive.
*The number of elements in triValues represents a triangle perfectly. (that is,
it has 1 or 3 or 6 or 10 or 15 or ... elements).
*triValues has at least 1 element (a 1 row triangle).
*triValues has at most 45 elements (a 9 row triangle).

Examples:
*If triValues = {5, 4, 1, 2, 8, -1}, the path 5->4->8 yeilds the maximum sum
and the method returns the sum: 17.
 */

#include <stdio.h>
#include <stdlib.h>

//校验三角形的点个数是否有效。有效的值如：1 3 6 10 15
//有效返回层高，无效返回0
static int 
validateTriCount (int count) {
	int sum = 0;
	for(int i = 1; sum < count; i++) {
		if((sum += i) == count) {
			return i;
		}
	}
	return 0;
}

//递归深度扫描树
//a存储三角数组；index从0开始；row从1开始；maxrow为最大层高
//index的左子=index+row，index的右子=index+row+1
static int 
dotravel (int a[], int index, int row, int maxrow) {
	if(row == maxrow) {
		return a[index];
	}
	else if(row < maxrow) {
		int left = dotravel(a, index + row, row + 1, maxrow);
		int right = dotravel(a, index + row + 1, row + 1, maxrow);
		return a[index] + (left > right? left:right);
	}
	else {	//这里不可能进入，否则算法错误
		fprintf(stderr, "row(%d) > maxrow(%d)\n", row, maxrow);
		exit(1);
	}
}

//a存储三角数组，len是数组长度，a[0]-a[len-1]有效
int
tritravel (int a[], int len) {
	int maxrow = validateTriCount(len);
	if(maxrow == 0 || NULL == a) {
		fprintf(stderr, "pointer is null, or invalid number count %d\n", len);
		exit(1);
	}
	//printf("the result is: %d\n", dotravel(a, 0, 1, maxrow));
	return dotravel(a, 0, 1, maxrow);
}

/*
int 
main (void) {
	int a[]= {1,6,7,4,-1,6,5,8,9,0};
	int len = sizeof(a)/sizeof(int);
	printf("%d %d", len, tritravel(a, len));
	//int b[] = {5, 4, 1, 2, 8, -1};
	//ck_assert_int_eq( tritravel(b, sizeof(b)/sizeof(int)), 17);
}
*/
