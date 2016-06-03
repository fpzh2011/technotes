# autoconf、automake详解

## autoconf

autoconf自动配置软件源代码包（software source code packages），以适应各种类型的类Posix系统。autoconf利用M4宏处理，生成自包含的shell脚本。

autoconf生成的shell脚本的执行不需要人工介入，甚至不需要参数确定系统类型，而是逐个测试软件包依赖的feature是否存在。在每个check前，会打印将要执行什么样的check。

每个使用autoconf的软件包（software package）需要定制一个模版文件，用于列出所需要的所有features。autoconf根据这个模版文件生成shell脚本。



## 参考资料

	autoconf官方文档
		Introduction
	automake官方文档

