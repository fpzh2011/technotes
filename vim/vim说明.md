# vim说明

c/c++: ctags, TagList, Nerd_Tree, cscope
http://my.oschina.net/kutengshe/blog/464602
http://blog.sina.com.cn/s/blog_6c090c5d0101c2nv.html
http://easwy.com/blog/archives/advanced-vim-skills-use-ctags-tag-file/


在rhel7下，如果使用 `set mouse=a` 启动对鼠标的支持，用鼠标选择一块文本后，反而不能复制。
解决方法：仍然设置mouse=a，选中后用y复制，用鼠标中键粘贴。还有一种方法说用vim.gnome替换vim，但没有找到vim.gnome。

编辑另一个文件 `:edit foo.txt`
缓冲区列表 `:buffers` 或 `ls`
切换缓冲区 `:b1` ，后面的数字表示ls列出的缓冲区编号

切换到shell：ctrl+z 。 切换回vim：fg 命令。
或者用`:shell`切换到shell，用`exit`切换回vim。

`vim file1 file2`可以打开多个文件，但只有一个窗口，第2-N个文件在缓冲区。

`vim -o file1 file2`可以用多个窗口（不是标签）打开多个文件，窗口上下排列。`-O`是左右排列窗口。

标签管理
	shell命令行运行`vim -p test1 test2`可以将不同文件在不同标签窗口打开。
	:tabnew f2.c  创建新标签
	:tabp 前一个标签
	:tabn 后一个标签
	:tabc 关闭当前标签
	:tabo 关闭其它所有标签
	gt 可以直接在前后标签之间切换

`:Ex`可以显示目录浏览器，并点击目录进入、或者点击文件并打开。`:Sex`可以在新窗口显示目录浏览器，并点击目录进入、或者点击文件并打开。

窗口分隔：

	:split 对同一个文件，横向（上下）切割窗口。可以同时浏览/编辑一个文件的不同部分。
	:new test4 新建一个横向（上下）窗口。
	:vsplit 对同一个文件，纵向（左右）切割窗口。可以同时浏览/编辑一个文件的不同部分。
	:vsplit filename 打开新文件，并纵向（左右）切割窗口。

`:only`只保留当前窗口（不是标签），关闭其它窗口。vim中，多个标签是在一个窗口内的。

窗口切换：`ctrl+w+j/k`

pwd 显示当前目录（不用带!）

ctrl+G 显示当前文件

cd 切换vim的工作目录（:cd 不起作用）

w [filename] 保存，或另存为。注意，另存为之后，当前编辑的缓冲区文件并没有切换，需要用e命令手工切换。

wall 保存所有窗口（标签）的内容。

qa qall 退出所有窗口（标签）。但这样vim也退出了。

u 撤销单次修改。U 撤销对整行的修改。
ctrl+R 重做撤销的操作

v visual，可视命令。可以用键盘或鼠标选择文本区域。

y 抽出 （yank）命令可以把文字拷贝到寄存器中。然后用p命令粘贴到别处。但是仅先于在vim中使用，无法将文本粘贴到其他编辑器。

p 粘贴，put。删除的行也可以粘贴。

2dd	删除2行。
dw	删除一个单词
d2w	删除2个单词
de	删除从光标到单词末尾的内容
d$	删除从光标到行末的内容

r 替换一个字符；R 连续替换多个字符。

c 修改内容。比如，c2w 删除2个单词，并进入插入模式。

J 连接两行为一行。下一行拼到当前行后面。

* 查找与“光标所在单词”相同的单词，并高亮显示。相当于notepad++的“双击全选单词”。查看代码时特别有用。

查找
	要查找单词 ignore 可在正常模式下输入 /ignore <回车>。
	要重复查找该词，可以重复按 n 键。
	忽略大小写（Ignore Case）：请输入：:set ic
	禁用忽略大小写，请输入：:set noic
	要移除匹配项的高亮显示，请输入：:nohlsearch
	如果您想要仅在一次查找时忽略字母大小写，您可以使用 \c ，即输入 /ignore\c <回车>

bw 关闭当前缓冲区文件。

字符串替换，类似于sed的s命令。
`:[range]substitute/from/to/[flags]` 字符串替换，简写命令为s。
如`:%s/Professor/Teacher/`，%表示命令作用于全部行。
如果不指定行范围，`:s` 命令只作用在当前行上。
默认情况下，`:substitute` 命令只对某一行中的第一个匹配点起作用。要对行中所有匹配点起作用，你需要加一个g（global）标记。

翻页：
	整页翻页：ctrl-f ctrl-b。f就是forword，b就是backward。
	翻半页：ctrl-d ctlr-u。d=down，u=up。
	zz 让光标所在的行居屏幕中央。
	zt 让光标所在的行居屏幕最上一行，t=top。
	zb 让光标所在的行居屏幕最下一行，b=bottom。



关于鼠标。缺省值为空,即不使用鼠标。 `:set mouse=`

	n 普通模式。可以使焦点移动到点击位置；但是不能选择文本区域、复制。
	v 可视模式。不能使焦点移动到点击位置；但是可以选择文本区域、复制，但连行号一起拷贝。
	i 插入模式。不能使焦点移动到点击位置；但是可以选择文本区域、复制，但连行号一起拷贝。
	c 命令行模式。
	h 在帮助文件里,以上所有的模式。
	a 以上所有的模式。


移动
	
	5G 移动到第5行，而不论现在在哪一行。在命令模式下，不用输入“:”，直接输入5G即可。gg移动到第一行。
	2w 向右移动2个词。2b 向左移动2个词。2e移动到单词末尾。0移动到行首。
	$ 行首，^ 行尾。
	fr 当前行向右寻找字符r；Fa 当前行向左寻找字符a。
	* 匹配光标所在单词，移动到下一个匹配项
	% 移动到匹配的括号上（同行或跨行）。
	    k
	  h   l
	    j

区域缩进

	进入可视模式（mouse=a时，用鼠标选择文本块），键入"<" 或 ">"就可以向左向右缩进。如果要继续缩进，所以键入"."。
	另外，键入"gv"可以选中之前选中的文本块；键入"gv>"可以将之前选中的文本块向右缩进。

文件比较

	shell命令`vimdiff 1.txt 2.txt`可以比较文件。
	或者`vim -d test1 test2`

:make 直接在vim中执行make命令，不用加“!”

配置文件：
~/.vimrc
~/.vim

保存会话

	当前编辑多个文件，如果想先退出，下次启动vim再自动打开这些文件，可以：
	:mksession! work.vim 保存当前会话。（在当前目录创建文件work.vim，默认是Session.vim）
	vim -S work.vim 恢复会话
	在vim中手动加载会话 :source work.vim

拷贝与粘贴
	加号寄存器 普通模式下按`"+y` Vim使用技巧P139，拷贝到系统剪贴板
	不离开插入模式，粘贴寄存器中的文本：ctrl+r+0。Vim使用技巧P28


待研究的内容：
为什么gcc如果有警告，在vim的shell中无法显示，但是在独立的shell下可以？alias也是。
变量自动补全、函数自动补全，拼写错误检查如getcha(r)。
代码结构，方便点击查看函数。
多窗口、多文件切换，比如在Eclipse中可以打开多个相关文件，随意切换。
某个区块的删除。

参考资料：
	https://zh.wikipedia.org/wiki/Vim
	https://en.wikipedia.org/wiki/Vim_(text_editor)
	http://coolshell.cn/articles/11312.html

## session

```
:mksession ~/srv.vim
:source ~/srv.vim
$ vim -S ~/srv.vim
```

## 快捷键

```
ctrl+[ esc
```
