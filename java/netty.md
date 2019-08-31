# netty

## nio

标准的IO编程接口是面向字节流和字符流的。而NIO是面向通道和缓冲区的，数据总是从通道中读到buffer缓冲区内，或者从buffer写入到通道中。
Java NIO使我们可以进行非阻塞IO操作。
http://wiki.jikexueyuan.com/project/java-nio-zh/java-nio-tutorial.html

### Channel

https://docs.oracle.com/javase/8/docs/api/java/nio/channels/Channel.html
http://tutorials.jenkov.com/java-nio/channels.html
http://wiki.jikexueyuan.com/project/java-nio-zh/java-nio-channel.html
https://segmentfault.com/a/1190000006824107
https://stackoverflow.com/questions/3955250/why-filechannel-in-java-is-not-non-blocking
https://www.remlab.net/op/nonblock.shtml

### Selector

https://docs.oracle.com/javase/8/docs/api/java/nio/channels/Selector.html
http://wiki.jikexueyuan.com/project/java-nio-zh/java-nio-selector.html

### SelectableChannel

https://docs.oracle.com/javase/8/docs/api/java/nio/channels/SelectableChannel.html

### Buffer

nio为什么需要buffer呢？io就绪后通知程序读，去哪里读呢？jvm把数据放到buffer，程序去buffer中读。
https://docs.oracle.com/javase/8/docs/api/java/nio/Buffer.html
http://wiki.jikexueyuan.com/project/java-nio-zh/java-nio-buffer.html
http://www.importnew.com/28007.html
https://www.jianshu.com/p/e20f54672865

## netty

### Channel

https://netty.io/4.1/api/io/netty/channel/Channel.html
每个Channel都有一个ChannelPipeline。the ChannelPipeline which handles all I/O events and requests associated with the channel

### ChannelHandler

https://netty.io/4.1/api/io/netty/channel/ChannelHandler.html

### **ChannelHandlerContext**

https://netty.io/4.1/api/io/netty/channel/ChannelHandlerContext.html

### **ChannelPipeline**

https://netty.io/4.1/api/io/netty/channel/ChannelPipeline.html

### **EventLoop**

https://netty.io/4.1/api/io/netty/channel/EventLoop.html
https://netty.io/4.1/api/io/netty/channel/EventLoopGroup.html

EventLoop与其它组件的关系（Netty实战，P32，CH3.1.2末尾）：
* 一个EventLoopGroup 包含一个或者多个EventLoop；
* 一个EventLoop 在它的生命周期内只和一个Thread 绑定；
* 所有由EventLoop 处理的I/O 事件都将在它专有的Thread 上被处理；
* 一个Channel 在它的生命周期内只注册于一个EventLoop；
* 一个EventLoop 可能会被分配给一个或多个Channel。


## 问题

对于Server来说，一个EventLoop的什么周期是什么？是一次连接交互，还是整个server的生命周期？
accept的不同channel，其pipeline是同一个吗(验证一下)？
	每一个新创建的Channel 都将会被分配一个新的ChannelPipeline。 ch6.2 ChannelPipeline接口 P76
ChannelHandlerContext 和ChannelHandler这些实例，每个连接是用同一个实例，还是不同的实例？（一个ChannelHandler 可以从属于多个ChannelPipeline）

用netty写一个http server，用curl测试，检查header、body等。检查pipeline等各个组件。
写一个http client，请求baidu首页，检查response的header body。检查pipeline等各个组件。

6.2 ChannelPipeline 接口 P77的图6.3中，有没有具体的场景和代码例子？

编解码器要在不同handler之间读写不同数据吗？

阻塞、非阻塞，同步、异步

3.2.2末尾（doc中有类似描述吗？）：
在Netty 中，有两种发送消息的方式。你可以直接写到Channel 中，也可以写到和Channel-
Handler相关联的ChannelHandlerContext 对象中。前一种方式将会导致消息从Channel-
Pipeline 的尾端开始流动，而后者将导致消息从ChannelPipeline 中的下一个Channel-
Handler 开始流动。


