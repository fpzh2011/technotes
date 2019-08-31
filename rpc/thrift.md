# 安装

0.9.3: `brew install thrift@0.9`

http://thrift.apache.org/docs/install/os_x

# 生成代码

`thrift --gen java Hello.thrift`
* `Hello.Iface`是service接口定义。
* `Hello.Client`是客户端代码。
* `Hello.Precossor`是服务端代码。
* 支持的语言: **java, cpp, py, go,** ...

server代码可以在不同的包。同时写一个service的实现代码（可以不涉及thrift）。
server通过processor启动服务器，processor调用具体的service来实现业务逻辑。

client代码也可以在不同的包。创建`Hello.Client`后直接调用相关方法。

## IDL

http://1csh1.github.io/2017/02/21/Thrift%20IDL基本语法/
http://www.yeolar.com/note/2015/10/24/thrift-missing-guide/
http://thrift.apache.org/docs/types
http://thrift.apache.org/docs/idl

## 编写IDL文件时需要注意的问题

https://blog.csdn.net/houjixin/article/details/42778335
https://juejin.im/post/5baf11e35188255c8c0ca80e

# generator 语言相关

https://thrift.apache.org/tutorial/

## go

https://thrift.apache.org/tutorial/go
https://godoc.org/github.com/apache/thrift/lib/go/thrift
https://segmentfault.com/a/1190000013127570
https://www.jianshu.com/p/a58665a38022

## python

https://thrift.apache.org/tutorial/py

# 资料

https://github.com/apache/thrift
http://diwakergupta.github.io/thrift-missing-guide/
https://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/index.html
https://zh.wikipedia.org/wiki/Thrift
https://blog.csdn.net/houjixin/article/details/42778335
http://thrift.apache.org/docs/concepts

注意事项
https://www.cnblogs.com/duanxz/p/3554341.html

# 问题

如果定义了多个服务，是否需要多个processor？thrift multiple service
Iface和AsyncIface的区别是什么？
go build/install 不能自动下载依赖包吗？
go build/install 的区别是什么？
go get github.com/.../...
go list 列出的包，是系统包，还是项目包？
理解go的各个子命令


