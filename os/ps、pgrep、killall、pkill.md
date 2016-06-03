# ps、pgrep、killall、pkill等相关命令

## ps

常用选项

	-m 在进程后面输出线程信息。
	-o 按指定格式输出，如 -o pid,cmd,psr 。其中psr表示运行的cpu core。完整的格式可以参考 man ps 。

按线程显示
	ps -eLf

## pgrep

pgrep根据名字等属性查询进程。如 `pgrep top` 查询命令行名称带top的运行进程，包括top和rdesktop都会出来。

仅运行`pgrep top`将只列出pid。

常用选项

	-l 列出进程名称和pid，不含命令行参数。
	-a 列出完整的命令行信息，包括参数。
	-x 列出名称精确匹配的进程。如 pgrep -x top ，只列出top，不会列出rdesktop。

## pkill 

类似于pgrep。


