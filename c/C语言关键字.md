# C语言关键字

## C99新增关键字

restrict 修饰指针。指针指向的对象，仅能通过该指针赋值。

	restrict type qualifier, 6.7.3, 6.7.3.1
	restrict-qualified type, 6.2.5, 6.7.3
	
_Bool 布尔类型。任何一个标量x被转换为_Bool值时，遵循 b = x == 0 ? 0 : 1

	_Bool type, 6.2.5, 6.3.1.1, 6.3.1.2, 6.7.2

_Complex 复数

	_Complex types, 6.2.5, 6.7.2, 7.3.1, G
	
_Imaginary 虚数

	_Imaginary types, 7.3.1, G
	
inline 函数说明符

	inline, 6.7.4

## C11新增关键字

_Alignas 对齐说明符

	_Alignas, 6.7.5

_Alignof 对齐说明符

	_Alignof operator, 6.3.2.1, 6.5.3, 6.5.3.4

_Atomic 类型说明符，而不是限定词。

	_Atomic type qualifier, 6.7.3
	_Atomic type specifier, 6.7.2.4

_Generic 对范型的支持

	6.5.1.1, 6.5.1.1-5的例子

_Noreturn 用于修饰函数声明，表明该函数不会返回到调用者。

	_Noreturn, 6.7.4

_Static_assert

	_Static_assert, 6.7.10, 7.2

_Thread_local 声明一个对象has thread storage duration，即仅在线程运行期间存在。

	_Thread_local storage-class specifier, 6.2.4,6.7.1, 7.26.1

## 参考资料

C99标准

C11标准

