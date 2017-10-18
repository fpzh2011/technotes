
# Git

## 安装

### Linux

	`sudo yum install git`

### Windows

Windows下使用Git需要Cygwin模拟环境。

msysgit打包了Cygwin和Git，是Windows版的Git，可以从http://msysgit.github.io/下载。

### 设置

	git config --global user.name "zhengjh"
	git config --global user.email "zhengjianhua@lovebizhi.com"
	git config --global core.quotepath false # 解决中文文件名乱码问题
	git config --global core.editor vim # git默认编辑器

Git的配置信息分为全局和项目两种，上面命令中带了“--global”参数，这就意味是在进行全局配置，它会影响本机上的每个一个Git项目。查看Git的全局配置：`cat ~/.gitconfig`。

Git可以为每个项目设定不同的配置信息。查看Git项目的配置信息：`cat .git/config` 或 `git config --list`。进入Git项目所在目录，执行下面的配置命令：

	git config　user.name nickname
	git config　user.email nickname@gmail.com

### 忽略文件权限的配置

git config core.filemode false

## 创建版本库

版本库又名仓库，英文名repository，你可以简单理解成一个目录，这个目录里面的所有文件都可以被Git管理起来，每个文件的修改、删除，Git都能跟踪，以便任何时刻都可以追踪历史，或者在将来某个时刻可以“还原”。

	mkdir -p project/git
	cd project/git
	git init

`git init`在项目的根目录下创建`.git`目录作为版本库。项目的子目录下没有git相关目录。
如果本地目录已经有了一些文件，在之上创建仓库，直接`git init`即可。之前的文件可以被add、commit。

你对本地的源代码进行编辑修改后创建的提交也都会先保存在这里面， 然后再推送到远端的服务器。
当我们我把项目目录和“Git目录”一起拷到其它电脑里，它能马上正常的工作（所有的提交信息全都保存在Git目录里）；甚至可以只把“Git目录”拷走也行，但是要再签出（checkout）一次。

### 增加文件到版本库

	vi readme.txt
	git add readme.txt
	git commit -m "wrote a readme file" # 把文件提交到仓库
	# 为什么Git添加文件需要add，commit一共两步呢？因为commit可以一次提交很多文件，所以你可以多次add不同的文件，最后一次性提交。

Git中的提交，是指提交到本地版本库。push才是提交到远程版本库。

## 操作

帮助 `git help` 或 `git help commit`。

每次执行`git add`添加文件到暂存区时，它都会把文件内容进行SHA1哈希运算，在索引文件中新加一项，再把文件内容存放到本地的“git目录”里。
如果在上次执行 “git add”之后再对文件的内容进行了修改，那么在执行“git status”命令时，Git会对文件内容进行SHA1哈希运算就会发现文件又被修改了，这时“readme.txt“就同时呈现了两个状态：被修改但未 被暂存的文件（changed but not updated），已暂存可以被提交的文件（changes to be committed）。如果我们这时提交的话，就是只会提交第一次“git add"所以暂存的文件内容。

状态查看 `git status`，可以查看哪些文件还没有纳入版本控制，纳入版本控制的哪些作了修改、尚未提交等。

文件修改后提交，也要先add，再commit到本地仓库，push到远程仓库。
```shell
git add readme.txt
git commit -m "the second commit" readme.txt
git push origin master
```
删除文件 `git rm readme.txt` 。删除操作也要commit才能提交到版本库。
执行`git rm --cached filename`把误添加的文件从暂存区中移除。

查看提交历史 `git log` 或 `git log --pretty=oneline`，列出每一次提交。
这里的版本号（commit id）是sha1计算得到的。

## 工作区、暂存区、版本库

开始创建、修改的文件，都在工作区（Working Directory）。
当你在项目的不同分支间切换时，“工作目录”中的文件可能会被替换或者删除；“工作目录”只是保存着当前的工作，你可以修改里面文件的内容直到下次提交为止。

add命令将工作区的**修改**内容移动到暂存区（Stage或Index）。
暂存区（staging area）一般存放在`.git/index`中，所以我们把暂存区有时也叫作索引。
索引是一个二进制格式的文件，里面存放了与当前暂存内容相关的信息，包括暂存的文件名、文件内容的SHA1哈希串值和文件访问权限，整个索引文件的内容以暂存的文件名进行排序保存的。
`git commit -a`会跳过暂存区，即自动将所有未跟踪的内容添加到暂存区、再提交。
查看暂存区内容（Pro Git，P243）`git ls-files -s`
参考资料：
http://www.worldhello.net/2010/11/30/2166.html

commit将暂存区的内容移动到版本库（Repository）的当前分支。文件提交后，暂存区仍保存提交过的所有文件的内容（可以用`git ls-files --stage`查看暂存区的内容）。下次“git add”时把同名文件的内容冲掉。

