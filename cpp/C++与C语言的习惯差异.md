# C++与C语言的习惯差异

## 函数形参列表声明

如果函数没有形式参数，C语言一般声明为 `f(void)` ，C++一般声明为 `f()`。

## 字符串

C++中的string的字符数组中，不含最后的 '\0' 。

C++中，字面值 'a' 默认是char类型，占一个字节；C语言中，'a' 默认是int类型。

## 字符串与算术类型转换

应该使用 string.h 中定义的系列函数。参考 http://stackoverflow.com/questions/5290089/how-to-convert-a-number-to-string-and-vice-versa-in-c

## 字符串比较

string的c_str()函数可以得到一个字符指针，指向一个C类型的字符数组，可用于strcmp等字符串比较。
但是，如果string本身的数据就包含`'\0'`字符，比如二进制数据，就不能用c_str()转换。

## 数组遍历

对于数组，标准库定义了两个函数：begin()返回第一个元素的指针，end()返回尾后指针。

## ++和--

C语言中，`++`和`--`运算符是前缀还是后缀，区别不大。但是在C++中，很多容器对象的迭代器应优先使用前缀`++`。

## 构造函数设计

优先使用构造函数处室值，如 `Person::Person(const string n, const string a) : name(n), addr(a){ }`

## sleep

C++11版本：`std::this_thread::sleep_for(std::chrono::seconds(1));`

