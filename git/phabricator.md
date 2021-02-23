# Phabricator使用简易说明

使用Phabricator需要先安装Arcanist客户端。[官方wiki](https://www.mediawiki.org/wiki/Phabricator/Arcanist)上有安装说明，尤其对Mac和Windows有简便的installer方案。

一般的workflow按以下顺序即可。

## 创建分支

`arc feature feature_name`
这个命令会在本地仓库创建一个`feature_name`分支、并切换到新分支。

新版本是`arc start`
https://secure.phabricator.com/T2928

这个repo有release tag
https://github.com/wikimedia/arcanist
或者用官方的legacy 2019分支

用gitlab时，有些规范要求按用户创建长期存在的分支。在Phabricator下，一般推荐的方案是按需求特性为feature（分支）命名。比如`add_hi_vec_recall`。这些分支在合并后会自动删掉。

feature的改动尽量保持完整、紧凑。如果一个feature1没开发完，又来了一个新需求，可以把feature1的改动commit后，切换到master，重新`arc feature new_feature`。一般不在一个feature中包含多个需求的改动。

不要直接在master上修改。如果是简单改动文件、合并时没有冲突，还能过去。但如果涉及到submodule的调整，直接在master修改会比较麻烦。

Tips：在`arc feature`前，执行`git pull [--ff-only]`获取最新的remote改动。

## 本地修改提交

对代码、配置进行修改后，用`git commit`提交到本地仓库。一个feature可以多次commit到本地仓库，和普通的git操作一样。

## 提交review

本地修改完成后，执行`arc diff`提交review。
`arc diff`的编辑界面如下图所示：

Tips：有些环境下的默认编辑器是nano，如果不习惯，可以在执行`arc diff`前，在shell环境设置`export EDITOR=vim`。

一般reviewer不要超过2个人，所有reviewer都accept后才可以合并代码。这些都不是强制的。
但是subscriber可以列出很多人，subscriber也会收到邮件通知、在review界面提交修改意见。如果只是像让相关者知道这个修改，列为subscriber即可。

## 合并代码

执行`arc land`可以合并代码。
这个操作会合并本地代码到master、将修改合并到remote、删除feature分支、将Phabricator中该feature设置为close状态。

如果合并的过程中有冲突，手动编辑文件、修复冲突后，执行如下操作：
* git add
* git commit
* arc diff
* arc land
如果冲突涉及的逻辑比较多，需要重新review之后才能land（这个也不是强制的）。

## arc patch

一个feature的代码一般尽量不超过300行，否则很难review。
如果feature的改动比较多、需要在自己的环境浏览、调试，可以执行`arc patch D666`。
这个操作会在本地创建一个分支，把feature代码都同步到本地分支。D666是feature编号。



