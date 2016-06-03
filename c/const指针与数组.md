# const指针与数组

## 测试环境

rhel7 64位，gcc 版本 4.8.2。编译命令：`gcc -std=c99 -Wpedantic`

## const普通变量

普通const变量，如`const int`，初始化之后不能再赋值，否则会报错。如下面代码编译报错。

	int 
	main () {
		const int i = 0;
		i = 1;		//错误：向只读变量‘i’赋值
	}

## const结构对象

如果struct对象被声明为const，也不能被赋值。

	typedef struct { 
		int year;
		int month;
		int day;
	} date;

	void 
	f () {
		const date d1 = {1,2,3};
		d1.year = 0;		//错误：assignment of member ‘year’ in read-only object
	}

## const数组

声明 `const char p[]` 的含义是，数组中的元素不能被赋值。如下面的代码编译报错。

	int 
	main () {
		const char p[] = "Hello world\n";
		//char const p[] = "Hello world\n";  //一样报错
		p[0] = '\0';		//错误：向只读位置‘p[0]’赋值
	}

## const指针

`const char *p`这样的声明，和常量数组类似，**不允许对指针所指向的元素赋值**。但是，**指针变量本身是可以改变的**。

	int 
	main () {
		const char *p = "Hello world\n";
		*p = 'a';			//错误：向只读位置‘*p’赋值
		p[1] = '\0';		//错误：向只读位置‘*(p + 1u)’赋值
		p = 0;				//正常编译
	}

可以将一个const指针**赋值**给另一个const指针。但如果试图将const指针赋值给另一个非const指针，编译会警告。函数的参数也是类似的。

	void 
	f (const int *m) {
		const int *n = m;	//正常编译
		int *o = m;			//警告：initialization discards ‘const’ qualifier from pointer target type [默认启用]
		
		const int *p;
		const int *q = p;	//正常编译
		p = o;					//正常编译。const指针可以重新赋值
		int *r = p;			//警告：initialization discards ‘const’ qualifier from pointer target type [默认启用]
	}

目前看来，const int *p 与 int const *p 是等价的。

如果试图获取const struct成员的地址，编译会警告：

	typedef struct { 
		int year;
		int month;
		int day;
	} date;

	void 
	f () {
		date d1 = {1,2,3};
		const date *p = &d1;
		p->year = 0;					//错误：assignment of member ‘year’ in read-only object
		int *ip = &(p->year);		//警告：initialization discards ‘const’ qualifier from pointer target type [默认启用]
	}

## const指针数组

一个const指针如果作为函数的返回值，也遵循类似的规则。看下面这个指针数组：

	int* 
	f () {
		int i, k;
		const int *a[] = {&i};
		a[0] = &k;		//正常编译，没有警告。
		const int *q = a[0];	//正常编译，没有警告。
		int *p = a[0];	//警告：initialization discards ‘const’ qualifier from pointer target type [默认启用]
		*(a[0]) = 1;		//错误：向只读位置‘*a[0]’赋值
		return a[0];		//警告：return discards ‘const’ qualifier from pointer target type [默认启用]
	}

上述例子中，数组a被声明为const，即数组中的元素是const的；这是个指针数组，所以数组中的指针元素是const的，相当于每个元素都被声明为 const int* 。

所以，`const int *q = a[0];`可以正常编译；但是`int *p = a[0];`就会有警告。

而`a[0] = &k;`也会正常编译。因为用const修饰一个数组，是说明这个数组的元素是const。但对于指针来说，const的修饰并不是说这个指针变量本身不能修改，而是这个指针指向的对象不能被修改。
正如`*(a[0]) = 1;`就会发生编译错误。

## 再谈const指针数组

从下面的例子中可以看到，对于**指针数组**来说，`const char *weekdays[]` 与 `char* const weekdays[]` 是有区别的。
前者修饰的是指针数组，即指针中的元素都是const指针；但是对于后者，因为"*"提到了const前面，所以const只修饰数组，不修饰指针，这样语义就变为数组元素不可以修改，但是指针也不是const了。

**关键是const、"*"、"[]"之间的位置关系。**

	char* 
	weekday () {
		const char *weekdays[] = {
			"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
		};
		weekdays[0] = 0;		//正常编译
		//下面的警告，将函数返回值声明为 const char* 就可以修复，即 const char* weekday () { ... }
		return weekdays[0];	//警告：return discards ‘const’ qualifier from pointer target type [默认启用]
	}

	char* 
	f () {
		char* const weekdays[] = {
			"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
		};
		//下面两条语句，是因为指针运算符'*'在const前面，所以const修饰的只是数组，而没有修饰指针。所以数组元素不可修改，但指针也不是const了。
		weekdays[0] = 0;		//错误：向只读位置‘weekdays[0]’赋值
		*(weekdays[0]) = '0';	//正常编译，因为指针不是const了。
		return weekdays[0];	//正常编译。这里没有警告了。
	}

	//这里 char const *weekdays[] 的声明和第一个函数weekday()中的声明应该是等价的。
	char* 
	f2 () {
		char const *weekdays[] = {
			"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
		};
		weekdays[0] = 0;
		return weekdays[0];	//警告：return discards ‘const’ qualifier from pointer target type [默认启用]
	}

下面的这个简单例子也说明了同样的情况。

	//const与*的位置关系，决定了const限定的是指针指向的对象，还是指针本身。
	void
	f () {
		int const *p;	//const指针，限定的是指针指向的对象。
		p = 0;
		*p = 0;	//错误：向只读位置‘*p’赋值
		
		int* const s;	//const变量s，变量的值不可更改。
		s = 0;		//错误：向只读变量‘s’赋值
		*s = 0;
	}

## 参考资料

[1] C程序设计语言（K&R第二版，P193，） ISNB：978-7-111-12806-9
