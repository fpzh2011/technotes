# Go语言基础

Go 语言实践：编写可维护的程序的建议 https://juejin.im/post/5e0ab70ef265da5d0f098988

Go是一门编译型语言，而且是静态编译（[3]，CH1.1）。

Go语言的代码通过包（package）组织，包类似于其它语言里的库（libraries）或者模块（modules）。一个包由位于单个目录下的一个或多个.go源代码文件组成, 目录定义包的作用。每个源文件都以一条package声明语句开始（[3]，CH1.1）。

main包比较特殊。它定义了一个独立可执行的程序，而不是一个库。在main包里的main函数也很特殊，它是整个程序执行时的入口（[3]，CH1.1）。

Go语言不需要在语句或者声明的末尾添加分号，除非一行上有多条语句。实际上，编译器会主动把特定符号（应该不包括花括号）后的换行符转换为分号, 因此换行符添加的位置会影响Go代码的正确解析（[3]，CH1.2）。

Go语言不允许使用无用的局部变量（local variables），因为这会导致编译错误。

## 帮助

```
go doc text/template
```

## 变量

### 命名

关键字列表（[3]，CH2.1）。
一个名字如果是在函数外部定义，那么将在当前包的所有文件中都可以访问（[3]，CH2.1）。
名字的开头字母的大小写决定了名字在包外的可见性。如果一个名字是大写字母开头的，那么它将是导出的，也就是说可以被外部的包访问（[3]，CH2.1）。
在习惯上，Go语言程序员推荐使用 驼峰式 命名

### 声明

Go语言主要有四种类型的声明语句：var、const、type和func，分别对应变量、常量、类型和函数实体对象的声明（[3]，CH2.2）。
包一级的各种类型的声明语句的顺序无关紧要（译注：函数内部的名字则必须先声明之后才能使用）。（[3]，CH2.2）

### 变量

如果初始化表达式被省略，那么将用零值初始化该变量。 数值类型变量对应的零值是0，布尔类型变量对应的零值是false，字符串类型对应的零值是空字符串，接口或引用类型（包括slice、map、chan和函数）变量对应的零值是nil。数组或结构体等聚合类型对应的零值是每个元素或字段都是对应该类型的零值。（[3]，CH2.3）
**在Go语言中不存在未初始化的变量。**（[3]，CH2.3）

一行语句可以声明多个类型不同的变量（[3]，CH2.3）：
```go
var b, f, s = true, 2.3, "four" // bool, float64, string
```

在包级别声明的变量会在main入口函数执行前完成初始化，局部变量将在声明语句被执行到的时候完成初始化。一组变量也可以通过调用一个函数，由函数返回的多个返回值初始化。（[3]，CH2.3）

在函数内部，`:=`可以不带`var`关键字，但是需要在声明同时初始化（[3], P5）。
简短变量声明左边的变量可能并不是全部都是刚刚声明的。如果有一些已经在相同的词法域声明过了，那么简短变量声明语句对这些已经声明过的变量就只有赋值行为了。如果变量是在外部词法域声明的，那么简短变量声明语句将会在当前词法域重新声明一个新的变量。（[3]，CH2.3.1）

### 指针

Go语言提供了指针。&操作符可以返回一个变量的内存地址，并且`*`操作符可以获取指针指向的变量内容，但是在Go语言里没有指针运算，也就是不能像c语言里可以对指针进行加或减操作（[3],CH1.8）。

即使变量由表达式临时生成，那么表达式也必须能接受&取地址操作。（[3]，CH2.3.2）

指向struct的指针p，可以直接用`.`访问成员变量，如`p.value = 1`。

rp util.Hash 如果对传入的string进行修改，会影响原来的string对象吗？？？？？？？？？？？？（[1]，P108）

## 协程 goroutine

协程之间没有父子关系，一个协程只是与其它协程一起并行执行。唯一的例外是，main协程退出后，程序终止。
https://www.reddit.com/r/golang/comments/4kpv6x/why_a_parent_goroutine_doesnt_kill_its_child_and/

## slice

slice包含自身(go语言程序设计 P66)

### string与[]byte

