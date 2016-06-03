# decltype和auto的区别

## 数组声明

给定一个数组a，用 `decltype(a)` 得到的是数组，用 `auto b = a` 得到的是指针。（参考[1]，P105）

示例程序：

	#include <iostream>
	int 
	main () {
		int a[] = {1,2,3,4,5,6,7,8,9,0};
		decltype(a) b;
		auto c = a;
		std::cout
				<< "sizeof a " << sizeof(a) << std::endl
				<< "sizeof b " << sizeof(b) << std::endl
				<< "sizeof c " << sizeof(c) << std::endl;
	}

32位Ubuntu14.04环境下，输出：

	sizeof a 40
	sizeof b 40
	sizeof c 4

## 其它区别

参考[1]，Ch2.5.3

## 参考资料

	[1] C++ Primer中文版（第5版）


