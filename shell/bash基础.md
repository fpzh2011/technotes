# bash基础

## 重定向

### Here Documents

`command << word`可以指定一段多行文本，这段文本作为命令的输入，输入的起止位置被特定单词界定。如下面的例子中用`111`定界起止位置：
```shell
cat << 111
abc
def
111
```

参考：
* [1]，P209
* man bash

### Here Strings

更进一步的，`<<<`可以从一个变量，或者一行后面的内容读取输入。如：
```shell
a="abc"
cat <<< $a
cat <<< 12345678
```


## 参考资料

[1] Linux命令行与shell脚本编程大全（第2版）

