#Linux系统环境检测

## 测试环境

Ubuntu 14.04 LTS 32位

## CPU

运行 `lscpu` 查询CPU概况。如32/64位，字节序，CPU核个数，Socket插槽个数、每个座的核数、每个核的线程数，CPU频率、缓存等。

运行 `cat /proc/cpuinfo` 查询每个核的情况。其中，
processor表示：CPU个数*每个座的核数*线程数，从0开始计数。
physical id表示物理CPU的ID，从0开始计数。

## 硬盘

### 硬盘类型

运行 `df -h` 查看文件系统。其中 /dev/sdX就是scsi的，hdX就是普通的。

运行 `cat /sys/block/sda/queue/rotational` , 0是SSD，1是传统硬盘。

### 测试读写速度

`dd if=/dev/zero of=dd.file bs=8k count=128k conv=fdatasync`

`dd if=/dev/zero of=dd.file bs=1G count=1 conv=fdatasync`

上面两个命令分别测试了分别以每次8k和1g的大小，写入1g文件的速度。`conv=fdatasync`表示实际写盘，而不是写入Page Cache。（sync、fsync和fdatasync可以参考apue）

硬盘读速度的测试同理，不过要先清理缓存，否则直接从Page Cache读了。

`sh -c "sync && echo 3 > /proc/sys/vm/drop_caches"`

`dd if=./dd.file of=/dev/null bs=8k count=128k`

其中，sync命令将cache中修改的数据写回磁盘，以免清除cache丢失数据。

## 网卡

查看网卡速度：`ethtool eth0`

查看网卡绑定：`cat /proc/net/bonding/bond0`。留意其Bonding Mode是负载均衡的，再留意其捆绑的网卡的速度。
网卡绑定可以把多块网卡绑起来，突破单块网卡的带宽限制。

使用ping或traceroute、qperf检查网络延时（ubuntu好像没有qperf，rhel有）。使用`iperf`测试网络吞吐。

## 参考资料

	http://calvin1978.blogcn.com/articles/perf-tunning-1.html
	http://www.xuebuyuan.com/2125675.html
	http://www.linuxidc.com/Linux/2010-03/24939.htm
	http://blog.csdn.net/wyzxg/article/details/7279986/
	http://weiweinews.blog.51cto.com/1188066/717287