string与[]byte转换会拷贝数据。go官方不建议不安全的拷贝，因为很多包依赖于string是不可变的，比如map。
strings.Builder使用了这种方式。
一种不安全的方式：
```go
// https://github.com/golang/go/issues/25484
// https://golang.org/src/strings/builder.go#L45
func ByteSlice2String(bs []byte) string {
	return *(*string)(unsafe.Pointer(&bs))
}

func UnsafeStringToBytes(str string) []byte {
	return *(*[]byte)(unsafe.Pointer(&str))
}
```

## debug数据结构

github.com/davecgh/go-spew/
https://blog.thinkeridea.com/201902/go/string_ye_shi_yin_yong_lei_xing.html

## 值还是引用

### 值保存方式

一般变量都是值保存方式，如int等。

数组、struct等也是值保存方式。

### 引用保存方式

map、slice切片、接口等都是引用保存方式。（[1]，P108，P113；[2] P66）
无法获取map的value的地址。

### 垃圾回收

Go语言没有析构函数。在函数中定义的Constructors and composite literals，其地址可以作为函数返回值。如果没有其它变量引用了，由运行时自动回收、释放。
Go语言规范中并没有提及Stack和Heap的概念。编译器可以判断是应该放在栈中，还是放到Heap中；或者，可能每次对复合变量取地址，都会生成一个新的实例？
参考：
https://golang.org/doc/effective_go.html#composite_literals
http://stackoverflow.com/questions/10866195/stack-vs-heap-allocation-of-structs-in-go-and-how-they-relate-to-garbage-collec

深入理解Go-垃圾回收机制
https://segmentfault.com/a/1190000020086769

go什么时候会产生heap对象，什么时候是stack对象？

## 切片Slice slice

切片是Go语言的基础概念（[3], P4）。
https://blog.golang.org/go-slices-usage-and-internals

### append

```go
t = s[:]
s = append(s, a)
```
之所以必须重新赋值，是因为如果直接修改s的array，那么t与s就不会共享底层数组，修改s的元素就不会影响t。违背语言承诺。

## 数据封装

Go没有`public private`等数据可见性的修饰关键字。要使某个符号对其它包可见，需要将该包定义为大写字母开头。Go语言中符号的可访问性是包一级，而不是类型一级的。（[2] P71）

## 程序结构

if、for等语句，条件等可以不必用括号封起来。当然，如果避免误输入，封起来也可以。

### if

Go语言允许在if语句的条件中先赋值，再进行条件判断，这样err的生命周期仅在if语句内（[3],CH1.7）。

```go
if err := r.ParseForm(); err != nil {
	log.Print(err)
}
```

### for 

for循环有多种形式（[3],CH1.2）。如果连condition也省略了，这就变成一个无限循环。initalization如果存在，必须是一条简单语句（simple statement），即，短变量声明、自增语句、赋值语句或函数调用。
```go
for initialization; condition; post {
    // zero or more statements
}
for condition {
    // ...
}
for {
    // ...
}
for _, arg := range os.Args[1:] {
}
```

### switch

Go语言并不需要显式地在每一个case后写break，语言默认执行完case后的逻辑语句会自动退出（[3],CH1.8）。但是可以使用fallthrough强制执行后面的case代码。
Go语言里的switch还可以不带操作对象（译注：switch不带操作对象时默认用true值代替，然后将每个case的表达式和true值进行比较）；可以直接罗列多种条件，像其它语言里面的多个if else一样，下面是一个例子：
```go
func Signum(x int) int {
	switch {
	case x > 0:
		return +1
	default:
		return 0
	case x < 0:
		return -1
	}
}
```
switch也可以紧跟一个简短的变量声明，一个自增表达式、赋值语句，或者一个函数调用。

## FileWatcher

https://github.com/fsnotify/fsnotify

## 堆栈分析stack

https://mp.weixin.qq.com/s/SGbiWLW6pLfd32TKIil10A

## 常用包

### 字符串操作

字符串是不可变的。(CH3.5, 3.5.4)
```go
strings.Join //连接字符串（[3],CH1.2）
```

### I/O

```go
bufio.NewScanner //按行扫描。可以通过Split函数指定分隔方法（[3],CH1.2）。
fmt //格式化io（[P3, 7],CH1.2）
fmt.Printf("%d\t%s\n", n, line)
io.Copy
ioutil.Discard //类似/dev/null
```

