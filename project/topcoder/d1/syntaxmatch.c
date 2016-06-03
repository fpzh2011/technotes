/*
括号语法匹配。
create a class called Syntax. Given a String that contains, among other
characters, parentheses '()', square brackets '[ ]' and curly
braces '{}',

create a solution that will determine if the string is balanced.  The String is
considered balanced if all of the parentheses,
brackets, and braces are
nested, opened, and closed properly.  The rules are as follows:

  There must be equal numbers of beginning and ending delimiters for each type.
  Any time a closing delimiter occurs, it must match the most recent unmatched
  open delimiter.
  An ending delimiter can never occur without a cooresponding begining delimiter
  of the same type.

EXAMPLES:

x(y(z{test})abc)  - should return true
()                - should return true
([)]              - should return false
[[]](()){{{}}}    - should return true
abc(def(ghi)      - should return false

*/

#include <stdio.h>

#define isleft(x) 	((x) == '{' || (x) == '(' || (x) == '[')
#define isright(x)	((x) == '}' || (x) == ')' || (x) == ']')
//ascii中只有()是紧邻
#define match(a,b)  ((a) == '{' && (b) == '}' || (a) == '(' && (b) == ')' || (a) == '[' && (b) == ']')
#define STACK_DEPTH 10

//用栈实现。括号匹配，返回1；括号不匹配，返回0；栈溢出，返回-1
int 
syntaxmatch (char s[]) {
	int sp = 0;
	char c, temp, stack[STACK_DEPTH];
	while((c = *s++) != '\0') {
		if(isleft(c)) {
			if(sp == STACK_DEPTH) {
				return -1;
			}
			stack[sp++] = c;	//push(c)
		}
		else if(isright(c)){
			if(sp <= 0) {
				return 0;
			}
			temp = stack[--sp];	//pop()
			if(!match(temp,c)) {
				return 0;
			}
		}
	}
	return sp == 0; 
}

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s teststring\n", argv[0]);
		return 0;
	}
	printf("%d\n", syntaxmatch(argv[1]));
}
