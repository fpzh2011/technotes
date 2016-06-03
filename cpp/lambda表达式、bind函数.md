# lambda表达式、bind函数等可调用对象

## lambda表达式

lambda表达式的一般形式是（[1]，P346）：
```cpp
[capture list] (parameter list) -> return type { function body }
```

lambda表达式实质是一个匿名函数对象（[1]，P507，CH14.8.1）。
“当我们编写了一个lambda后，编译器将该表达式翻译成一个未命名类的未命名对象。在lambda表达式产生的类中含有一个重载的函数调用运算符。”（[1]，P508）

### 捕获列表

capture list（捕获列表）是一个lambda所在函数中定义的局部变量的列表。
因为lambda是一个函数对象，它可以有自己的数据成员。编译器在构造lambda函数对象时，根据捕获列表的声明，使用调用函数环境中的局部变量，初始化lambda函数对象的数据成员。（[1]，P349-350，P508）

捕获列表、lambda匿名类数据成员的作用，就是实现lambda与调用函数环境的信息交换。

捕获可以是赋值方式，也可以是引用方式。可以显式，也可以隐式。（[1]，P349 CH10.3.3，P352）

#### 可变lambda

默认情况下，以值方式捕获的lambda对象的数据成员是不可变的。如果希望改变，可以用`mutable`修饰。（[1]，P352）

### 参数列表

parameter list（参数列表），是由lambda的调用方、在调用lambda函数时传递给lambda的。
参数列表不一定与定义lambda的函数环境有直接关系，比如将lambda用于标准库算法，通常是由标准库算法调用lambda函数，传递给lambda的是函数参数，而不是捕获列表。

如下面的例子（[1] P349）。stable_sort中用的lambda表达式的参数列表，是由stable_sort传递给它的。stable_sort拥有的是迭代器，它将迭代器解引用，得到string的引用，并传递给lambda。

这里要注意的是，lambda的参数个数，要与调用方的约定相一致。如果调用方，比如stable_sort中执行的时候，需要传递两个参数，lambda就定义两个。

参数列表是与调用方密切相关的，实参在调用方函数，由调用方将实参赋值给lambda的形参。可以认为参数列表与外部函数环境的关系不大。

```cpp
void 
biggies(vector<string> &words, vector<string>::size_type sz)
{
	elimDups(words); // put words in alphabetical order and remove duplicates
    // sort words by size, but maintain alphabetical order for words of the same size
    stable_sort(words.begin(), words.end(), 
	            [](const string &a, const string &b) 
	              { return a.size() < b.size();});

	// get an iterator to the first element whose size() is >= sz
    auto wc = find_if(words.begin(), words.end(), 
                [sz](const string &a) 
	                { return a.size() >= sz; });

	// compute the number of elements with size >= sz 
	auto count = words.end() - wc;
    cout << count << " " << make_plural(count, "word", "s")
         << " of length " << sz << " or longer" << endl;

	// print words of the given size or longer, each one followed by a space
	for_each(wc, words.end(), 
	         [](const string &s){cout << s << " ";});
	cout << endl;
}
```

### 返回类型

lambda必须使用尾置返回来指定返回类型（[1]，P346）。

关于lambda返回类型的更多信息，可以参考（[1]，P353）。

## bind函数

参考 [1] P354 CH10.3.4。

## 其它函数对象

参考 [1] P509-513, CH14.8.2-3。

## 参考资料

[1] C++ Primer 中文版（第5版）