### random

程序启动时设置seed: 
```go
rand.Seed(time.Now().UnixNano())  // or time.Nanoseconds()
```
https://flaviocopes.com/go-random/
https://stackoverflow.com/questions/12321133/

shuffle
https://golang.org/pkg/math/rand/#Shuffle
https://studygolang.com/articles/14058

### time

https://studygolang.com/topics/2192
时间包
https://github.com/jinzhu/now

## 代码格式

Go语言不需要在语句或者声明的末尾添加分号，除非一行上有多条语句。（[3], P3）
实际上，编译器会主动把特定符号后的换行符转换为分号, 因此换行符添加的位置会影响Go代码的正确解析（[3], P3）。
举个例子, 函数的左括号`{`必须和`func`函数声明在同一行上, 且位于末尾，不能独占一行。




用缩进表示代码结构？？？

函数的多个返回值。

## 类与面向对象

Go不支持继承，没有类的层次结构。甚至没有“类”，通过Composition组合来构造复杂对象（[3] Preface, xv）。

### struct embedding 结构体嵌入

embedding struct也会实现embeded struct同接口的方法，其实就是直接调用embeded struct同接口的方法。
```go
package main

import "fmt"

type Point struct{ X, Y int }

func (p *Point) print() int {
  return 123;
}

type Circle struct {
    Point
    Radius int
}

func main() {
    var c Circle
    fmt.Printf("%#v\n", c)
}
```
运行如下命令：
```
go tool compile test.go
go tool objdump test.o > test.dump
```
查看test.dump，发现`(*Circle).print`中`R_CALL:%22%22.(*Point).print`

interface与method的语法不同，method需要显式指定接收者，interface不需要。但是如果一个类型声明了interface要求的方法，就实现了该interface。
方法的指针和非指针基本可以混用。综上所述，指针可以实现接口。

golang struct embedding interface
https://stackoverflow.com/questions/38043678/golang-embedded-interface-on-parent-struct
https://travix.io/type-embedding-in-go-ba40dd4264df?gi=4276c887d66b
https://studygolang.com/articles/385
http://xargin.com/go-and-interface/

方法不仅可以定义在结构体上, 而且, 可以定义在任何用户自定义的类型上；并且, 具体类型和抽象类型（接口）之间的关系是隐式的，所以很多类型的设计者可能并不知道该类型到底实现了哪些接口。

### struct在某种情况下可以部分实现多态

override Base不会报错。
```go
package main

import "fmt"

type Base struct {
	b string
}

func (b *Base) base() {
	fmt.Printf("base: %v\n", b.b)
}

type Derived struct {
	Base
	d string
}

func (d *Derived) base() {
	d.Base.base()
	fmt.Printf("base in derived: %v\n", d.d)
}

func (d *Derived) derived() {
	d.base()
	fmt.Printf("derived: %v\n", d.d)
}

func main()  {
	d := Derived{Base: Base{b: "b"}, d: "d"}
	// 前提是，调用的是子类方法，在子类方法内可以调用子类override的方法
	d.derived()
}
```

但是如果子类对象调用base类的方法，就不能按多态运行了。
```go
package main

import "fmt"

type Base struct {
	b string
}

func (b *Base) base() {
	fmt.Printf("base: %v\n", b.b)
	b.polymorphism()
}

func (b *Base) polymorphism() {
	fmt.Println("polymorphism in base")
}

type Derived struct {
	Base
	d string
}

func (d *Derived) polymorphism() {
	fmt.Println("polymorphism in derived")
}

func main()  {
	d := Derived{Base: Base{b: "b"}, d: "d"}
	// 实际是 d.Base.base() , 所以在base()内部调用Base的方法。因为go是静态类型语言。
	d.base()
}
```

