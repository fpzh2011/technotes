/* atof 可处理形如 123.45e-6 或 123.45E-6 的科学表示法。*/

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
//测试用例 123. 0.1 123.e1 0.2E2 12e11 123456789000e-10 +123 -123 
double 
atof2 (char *s) {
	double sign = 1.0, d, power;

	for(; isspace(*s); s++)
		;
	if(!isdigit(*s)) {	// +, -, 或者没有符号位（本语句块不会被执行）
		sign = *s == '-' ? -1.0 : 1.0;
		s++;
	}

	//处理E之前的浮点数据
	for(d = 0.0; isdigit(*s); s++) {
		d = d * 10.0 + (*s - '0');
	}
	power = 1.0;
	if(*s == '.') {
		while(isdigit(*++s)) {
			d = d * 10.0 + (*s - '0');
			power *= 10.0;
		}
	}
	//处理E之后的指数
	if(*s == 'e' || *s == 'E') {
		int ep;	//E后面的指数
		double *dp = &d;
		if(*++s == '-') {
			dp = &power;
			s++;
		}
		ep = atoi(s);
		for(int i = 0; i < ep; i++) {
			*dp *= 10.0;
		}
	}
	
	//printf("%f %f, %f\n", d, sign, power);
	return d * sign / power;
}

int 
main (int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s floatstring\n", argv[0]);
		return 0;
	}
	printf("%f\n", atof2(argv[1]));
	return 0;
}
