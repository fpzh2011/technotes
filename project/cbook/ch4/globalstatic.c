/* 研究全局变量、静态变量的汇编代码 */

int g = 'a';

void 
f (int v) {
	g = 'b';
	static int s1 = v;
	static int s2 = 'd';
	s2 = v;
}