即使开始调用子类方法，但中间调用了父类方法m，m需要调用多态方法p，也不能实现多态。
```go
package main

import "fmt"

type Base struct {
	b string
}

func (b *Base) base() {
	fmt.Printf("base: %v\n", b.b)
	b.polymorphism()
}

func (b *Base) polymorphism() {
	fmt.Println("polymorphism in base")
}

type Derived struct {
	Base
	d string
}

func (d *Derived) derived() {
	d.base()
	fmt.Printf("derived: %v\n", d.d)
}

func (d *Derived) polymorphism() {
	fmt.Println("polymorphism in derived")
}

func main()  {
	d := Derived{Base: Base{b: "b"}, d: "d"}
	// 中间调用了父类方法，也不能多态
	d.derived()
}
```


### struct+method不能实现多态

Go也不直接支持多态。 https://hackthology.com/golangzhong-de-mian-xiang-dui-xiang-ji-cheng.html
单纯用struct和method也无法实现多态。示例如下。因为go是静态类型语言，struct的method是静态实现，无法更改。要修改方法实现只能通过interface。

让Go interface实现多态的一个技巧是，对于需要多态的方法，用一个函数变量表示。每个类型在New新对象时，设置这个多态的函数变量。
这种多态技巧，可以延后到需要时再使用。只要控制好所有实例都用New模式生成，也就是说，struct类型小写，不export。
```go
// https://stackoverflow.com/questions/43147973/
type MyStruct struct {
    id int
}

func (ms *MyStruct) PrintHello() {
    fmt.Printf("Hello from original method %v", ms.id)
}

func main() {
    fmt.Println("Hello, playground")
    m := MyStruct{}
    m.PrintHello()

    // 如果一个类型想复用MyStruct，但需要override PrintHello方法。用struct不可行。
    // 编译报错 cannot assign to m.PrintHello
    m.PrintHello = func() {fmt.Printf("Hello from newer method 2")}
}
```

### struct + func variable可以实现多态

https://stackoverflow.com/questions/43147973/
https://hackthology.com/golangzhong-de-mian-xiang-dui-xiang-ji-cheng.html
但是不同的struct仍是不同类型。只有interface才能实现类似继承的机制。
如果有多态行为，保险的方式是直接用函数变量实现。否则在父类、子类之间来回跳，很容易出问题。

## 包管理

Go语言的代码通过包（package）组织，包类似于其它语言里的库（libraries）或者模块（modules）。（[3], P2）

Go的包可以有目录层次。

代码开始的`package main`表示该文件属于哪个包。

`import`声明导入需要的包。但**如果导入了不需要的包**，程序也会无法编译通过。（[3], P3）
import声明必须跟在文件的package声明之后。

`main`包比较特殊。它定义了一个独立可执行的程序，而不是一个库。
在main包里的 main 函数也很特殊，它是整个程序执行时的入口。（[3], P2）

包导入顺序并不重要；gofmt工具格式化时按照字母顺序对包名排序（[3], P4）。

包导入有两种形式：
```go
import (
	"fmt"
	"os"
)
```

或
```go
import fmt
import os
```

按照惯例，我们在每个包的包声明前添加注释（[3], P5）。

### import的其它技巧

https://www.cnblogs.com/baiyuxiong/p/4440581.html
https://studygolang.com/articles/4356

### go mod

Go Modules 终极入门
https://mp.weixin.qq.com/s/zo7zmEVXvxgr80n6H_49Mg

一个module是相关Go package的一个集合。modules是源码交互和版本控制的单元。
http://www.huamo.online/2019/07/08/Golang-Modules学习/

https://github.com/golang/go/wiki/Modules
go mod init .
go mod edit -require github.com/wilk/uuid@0.0.1
go get github.com/wilk/uuid
go mod download
go mod vendor

`go.mod`文件一旦创建后，它的内容将会被go toolchain全面掌控。go toolchain会在各类命令执行时，比如go get、go build、go mod等修改和维护go.mod文件。
https://juejin.im/post/5c8e503a6fb9a070d878184a

`go.mod`文件中的`module`属性，确定了当前项目的base package路径。通常这个路径与git路径一致。

go 1.13
https://mp.weixin.qq.com/s/SGGV3tWEg5AAJ7I_FcK0cg

```
# go >=1.13 需要配置
GOPRIVATE="code.xxx.cn"
```

## interface 接口

