# make基础说明

## 为什么需要make

参考 http://www.zhihu.com/question/27455963

## 示例与基本说明

makefile的基本格式为：

	target ... : prerequisites ...
		command

或

	target ... : prerequisites ... ; command 
		command

一个或多个target文件依赖于prerequisites中的文件，其生成规则定义在command中。
**prerequisites中如果有一个以上的文件比target文件要新的话，command所定义的命令就会被执行。**这就是Makefile的规则。也就是Makefile中最核心的内容。

target和prerequisites是文件名，以空格分隔。

第一个target是最终要生成的文件，也是依赖关系的根。如果第一条规则中有多个target文件名，第一个target是最终目标，make所完成的也是这个target。

command就是普通的shell命令，该行一定要以一个Tab键作为开头。

下面的例子来自官方手册（编译生成可执行文件edit）：

	edit : main.o kbd.o command.o display.o \
			insert.o search.o files.o utils.o
		cc -o edit main.o kbd.o command.o display.o \
			insert.o search.o files.o utils.o

	main.o : main.c defs.h
		cc -c main.c
	kbd.o : kbd.c defs.h command.h
		cc -c kbd.c
	command.o : command.c defs.h command.h
		cc -c command.c
	display.o : display.c defs.h buffer.h
		cc -c display.c
	insert.o : insert.c defs.h buffer.h
		cc -c insert.c
	search.o : search.c defs.h buffer.h
		cc -c search.c
	files.o : files.c defs.h buffer.h command.h
		cc -c files.c
	utils.o : utils.c defs.h
		cc -c utils.c
	clean :
		rm edit main.o kbd.o command.o display.o \
			insert.o search.o files.o utils.o

## make是如何工作的

make并不管command命令是怎么工作的，他只管执行所定义的命令。在默认的方式下，也就是我们只输入make命令。那么：

	1 make会在当前目录下找名字叫“Makefile”或“makefile”的文件。
	
	2 如果找到，它会找文件中的第一个目标文件（target），在上面的例子中，他会找到“edit”这个文件，并把这个文件作为最终的目标文件。
	
	3 如果edit文件不存在，或是edit所依赖的后面的 .o 文件的文件修改时间要比edit这个文件新，那么，他就会执行后面所定义的命令来生成edit这个文件。
	
	4 如果edit所依赖的.o文件也存在，那么make会在当前文件中找目标为.o文件的依赖性，如果找到则再根据那一个规则生成.o文件。（这有点像一个堆栈的过程）
	
	5 当然，你的C文件和H文件是存在的啦，于是make会生成 .o 文件，然后再用 .o 文件生命make的终极任务，也就是执行文件edit了。

这就是整个make的依赖性，make会一层又一层地分析文件的依赖关系，直到最终编译出第一个target文件。
在找寻的过程中，如果出现错误，比如最后被依赖的文件找不到，那么make就会直接退出，并报错。make只关注文件的依赖性；而对于所定义的命令的错误，或是编译不成功，make根本不理。

通过上述分析，我们知道，像clean这种，没有被第一个目标文件直接或间接关联（clean的冒号后面也没有定义依赖关系），那么clean所定义的命令将不会被自动执行。
不过，我们可以`make clean`显示执行clean。

## make工作时的执行步骤

1、读入所有的Makefile。

2、读入被include的其它Makefile。

3、初始化文件中的变量。

4、推导隐晦规则，并分析所有规则。

5、为所有的目标文件创建依赖关系链。

6、根据依赖关系，决定哪些目标要重新生成。

7、执行生成命令。

## makefile变量

上面的例子中，8个.o文件出现了3次。使用变量的形式为：

	objects = main.o kbd.o command.o display.o /
			insert.o search.o files.o utils.o

	edit : $(objects)
			cc -o edit $(objects)
	...
	clean :
			rm edit $(objects)

## 自动推导 auto deduce

GNU的make可以自动推导文件以及文件依赖关系后面的命令。
只要make看到一个[.o]文件，它就会自动的把[.c]文件加在依赖关系中。并且 cc 命令也会被推导出来。

	main.o : main.c defs.h
		cc -c main.c

可以精简为 `main.o : defs.h`

## 伪target

.PHONY意思表示clean是一个“伪目标”，即这个target文件可以不存在，可以不考虑其依赖关系。示例如下（-rm中的'-'表示，命令失败也不退出make的执行）：

	.PHONY : clean
	clean :
		-rm edit $(objects)

由于伪target不是文件，所以make无法自动生成它的依赖关系和决定它是否要执行。

## Makefile里有什么？

Makefile里主要包含了五部分内容：显式规则、隐晦规则、变量定义、文件指示和注释。

