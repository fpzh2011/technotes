#Linux与CPU亲和性affinity

## CPU affinity

	http://www.ibm.com/developerworks/cn/linux/l-affinity.html
	https://en.wikipedia.org/wiki/Processor_affinity
	www.glennklockwood.com/hpc-howtos/process-affinity.html
	这篇介绍了soft/hard affinity： m.linuxjournal.com/article/6799
	www.gnu.org/software/libc/manual/html_node/CPU-Affinity.html

## taskset

taskset用于检索或设置一个进程的CPU affinity。
如果指定`-p`参数，就查询一个正在运行的进程的affinity；否则，就根据后面的参数启动一个进程，taskset命令返回后，新进程已经设置为指定的affinity。

参数解释：

	-p 后面跟着pid参数，如 -p 30773 。针对正在运行的pid为30773的进程，查询当前的affinity。
	-c 以list方式列出指定的core，如“0,1,2”，也支持“0-2,4”这种形式。查询和设置都可以用。

示例：

`taskset 2 top` 指定用第2个核运行top。因为2的二进制是10，所以是第2个核。

`taskset -c 1 top` 指定用第2个核运行top。因为`-c`是从0开始计数，1就代表第2个核。

`taskset -p 30773`：显示pid为30773的进程的当前affinity mask。如结果为3，二进制表示为011，即占用了core0和core1。

`taskset -cp 30773`：显示pid为30773的进程的当前affinity list。如结果为 “0,1”，则表示占用了core0和core1。

`taskset -p -c 1 30773`：将pid为30773的进程的affinity设置为1，即只在core1上运行。

**问题：参数a是设置所有的线程。如果不设置参数a呢？**

## numactl

numactl可以进行更精确、更灵活的CPU绑定。它可以识别CPU socket的拓扑、core与socket的对应关系。

## 查看中断处理在CPU上的分配

`cat /proc/interrupts`

www.vpsee.com/2010/07/load-balancing-with-irq-smp-affinity/