https://github.com/astaxie/build-web-application-with-golang/blob/master/zh/02.6.md
https://sanyuesha.com/2017/07/22/how-to-understand-go-interface/
https://juejin.im/post/5a6873fd518825734501b3c5
https://www.ardanlabs.com/blog/2017/07/interface-semantics.html
go语言传递struct, pointer和interface的差异
https://www.jianshu.com/p/42762865c2d8

### 空接口

https://newt0n.github.io/2016/11/07/如何避开-Go-中的各种陷阱/
https://golang.org/doc/faq#nil_error

## 错误处理 error

https://www.flysnow.org/2019/09/06/go1.13-error-wrapping.html
https://github.com/pkg/errors
https://godoc.org/github.com/pkg/errors

## go设计模式 pattern

https://github.com/tmrts/go-patterns
http://tmrts.com/go-patterns

## sleep tick

个人感觉，如果不是对定时要求特别精确，用time.Sleep比较简单；如果需要灵活模式控制，tick更适合。

详细讲解：
https://blog.csdn.net/Star_CSU/article/details/86650684
https://stackoverflow.com/questions/32147421/behavior-of-sleep-and-select-in-go
https://www.qingtingip.com/h_235506.html
https://xargin.com/go-timer/  这里有代码详细讲解
https://github.com/cch123/golang-notes

## go并发控制

WaitGroup/context
https://www.flysnow.org/2017/05/12/go-in-action-go-context.html

并发模型-pipeline
https://zhuanlan.zhihu.com/p/59295820

map不是协程安全的
https://www.jianshu.com/p/f2e7650da938

在 Go 語言內管理 Concurrency 的三種方式
https://blog.wu-boy.com/2020/08/three-ways-to-manage-concurrency-in-go/

### context

https://juejin.im/post/5a6873fef265da3e317e55b6
https://blog.golang.org/context
https://zhuanlan.zhihu.com/p/34417106
https://draveness.me/golang-context
https://zhuanlan.zhihu.com/p/60180409?utm_source=wechat_session&utm_medium=social&s_r=0

晓辉
https://github.com/grpc/grpc-go/blob/master/metadata/metadata.go
https://github.com/grpc/grpc-go/blob/master/clientconn.go#L213

## sync

### atomic

https://blog.betacat.io/post/golang-atomic-value-exploration/
https://skyao.io/learning-go/stdlib/sync/atomic.html
https://golang.org/pkg/sync/atomic/#Value

Value一经使用就不能拷贝(可传递指针)。拷贝+Load也不行(无法反映Store数据更新)。
https://studygolang.com/articles/17972

闭包是引用传递环境变量
```go
package main

import (
    "fmt"
	"time"
	"sync/atomic"
)

func main() {
    var i int = 0
	var value atomic.Value
	value.Store(i)
	fmt.Println(value.Load())
	go func() {
		value.Store(1)
	}()
	time.Sleep(5000)
	fmt.Println(value.Load())
}
```

### map

目前只适用个别场景
https://colobu.com/2017/07/11/dive-into-sync-Map/

## defer/panic/recover

panic如果没有被recover，错误信息会输出到stderr。
对于grpc等server，通常每个请求一个goroutine。handler panic后，server不会崩溃。应该是server在创建goroutine是增加了defer/recover处理。thor会把panic错误存储到日志文件。
但是，如果handler自己创建了goroutine并在其中panic而没有recover，整个server会崩溃。这个错误信息如果没有特殊处理，不会保存到日志文件。

java中的线程也没有父子关系。
https://stackoverflow.com/questions/19988092/does-a-child-thread-in-java-prevent-the-parent-threads-to-terminate

Recover is only useful inside deferred functions.
https://blog.golang.org/defer-panic-and-recover
https://golang.org/ref/spec#Handling_panics
https://ieevee.com/tech/2017/11/23/go-panic.html
https://sanyuesha.com/2017/07/23/go-defer/
https://deepzz.com/post/how-to-use-defer-in-golang.html

谈谈 panic 和 recover 的原理
https://draveness.me/golang-panic-recover

## json

https://bingohuang.com/go-json/
https://www.sohamkamani.com/blog/2017/10/18/parsing-json-in-golang/
Ignoring JSON fields, Converting JSON field types
https://flaviocopes.com/go-json/
go是一个强类型语言，所以不能像Python那样把string直接解析为一个map与slice相结合的组合对象。

