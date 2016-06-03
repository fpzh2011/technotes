# Vim插件

## 插件管理

https://github.com/VundleVim/Vundle.vim

### 安装方法

`git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim`
`vim +PluginInstall +qall`

```
	运行`vim 1.txt` 报错：
		处理 /home/zhengjh/.vim/bundle/newL9/plugin/l9.vim 时发生错误:
		第   40 行:
		E174: 命令已存在: 请加 ! 强制替换
		第   89 行:
		E174: 命令已存在: 请加 ! 强制替换
		第  100 行:
		E174: 命令已存在: 请加 ! 强制替换
		第  104 行:
		E174: 命令已存在: 请加 ! 强制替换
		请按 ENTER 或其它命令继续
	解决方法(https://github.com/VundleVim/Vundle.vim/issues/714)：
		rm -rf ~/.vim/bundle/newL9/
```

## markdown

### vim-markdown

https://github.com/plasticboy/vim-markdown

安装之后没有语法高亮。

### vim.org 

http://www.vim.org/scripts/script.php?script_id=2882

`vim markdown-1.2.2.vba.gz +":source % | quit!"`

使用向右的箭头，可以展开折叠的内容。

## golang

https://github.com/fatih/vim-go

http://tonybai.com/2014/11/07/golang-development-environment-for-vim/