## HEAD

[HEAD是当前分支的符号链接](https://git-scm.com/book/tr/v2/Git-Internals-Git-References)。一般来说，它总是指向分支最后一次提交（pro git，P242）
正常情况下，`HEAD`指向ref，ref指向commit。
出现`detached HEAD`时，`HEAD`直接指向commit，[可以认为这是一个未命名的分支](https://stackoverflow.com/questions/5772192/how-can-i-reconcile-detached-head-with-master-origin)。
`detached HEAD`指向的commit是dangling的，垃圾收集过程会清除这些commit。
interactive rebase是通过`detached HEAD`完成的。

上一个版本是HEAD^，上上一个版本是HEAD^^，依次类推。往上100个版本，可以写为HEAD~100。
HEAD不一定是整个仓库的最新版本，可能比其它分支的版本旧。




**如果是 detached HEAD ，在这上面提交会有什么后果？**



```shell
cat .git/HEAD
# ref: refs/heads/resume_tag_sort_jd
ls .git/refs/heads
# master  match_intern_test  resume_tag_sort_jd  syncdata_tag_intern
cat .git/refs/heads/master
# d90bbd0f0375565a0dd55eda3073137e1b3afdc0

# detached HEAD 。这样会新建一个未命名、unref的分支??????? 
git checkout HEAD^
cat .git/HEAD
# e2175451128d54bc7211461529ba360b7ff10ec1
```

参考资料：
https://stackoverflow.com/questions/5772192/how-can-i-reconcile-detached-head-with-master-origin
https://stackoverflow.com/questions/3965676/why-did-my-git-repo-enter-a-detached-head-state
https://stackoverflow.com/questions/2304087/what-is-head-in-git
https://stackoverflow.com/questions/10228760/fix-a-git-detached-head

### 管理修改

因为git管理的是**修改**diff，而不是文件。

比如：修改一个文件，add到暂存区，再修改该文件，提交。那么，只有第一次修改的被提交了。
因为git commit只负责把暂存区的修改提交了，其它修改没有提交。

传统版本控制系统如SVN，使用的是“增量文件系统”（Delta Storage systems）。它们存储是每次提交之间的差异。
而Git正好与之相反，它是保存的是每次提交的完整内容（snapshot）。

Git跟踪的是内容不是文件。

## 版本回退、撤销修改

### 版本切换
用 `git reset --hard HEAD^` 回退到上一个版本。
向前到最新版可以用 `git reset --hard cd27f` ，hard后面的版本号不用写全，git会自己找。

git的版本切换非常快。因为其内部用HEAD指针指向当前版本。向前或向后切换版本时，只是改变HEAD指针的值，同时更新工作区的文件。

`git reflog` 可以列出输入的每个git命令，同时带有每次提交的版本号，可以用于向前、向后的版本切换。


**分支切换时，如果暂存区还有内容，会怎样？？？**
https://stackoverflow.com/questions/22053757/checkout-another-branch-when-there-are-uncommitted-changes-on-the-current-branch

### 撤销工作区修改

撤销在工作区的修改 `git checkout -- readme.txt`。让文件回到最近一次commit或add后的情况。
如果文件修改还没有被放到暂存区，撤销修改就回到和版本库一模一样的情况。
如果文件已经被添加到暂存区后，又做了修改，撤销修改就回到添加到暂存区后的状态。

这个命令也可以处理本地工作区文件被误删除的情况。会将版本库或暂存区中的文件恢复到工作区。

### 撤销暂存区修改

`git reset HEAD readme.txt` 可以把暂存区的修改撤销掉，重新放回工作区。
所以，reset命令既可以回退版本，也可以把暂存区的修改回退到工作区。这里HEAD表示最新的版本。

如果提交错误文件到本地仓库，可以通过上述`git reset`命令切换到合适的版本。
但如果将本地仓库的内容提交到了远程仓库，就无法撤回修改了。

## rebase

gitpro P86

## 远程仓库管理

### 关联远程仓库到本地

如果先有本地仓库、并做了一些修改，用这种模式。
还没调试通过，没搞懂！！！？？？

	git remote add origin git@git.sinosure.com.cn:scm/~zhengjh/zhengjh.git
	git push -u origin master 

### 克隆远程仓库到本地

`git clone http://zhengjh@git.sinosure.com.cn/scm/~zhengjh/zhengjh.git`

git支持多种协议，ssh、http、https等。默认的 git@git.sinosure.com.cn:scm/~zhengjh/zhengjh.git 使用ssh协议。

如果通过git、ssh协议传输，服务器端会在传输前把需要传输的各种对象先打好包再进行传输；而http（s）协议则会反复请求要传输的不同对象。现在Git对http（s）协议传输Git仓库做了一定的优化，http（s）传输现在也能达到ssh协议的效率。但是https需要输入口令。

`cat ./.git/config` 可以查看远程版本库的地址。

显示所有远程分支的状态：`git remote show origin`

最后，如果有一个远程分支，你想在本地跟踪它，通常的做法是：

	git branch --track myfeature origin/myfeature
	git checkout myfeatur

如果你使用 -b 标识符去检出的话，新版的 Git 会自动建立跟踪：`git checkout -b myfeature origin/myfeature`

##分支管理

###创建分支###

`git checkout -b dev`
这条命令相当于以下两条命令：
```shell
git branch dev	# 创建分支dev
git checkout dev	# 切换到分支dev
```

###查看所有分支###

`git branch` 

在Git中，分支的命名信息保存在“.git/refs/heads”目录下；远程仓库的命名信息保存在“.git/refs/remotes”目录下；标签信息保存在 “.git/refs/tags”下。

###删除分支###

`git branch -d branch_name` 或 `git branch -D branch_name`
https://stackoverflow.com/questions/2003505/how-do-i-delete-a-git-branch-both-locally-and-remotely

## 从github克隆项目

`git clone git://github.com/nlohmann/json.git`

## 忽略文件

参考资料：
http://www.cnblogs.com/eddy-he/archive/2012/03/08/git_ignore_file.html
http://blog.csdn.net/benkaoya/article/details/7932370
http://blog.csdn.net/xmyzlz/article/details/8592302

### 全局配置

编辑文件`vim ~/.gitignore`：
```
test.cc
test.c
*.log
a.out
*.o
#vim临时文件
*.swp
*.pyc
#mysql workbench bak
*.mwb.bak
```

使全局配置生效：`git config --global core.excludesfile ~/.gitignore`

### 局部配置

在相应目录下配置文件`.gitignore`即可。

## 常用命令

### diff

```shell
# git diff 可以按模式指定文件。如果文件没有提交过，或者没有放入过暂存区，不会进行比较。
# 比较工作区与暂存区的差异
git diff readme.txt
# 比较工作区与当前分支的差异
git diff HEAD -- readme.txt
# 比较暂存区与HEAD的差异 --cached
git diff --staged
# 比较本地库与远程库的差异
git diff HEAD origin/master
```
参考：
https://stackoverflow.com/questions/1587846/how-do-i-show-the-changes-which-have-been-staged
http://365git.tumblr.com/post/474079664/whats-the-difference-part-1
Git Pro 中文版，P30-32

### checkout

当检出一个分支时，它会修改HEAD指向新的分支引用，将索引（暂存区）填充为该次提交的快照，然后将索引的内容复制到工作目录中。（Pro Git，P237）


## 参考资料

http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/

http://www.infoq.com/cn/news/2011/03/git-adventures-index-commit

http://gitbook.liuhui998.com/index.html

www.ruanyifeng.com/blog/2015/08/git-use-process.html

Git常用命令备忘
http://www.imooc.com/article/1111

简明 Git 命令速查表（中文版）
http://www.imooc.com/article/1171

10 个迅速提升你 Git 水平的提示
http://www.imooc.com/article/1326

Git 中级用户的25个提示
http://www.imooc.com/article/1405

	www.imooc.com/wap/article?article_id=1426
	shanewfx.github.io/blog/2012/04/28/git-command-note/
	www.oschina.net/translate/git-fetch-and-merge?print
	www.toobug.net/article/git_and_gitflow.html
	www.zhihu.com/question/19946553
	
	segmentfault.com/a/1190000002918123
	www.devtf.cn/?p=686
	www.zhanjinwu.cn/blog/2014/01/17/git-merge-shou-ce-ye/
	www.jianshu.com/p/63f9ecc3540c
	www.cppblog.com/deercoder/archive/2011/11/13/160007.aspx
	www.codeceo.com/article/git-tips-years.html
	http://shanewfx.github.io/blog/2012/04/28/git-command-note/







## 问题

如果本地仓库已经有了文件，如何与远程仓库建立联系？文件之间如何建立关系，逐个文件进行文本比较？版本记录、提交记录如何体现？

**配置忽略策略，如a.out，obj，tags文件等。**

git自己创建的仓库，与Eclipse导出的，能否同时使用？

如何创建仓库，创建不同的项目，管理用户和权限。

有哪些好用的git可视化工具，Win/Linux。

如何减少大量不必要的提交历史？流程上如何控制？dev的提交历史，合并时，是否会带到master？
	git merge --squash dev  # git log只显示master的提交记录，没有dev的提交记录，包括merge的。但是blame单个文件时，可以显示dev的所有修改提交。
	一般可以考虑rebase、git cherry-pick