Unmarshal可以自动把首字母转为大写(仅限于首字母)，但Marshal不会对字段名做这种转换。两个方法都**不会**把下划线分隔的名字转为驼峰风格、并转换每部分的首字母为大写。

### 动态json库 dynamic json

https://github.com/Jeffail/gabs

## yaml

https://github.com/go-yaml/yaml
https://www.jianshu.com/p/84499381a7da

## stacl trace

https://golang.org/pkg/runtime/debug/#Stack
https://github.com/palantir/stacktrace

## flag

http://blog.studygolang.com/2013/02/标准库-命令行参数解析flag/
https://books.studygolang.com/The-Golang-Standard-Library-by-Example/chapter13/13.1.html

## sort

https://books.studygolang.com/The-Golang-Standard-Library-by-Example/chapter03/03.1.html

## godoc document 文档

https://blog.golang.org/godoc-documenting-go-code

## 运行、编译

`go run my.go`
这个命令编译一个或多个以.go结尾的源文件，链接库文件，并运行最终生成的可执行文件。但该可执行文件不会保存下来。

`go build my.go`
这个命令生成并保存可执行的二进制文件。

### 交叉编译

```shell
set GOARCH=amd64
set GOOS=linux
set GOOS=windows
```
https://golang.org/doc/install/source#environment
https://www.cnblogs.com/oxspirt/p/7072818.htmlGOOS=windows

### go build 汇编

https://mp.weixin.qq.com/s/mlkpqz5TRCiGrRs35WUNRQ

## 命令行参数

os.Args变量是一个字符串（string）的切片（[3], CH1.2, P4）。
os.Args的第一个元素，os.Args[0], 是命令本身的名字；其它的元素则是程序启动时传给它的参数。

## database sql

### Using Prepared Statements

http://go-database-sql.org/prepared.html
https://www.jianshu.com/p/ee0d2e7bef54

## set

empty interface
https://github.com/deckarep/golang-set
https://godoc.org/github.com/deckarep/golang-set

int and string
https://github.com/scylladb/go-set
这个实现使用具体类型、而不是空接口。但是需要struct、且不能使用链式调用。
method chaining在go中需要谨慎使用，因为惯用法是返回error。只是set这种操作，一般不涉及error。

## ide

https://github.com/golang/go/wiki/IDEsAndTextEditorPlugins
Jetbrains GoLand
Visual Studio Code
LiteIDE
IntelliJ IDEA
Sublime Text + Go Sublime
Vim with vim-go plugin

### VS Code/VsCode

VS Code 快速查看 Golang 接口
https://mp.weixin.qq.com/s/3wlwuA2sAm-sWSilBJptiA

## 隐晦操作符

https://www.gitdig.com/go-operators/

## 反射reflect

https://draveness.me/golang/basic/golang-reflect.html
Go反射的三个原则
https://sevenyu.top/2019/12/21/laws-of-reflection.html

## go test

运行当前目录和所有子目录的测试用例：`go test ./...`
https://stackoverflow.com/questions/16353016/how-to-go-test-all-testings-in-my-project

assert
https://github.com/benbjohnson/testing

Go语言测试进阶版建议与技巧
https://pengrl.com/p/32101/
Go编译时会忽略testdata目录。

go test会运行package的init函数。

## 最佳实践

High Performance Go Workshop
https://dave.cheney.net/high-performance-go-workshop/dotgo-paris.html

https://talks.golang.org/2013/bestpractices.slide
https://golang.org/doc/effective_go.html
https://github.com/llitfkitfk/go-best-practice
https://dave.cheney.net/practical-go/presentations/qcon-china.html
https://talks.golang.org/2013/bestpractices.slide#1
https://colobu.com/2017/06/30/idiomatic-go-references/
Golang 新手可能会踩的 50 个坑
https://segmentfault.com/a/1190000013739000
Go语言常见坑
https://chai2010.cn/advanced-go-programming-book/appendix/appendix-a-trap.html

https://github.com/hoanhan101/ultimate-go

## Graceful Restart in Golang

https://grisha.org/blog/2014/06/03/graceful-restart-in-golang/
https://github.com/fvbock/endless
https://colobu.com/2015/10/09/Linux-Signals/

