/* 
 * 返回一个字符串中出现频率最高的字符。字符串中只包含字母和数字。
 * Create a class called StringDup. Given a string made up of ONLY letters and
 * digits, determine which character is repeated the most in the string ('A' is
 * different than 'a'). If there is a tie, the character which appears first in
 * the string (from left to right) should be returned.
 *
 * Examples :
 *
 * 	aaiicccnn = c
 * 	aabbccdd = a
 * 	ab2sbf2dj2skl = 2
 *
 * 	Here is the method signature : 
 * 		public char getMax(String input);
 *
 * 	We will check to make sure that the input contains only letters and digits (no
 * 	punctuation marks or spaces).
 *
 * 	Division I, Level One
 */

#include <stdio.h>
#include <ctype.h>

#define N ('z' - '0' + 1)
//如果存在非字母、数字字符，返回-1；如果是空串，返回0
char 
getmax(char s[]) {
	int i, count[N], maxcount;  // '0' -> 'z'，从0开始。这样多占一些空间，但是代码简单了很多。
	char c;
	if(s == NULL) {
		return 0;
	}

	for(i = 0; i < N; i++) {
		count[i] = 0;
	}
	while((c = *s++) != '\0') {
		if(!isalnum(c)) {
			return -1;
		}
		count[c - '0']++;
	}
	for(i = 0, maxcount = 0; i < N; i++) {
		if(count[i] != 0) {
			printf("%c %d\n", (char)(i + '0'), count[i]);
		}
		if(maxcount < count[i]){
			maxcount = count[i];
			c = i + '0';
		}
	}
	return c;
}

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s teststring\n", argv[0]);
		return 0;
	}
	printf("the max char is: %c\n", getmax(argv[1]));
}
