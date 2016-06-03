# vector基础

## 范围for语句的

使用范围for语句遍历vector等容器时，循环控制变量如果不使用引用，则每次都会进行拷贝初始化。

下面的代码，如果不使用引用，输出如下，循环控制变量与容器中的地址不同。

	ABC
	0xbfeb5b50 0x8ce8020
	DEF
	0xbfeb5b50 0x8ce8024
	GHI
	0xbfeb5b50 0x8ce8028

如果使用引用，输出如下，循环控制变量与容器中的地址相同。

	ABC
	0x9304020 0x9304020
	DEF
	0x9304024 0x9304024
	GHI
	0x9304028 0x9304028

示例代码（输入若干字符串，输出大写转换后的字符串）：

	#include <iostream>
	#include <string>
	#include <vector>

	using namespace std;

	int 
	main () {
		string s;
		vector<string> v;
		while(cin >> s) {
			v.push_back(s);
		}
		decltype(v.size()) i = 0;
		for(auto &s1: v) {
			for(auto &c : s1) {		//改为 auto c : s1 ，进行对象拷贝赋值
				c = toupper(c);
			}
			cout << s1 << endl;
			cout << &s1 << " " << &v[i++] << endl;
		}
		return 0;
	}