## 开源项目 open source

https://github.com/hashicorp/consul

### db

https://github.com/pingcap/tidb
https://github.com/etcd-io/etcd

### message

https://github.com/nsqio/nsq

### web

https://github.com/gin-gonic/gin
https://github.com/astaxie/beego

## go代码检查

```shell
go tool vet -shadow main.go
go-nyet main.go
```
https://segmentfault.com/a/1190000013739000

## cgo

https://chai2010.cn/advanced-go-programming-book/ch2-cgo/ch2-02-basic.html

### Go 开发者路线图

https://github.com/Alikhll/golang-developer-roadmap/blob/master/i18n/ReadMe-zh-CN.md

## 内存管理

https://tonybai.com/2020/03/10/visualizing-memory-management-in-golang/

## 问题

struct与class的区别是什么？有class关键字吗？
如果协程、channel太多，是否会影响内存？多大量级会影响？

如何实现抽象类。比如A、B两个类有一些共用的逻辑，可以放到一个共同的类C中？
https://blog.csdn.net/neosmith/article/details/84456060
https://zhuanlan.zhihu.com/p/33759346

go程序设计语言 ch6.6 P130 倒数第3段：同一个类型的方法，只能定义在一个包内吗？

ch := make(chan string) //通过channel传递string时，拷贝的是引用，还是value？map呢？struct呢？

go不支持overload，为什么make可以省略cap参数？

## 待研究

如果要实现const slice或类似功能，有哪些方式？

### append slice

`s = append(slice, c)`为何要重新赋值给s，而不是修改s的指针字段？

### Go内存分配

https://mp.weixin.qq.com/s/3gGbJaeuvx4klqcv34hmmw

### GC

gc日志格式
https://segmentfault.com/a/1190000020255157

方圆
https://zhuanlan.zhihu.com/p/77943973

https://www.ardanlabs.com/blog/2019/05/garbage-collection-in-go-part2-gctraces.html?from=singlemessage
https://mp.weixin.qq.com/s/eDd212DhjIRGpytBkgfzAg

Go GC 20问 https://mp.weixin.qq.com/s/o2oMMh0PF5ZSoYD0XOBY2Q

### GOMAXPROCS

https://colobu.com/2017/10/11/interesting-things-about-GOMAXPROCS/
https://juejin.im/post/5b7678f451882533110e8948

### 控制goroutine数量

https://github.com/panjf2000/ants
https://github.com/Jeffail/tunny
https://gocn.vip/article/1541

https://gocn.vip/article/1541
https://www.zhihu.com/question/48507569
https://www.jianshu.com/p/42e89de33065

goroutine stack size
每个goroutine占用内存大小，由哪些成分构成？

goroutine pool
https://github.com/panjf2000/ants
https://github.com/Jeffail/tunny
https://github.com/ivpusic/grpool
https://github.com/valyala/fasthttp/blob/master/workerpool.go

### main函数终结

P171 main函数返回后，所有goroutine都暴力终结。redis/mysql等连接会正常close吗？从服务端看呢？os会发terminate包？

### debug

https://github.com/go-delve/delve
https://www.cnblogs.com/li-peng/p/8522592.html

### pprof

我是如何在大型代码库上使用 pprof 调查 Go 中的内存泄漏
https://juejin.im/post/5ce11d1ee51d4510601117fd
https://mp.weixin.qq.com/s/B8lJI_2BfMcz-Rd1bNjkyg
https://blog.golang.org/profiling-go-programs

对比两个时间点的heap，发现内存泄漏
https://colobu.com/2019/08/20/use-pprof-to-compare-go-memory-usage/

### trace

https://juejin.im/post/5d27400151882530af139a85

### 限流器limitRate rateLimit

https://www.cyhone.com/articles/analysis-of-uber-go-ratelimit/

### immutability

slice, map可否声明为不可变的？

## docker

最精简的docker镜像
https://mp.weixin.qq.com/s/TKt4AViDeP7cWjGym5oKyg

## 参考资料

[1] Go语言程序设计
[2] Go语言编程，许式伟
[3] The Go Programming Language （https://docs.ruanjiadeng.com/gopl-zh/ch1/ch1-01.html）
