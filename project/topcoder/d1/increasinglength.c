/* 给定一个整数数组，返回数组中最大递增序列的长度。如{-4, 5, -2, 0 , 4 , 5 , 9 ,9}返回5，{1,0}返回1。*/

/*
Class Name: Series
Method Name: increasingLength
Paramaters: int[]
Returns: int

Implement a class Series, which contains a method increasingLength.
increasingLength takes an int[] and returns the length of the largest strictly
increasing sequence in the int[].  A strictly increasing sequence is a sequence
of numbers such that every number in the sequence after the first is strictly
greater than the number before it.

Here is the method signature:
public int increasingLength(int[] data);

*The int[] will contain Integers between -10000 and 10000, inclusive.
*The int[] will have at least 1 element and at most 1000 elements.

Note:
*A single element of the int[] is a strictly increasing sequence of size 1.
*Read the int[] in the order in which element 0 is before element 1...

Examples:
*If the int[] is {-4, 5, -2, 0 , 4 , 5 , 9 ,9}, the method should return 5.
*If the int[] is {1,0} the method should return 1.
*/

#include <stdio.h>

//n:数组长度
int 
increasinglength (int a[], int n) {
	int count, max;
	count =  max = 1;
	if(n <= 0 || a == NULL) {
		return 0;
	}
	for(int i = 1; i < n; i++) {
		if(a[i] > a[i-1]) {
			count++;
			if(count > max) {
				max++;
			}
		}
		else {
			count = 1;
		}
	}
	return max;
}

int 
main () {
	int a[] = {-4, 5, -2, 0 , 4 , 5 , 9 , 9};
	printf("%d\n", increasinglength(a, sizeof(a)/sizeof(int)));
	int b[] = {1, 0};
	printf("%d\n", increasinglength(b, sizeof(b)/sizeof(int)));
}