1、显式规则。显式规则说明了，如何生成一个或多的的目标文件。这是由Makefile的书写者明显指出，要生成的文件，文件的依赖文件，生成的命令。

2、隐晦规则。由于我们的make有自动推导的功能，所以隐晦的规则可以让我们比较粗糙地简略地书写Makefile，这是由make所支持的。

3、变量的定义。在Makefile中我们要定义一系列的变量，变量一般都是字符串，这个有点你C语言中的宏，当Makefile被执行时，其中的变量都会被扩展到相应的引用位置上。

4、文件指示。其包括了三个部分，一个是在一个Makefile中引用另一个Makefile，就像C语言中的include一样；另一个是指根据某些情况指定Makefile中的有效部分，就像C语言中的预编译#if一样；还有就是定义一个多行的命令。有关这一部分的内容，我会在后续的部分中讲述。

5、注释。Makefile中只有行注释，和UNIX的Shell脚本一样，其注释是用“#”字符，这个就像C/C++中的“//”一样。如果你要在你的Makefile中使用“#”字符，可以用反斜框进行转义，如：“/#”。

## 文件搜索

默认的情况下，make命令会在当前目录下按顺序找寻文件名为`GNUmakefile`、`makefile`、`Makefile`的文件。
如果要指定特定的Makefile，你可以使用make的 `-f` 和 `--file` 参数，如`make -f Make.Linux` 或 `make --file Make.AIX`

变量VPATH可以设置文件搜索路径：`VPATH = src:../headers`

## include

在Makefile使用include关键字可以把别的Makefile包含进来，被包含的文件会原模原样的放在当前文件的包含位置。include的语法是：`include filename`

filename可以包含路径和通配符，也可以包括变量。如 `include foo.make *.mk $(bar)`

## 嵌套执行make

在一些大的工程中，我们会把我们不同模块或是不同功能的源文件放在不同的目录中，我们可以在每个目录中都书写一个该目录的Makefile，这有利于让我们的Makefile变得更加地简洁。
例如，我们有一个子目录叫subdir，这个目录下有个Makefile文件，来指明了这个目录下文件的编译规则。那么我们总控的Makefile可以这样书写：

	subsystem:
		cd subdir && $(MAKE)

其等价于：

	subsystem:
		$(MAKE) -C subdir

这两个例子的意思都是先进入“subdir”目录，然后执行make命令。

总控Makefile的变量可以传递到下级的Makefile中（如果你显示的声明），但是不会覆盖下层的Makefile中所定义的变量，除非指定了“-e”参数。

如果你要传递变量到下级Makefile中，那么你可以使用这样的声明：`export <variable ...>`

如果你不想让某些变量传递到下级Makefile中，那么你可以这样声明：`unexport <variable ...>`

## 其他

### 控制是否显示执行的命令

通常，make会把其要执行的命令行在命令执行前输出到屏幕上。当我们用“@”字符在命令行前，那么，这个命令将不被make显示出来。如`@echo 正在编译XXX模块......`

如果make执行时，带入make参数“-n”或“--just-print”，那么其只是显示命令，但不会执行命令.这个功能很有利于调试Makefile，看看我们书写的命令执行起来是什么样子。

而make参数“-s”或“--slient”则是全面禁止命令的显示。

### 连续的相关命令

如果你要让上一条命令的结果应用在下一条命令时，你应该使用分号分隔这两条命令。比如你的第一条命令是cd命令，你希望第二条命令得在cd之后的基础上运行，那么你就不能把这两条命令写在两行上，而应该把这两条命令写在一行上，用分号分隔。
如 `cd /home/hchen; pwd`

### 命令出错处理

忽略命令的出错，我们可以在Makefile的命令行前加一个减号“-”（在Tab键之后），标记为不管命令出不出错都认为是成功的。如 `-mkdir mydir/sub`

还有一个全局的办法是，给make加上“-i”或是“--ignore-errors”参数，那么，Makefile中所有命令都会忽略错误。

### 定义命令包

如果Makefile中出现一些相同命令序列，那么我们可以为这些相同的命令序列定义一个变量。定义这种命令序列的语法以“define”开始，以“endef”结束，如：

	define run-yacc
	yacc $(firstword $^)
	mv y.tab.c $@
	endef

这里，“run-yacc”是这个命令包的名字，其不要和Makefile中的变量重名。在“define”和“endef”中的两行就是命令序列。使用命令包就好像使用变量一样。

	foo.c : foo.y
		$(run-yacc)

## 参考资料

	http://www.zhihu.com/question/27455963  简单直观的背景来源介绍
	http://blog.csdn.net/haoel/article/details/2886  陈皓的专栏系列
	http://www.gnu.org/software/make/  make官方网站


