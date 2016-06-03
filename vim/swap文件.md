# vim的swap文件

Vim会在你连续4秒不键入内容时，或者是连续键入了200个字符之后，跟磁盘同步一次。
这可以通过`updatetime`和`updatecount`两个选项来控制。

swp文件产生原因分为：
* 当你用多个程序编辑同一个文件时。
* 非常规退出时。

第一种情况的话，为了避免同一个文件产生两个不同的版本（vim中的原话），还是建议选择readonly为好。

第二种情况的话，可以用`vim -r filename`恢复，然后再把swp文件删除（rm .filename.swp）。

可以通过选项禁止产生swp文件：`set noswapfile`

http://www.cnblogs.com/beauty/archive/2010/07/27/1786009.html
http://blog.csdn.net/lingdxuyan/article/details/4993868
http://vimdoc.sourceforge.net/htmldoc/recover.html
