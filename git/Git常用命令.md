
# Git常用命令

http://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000/

http://www.infoq.com/cn/news/2011/03/git-adventures-index-commit

http://gitbook.liuhui998.com/index.html


	git config --global user.name "zhengjh"
	git config --global user.email "zhengjh@sinosure.com.cn"

	git config　user.name nickname
	git config　user.email nickname@gmail.com

	git init

	git add readme.txt
	git commit -m "wrote a readme file" # 把文件提交到仓库
	
	git help
	git help commit
	
	git blame				# 找出文件是何时、被何人修改的
	
	git status
	
	git gc				# 清理工作区。养成定期清理垃圾的习惯。
	
	git diff readme.txt		# 比较工作区与暂存区的差异
	git diff HEAD -- readme.txt	# 比较工作区与版本库当前分支的差异。
	git diff --cached			# 列出暂存区中，将被提交的文件，以及内容差异。
	git diff dev master		# 比较两个分支的差异。也可以比较两个版本的差异。
	git diff HEAD origin/master	# 比较本地库与远程库的差异
	
	git rm readme.txt
	执行“git rm --cached filename" 来把误添加的文件从暂存区中移除。
	
	git log				# 显示从最近到最远的提交记录。
	git log --pretty=oneline

	git reset --hard HEAD^		# Reset current HEAD to the specified state
	git reset --hard cd27f
	
	git reflog				# 列出输入的每个git命令，同时带有每次提交的版本号，可以用于向前、向后的版本切换。
	
	git checkout -- readme.txt	# 撤销工作区修改，让文件回到最近一次commit或add后的情况。
	
	git reset HEAD readme.txt	# 撤销暂存区修改。可以把暂存区的修改撤销掉，重新放回工作区。
	
	git ls-files --stage		# 查看暂存区的内容
	
	git show				# 显示各种类型的对象

	git remote add origin git@git.sinosure.com.cn:scm/~zhengjh/zhengjh.git
	git push -u origin master 
	git remoute -v 			# 查看远程仓库配置
	
	git remote show origin		# 显示所有远程分支的状态

	# 如果有一个远程分支，你想在本地跟踪它，通常的做法是
	git branch --track myfeature origin/myfeature
	git checkout myfeatur

	git checkout -b myfeature origin/myfeature	# 如果你使用 -b 标识符去检出的话，新版的 Git 会自动建立跟踪

	git clone http://zhengjh@git.sinosure.com.cn/scm/~zhengjh/zhengjh.git
	
	cat .git/refs/heads/master | xargs git cat-file -t
	
	git branch				# 列出所有分支
	git branch dev			# 创建分支dev
	git checkout dev			# 切换到分支dev
	
	fetch
	
	pull = fetch + merge
	
	merge


