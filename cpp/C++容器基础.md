# C++容器基础

## 引用失效

如果使用了容器的迭代器，或者元素的引用、指针，要特别注意对容器的操作是否会使这些失效。
具体可以参考标准文档。

以双端队列插入为例，参考 C++ 标准2011，`23.3.3.4 deque modifiers -> push_back -> Effects` 部分（P753）。
如果在队列中间插入元素，会使所有的迭代器和元素引用失效；
但是在队列两端插入元素，仅会使迭代器失效，而对元素引用没有影响。

## 栈 Stack

从标准 23.3.3.1 Class template deque overview - 1 对deque的描述看，用deque实现stack，性价比应该是比较高的。

## 迭代器类型

各种迭代器的关系、支持的操作等，可以参考标准 Ch 24.2 Iterator requirements 。

标准Ch 23 Containers library 中，每个容器的开头都明确指出了该容器支持的迭代器类型。
如 Ch 23.3.3.1 Class template deque overview ，第一句话就明确支持 random access iterators （P749）。

http://www.cplusplus.com/ 中也明确列出了支持的迭代器。

## 对象的添加与读取

从容器中添加一个对象a时，其实是添加对象的拷贝，容器中的元素与a不在一个内存。（参考[1]，P306）
从容器获取一个元素（而不是引用）时，也是获取元素的拷贝。

## 销毁元素对象

从vector中删除元素后，元素对象会被自动销毁。deque和list都验证可以。
可以参考 http://www.cplusplus.com/ 的说明。

也可以用类似这样的代码验证：

	#include <iostream>
	#include <vector>
	
	using namespace std;
	
	class Test {
	public:
		~Test() { cerr << "destory Test." << endl; }
	};

	int 
	main () {
		vector<Test> v(10);
		v.pop_back();
		cout << "Press Enter to exit." << endl;
		cin.get();
		return 0;
	}

## 容器中对象的创建与销毁

allocator类（[1] P427）是如何调用对象的构造函数，容器又是如何实现自动销毁对象的呢？

应该是用类似于“定位new”的语言机制（[1] P729）。

## map

std::map is a binary search tree data structure.

若想用自定义类作为`map`的key，或者`set`的元素，有两种方式：
* 定义`operator<`操作符重载，或者特化`less`。这种方式不需要使用者在创建`map`时显式指定排序。
* 定义Comparator函数对象（或lambda表达式）。这种方式没有强化对象的序关系，但创建`map`时需要显式指定按Comparator排序。
http://thispointer.com/stdmap-tutorial-part-3-using-user-defined-class-objects-as-key-in-stdmap/
http://www.walletfox.com/course/mapwithcustomclasskey.php
```cpp
struct UserNameComparator
{
    bool operator()(const User & left, const User & right) const
    {
        return (left.getName() > right.getName());
    }
};
std::map<User, int, UserNameComparator> m_UserInfoMap;
```

更简便的方式，是用某个内置类型代表自定义类型，比如`string`或`int_64`。

## unordered_map

若想用自定义类作为`unordered_map`的key，或者`unordered_set`的元素，需要做两件事：
* 定义hash function
* 定义`operator==()`
https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key

## 参考资料

	[1] C++ Primer第5版（中文版）
	[2] C++ 标准2011
	[3] http://www.cplusplus.com/
	[4] http://www.boost.org/



