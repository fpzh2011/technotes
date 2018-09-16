# vert.x笔记

版本：3.4.2

http://www.sczyh30.com/posts/Asynchronous/vertx-blueprint-1-todo-backend-tutorial/
http://www.sczyh30.com/posts/Asynchronous/vertx-blueprint-2-vertx-kue-core-tutorial/
https://vertxchina.github.io/vertx-translation-chinese/start/SimpleStart.html

## 实现类

```
Vertx.factory -> io.vertx.core.impl.VertxFactoryImpl
vertx -> io.vertx.core.impl.VertxImpl

//MySQLClient.createNonShared(...)
sqlclient wrapper class: io.vertx.ext.asyncsql.impl.ClientWrapper
wrapper.client: io.vertx.ext.asyncsql.impl.AsyncSQLClientImpl
wrapper.client.baseClient: io.vertx.ext.asyncsql.impl.MYSQLClientImpl
MYSQLClientImpl extends: io.vertx.ext.asyncsql.impl.BaseSQLClient
conn class: class io.vertx.ext.asyncsql.impl.MySQLConnectionImpl

Future.future(): io.vertx.core.impl.FutureImpl
vertx.eventBus(): io.vertx.core.eventbus.impl.EventBusImpl
vertx.getOrCreateContext(): EventLoopContext
```

AbstractVerticle.start() 是同步启动。

`Vertx.runOnContext()`是由`nettyEventLoop`异步执行。

## Verticle & EventLoop

vertx、verticle、Event Bus、Event Loop 线程的关系。

`Verticle`部署最终是在`DeploymentManager.doDeploy(...)`中完成的，在这里调用`Verticle.start()`方法。

https://vertxchina.github.io/vertx-translation-chinese/start/FAQ.html
Vert.x保证同一个普通Verticle（也就是EventLoop Verticle，非Worker Verticle）内部的所有处理器（Handler）都只会由同一个EventLoop线程调用，由此保证Verticle内部的线程安全。
Vert.x的Handler内部是atomic/原子操作，Verticle内部是thread safe/线程安全的，Verticle之间传递的数据是immutable/不可改变的。
一个vert.x实例/进程内有多个Eventloop和Worker线程，每个线程会部署多个Verticle对象并对应执行Verticle内的Handler，每个Verticle内有多个Handler，普通Verticle会跟Eventloop绑定，而Worker Verticle对象则会被Worker线程所共享，会依次顺序访问，但不会并发同时访问，如果声明为Multiple Threaded Worker Verticle则没有此限制，需要开发者手工处理并发冲突，我们并不推荐这类操作。

建议将客户端与Verticle对象绑定，一个Verticle对象内保留一个特定客户端的引用，并在start方法中将其实例化，这样Vert.x会在执行deployVerticle的时候执行start方法，实例化并保存该对象，在Verticle生命周期内，不需要频繁创建和关闭同类型的客户端，建议在Verticle的生命周期内对于特定领域，只创建一个客户端并复用该客户端
每次使用客户端完之后，无需调用client.close();方法关闭客户端。Vert.x提供客户端的目的就在于复用连接以减少资源消耗，提升性能。

[多个Verticle实例不会引起冲突，因为它们会共享同一个http server。](http://colobu.com/2016/03/31/vertx-thread-model/)

当多个 HTTP 服务端在同一个端口上监听时，Vert.x 会使用轮询策略来管理请求处理。
https://vertxchina.github.io/vertx-translation-chinese/core/Core.html

DeploymentOptions对象可以设置发布参数，比如instances，它用来指定分布的Verticle实例的数量，默认是一个。

EventLoop逐个处理异步事件，如连接、redis命令等。redis命令响应到达后，也会触发事件处理。所以，正常情况下所有操作应该都是在EventLoop线程处理的。

每个 Socket 会自动在Event Bus中注册一个处理器，当这个处理器中收到任意 Buffer 时，它会将数据写入到 Socket。

## 异常处理

如果handler抛出异常，注册为后续执行的handler仍会正常执行，比如数据库连接关闭的handler，数据库连接会正常关闭。
web handler如果抛出异常，返回500错误码。

## jdbc

http://www.jianshu.com/p/6725ac7c2143

从`AsyncSQLConnectionImpl`这个类的代码可以看到，`SQLConnection.close()`将连接还给连接池。如果多次关闭同一连接，可能出现两个请求使用同一连接，导致两个请求的事务串了。
数据库操作都不抛出异常，异常都封装在 AsyncResult 中。估计所有异步操作都是这样。conn 不用 finally 包起来直接 close 似乎是惯常做法。

如果需要使用数据库事务，包含多个严格顺序的操作，是不是应该用compose？

## 线程模型

http://www.sczyh30.com/posts/Vert-x/vertx-advanced-demystifying-thread-model/
http://colobu.com/2016/03/31/vertx-thread-model/
http://www.sczyh30.com/posts/Vert-x/vertx-advanced-execute-blocking-internal/
异步RPC
http://www.sczyh30.com/posts/Vert-x/vertx-advanced-async-rpc/

通过jstack和top可以看到，即使服务器空闲，eventloop线程也是在持续不断运行。
一个应用程序通常是由在同一个 Vert.x 实例中同时运行的许多 Verticle 实例组合而成。不同的 Verticle 实例通过向 Event Bus 上发送消息来相互通信。
Stardand Verticle：这是最常用的一类 Verticle —— 它们永远运行在 Event Loop 线程上。
当 Standard Verticle 被创建时，它会被分派给一个 Event Loop 线程，并在这个 Event Loop 中执行它的 start 方法。当您在一个 Event Loop 上调用了 Core API 中的方法并传入了处理器时，Vert.x 将保证用与调用该方法时相同的 Event Loop 来执行这些处理器。
Verticle 实例中 所有的代码都是在相同Event Loop中执行（只要您不创建自己的线程并调用它！）
您可以将您的应用中的所有代码用单线程方式编写，让 Vert.x 去考虑线程和扩展问题。您不用再考虑 synchronized 和 volatile 的问题，也可以避免传统的多线程应用经常会遇到的竞态条件和死锁的问题。

一个vertx实例貌似只会创建一个Acceptor线程（在`VertxImpl`的构造函数中写死）。即使创建两个HttpServer监听不同端口，通过jstack也只能看到一个Acceptor线程。

## EventBus

http://www.sczyh30.com/posts/Vert-x/vertx-advanced-local-event-bus-internal/
http://www.enterprise-integration.com/blog/vertx-understanding-core-concepts/

## Future

https://vertxchina.github.io/vertx-translation-chinese/start/FAQ.html
在最新的版本中(3.4.0+)，可以通过 Future.future 方法以函数式的方式直接用 Future 来包装一个异步的代码块。
Future 接口提供了 compose 方法来链式地组合多个异步操作。

## HTTPS/SSL

http://vertx.io/docs/vertx-web-client/java/
https://gist.github.com/InfoSec812/a45eb3b7ba9d4b2a9b94

### 双向认证配置

PEM格式配置：
```json
"pemKeyCertOptions":{
  "certPath":"client.crt",
  "keyPath":"client.pem"
  },
"pemTrustOptions":{
  "certPaths":[
    "ca.crt"
    ]
  }
```

如果`keyPath`对应文件第一行是`-----BEGIN RSA PRIVATE KEY-----`，需要进行`nocrypt`转换。
`openssl pkcs8 -topk8 -inform pem -in client.key -outform pem -nocrypt -out client.pem`
转换后的第一行为`-----BEGIN PRIVATE KEY-----`。

查看PEM格式证书的信息: `openssl x509 -in certificate.crt -text -noout`
查看PEM格式私钥的信息: `openssl rsa -in mykey.key -text -noout`
参考：http://www.cnblogs.com/guogangj/p/4118605.html

## redis

http://vertx.io/docs/vertx-redis-client/java/
client会自动重连。

### redis订阅

```java
import io.vertx.core.json.JsonObject;
import io.vertx.redis.RedisClient;
import io.vertx.redis.RedisOptions;

//`io.vertx.redis.` 是固定的路径前缀，`channel1:id`是redis订阅频道
vertx.eventBus().<JsonObject>consumer("io.vertx.redis.channel1:id", received -> {
  JsonObject value = received.body().getJsonObject("value");
  System.out.println("sub message: " + value);
});
RedisClient redis = RedisClient.create(vertx, new RedisOptions());
redis.subscribe("channel1:id", res -> {
    if (res.succeeded()) {
      System.out.println("res.succ: " + res.result());
    }
});
System.out.println("after subscribe");
```
参考资料：
http://vertx.io/docs/vertx-redis-client/java/
https://github.com/vert-x3/vertx-redis-client/blob/master/src/main/java/examples/Examples.java
https://github.com/vert-x3/vertx-examples/blob/master/redis-examples/pom.xml

## JsonObject

如果是非法的json字符串，构造`JsonObject`时会抛出`RuntimeException`。
如果`gerString/getInteger`等函数的字段在`JsonObject`中不存在，会返回`null`。如果数据类型不匹配，会抛出异常。

## 异步协调方法

### CompositeFuture

如果成功，`CompositeFuture.list()`返回全部`Future`的结果。
http://vertx.io/docs/vertx-core/java/#_async_coordination

`CompositeFuture.all()`也是异步执行，它会为每个`Future`重新设置`Handler`。
`Future.handle()`只会被成功调用一次。所以，如果用`CompositeFuture`处理`Future`，就不要再定制`Future`自己的`Handler`了。否则，`Future`被哪个`Handler`处理是不确定的，如果处理逻辑不一样，会出现不一致。
`CompositeFuture`会以数组形式保存所有的`Future`参数，通过保存的`Future`数组，可以得到所有的`Future.result()`结果。

### compose

适用于下一个操作处理上一个操作结果的情况。
https://vertxchina.github.io/vertx-translation-chinese/start/FAQ.html

### 自定义future

[规避回调地狱](https://vertxchina.github.io/vertx-translation-chinese/start/FAQ.html)

## access.log

https://github.com/tokuhirom/vertx-samples/blob/master/src/main/java/com/example/AccessLogServer.java
```java

//java -Dvertx.logger-delegate-factory-class-name=io.vertx.core.logging.Log4j2LogDelegateFactory -jar target/server-1.0-fat.jar
package com.example;

import io.vertx.ext.web.handler.LoggerFormat;
import io.vertx.rxjava.core.Vertx;
import io.vertx.rxjava.core.http.HttpServer;
import io.vertx.rxjava.ext.web.Router;
import io.vertx.rxjava.ext.web.handler.LoggerHandler;

public class AccessLogServer {
    public static void main(String[] args) {
        System.setProperty("vertx.logger-delegate-factory-class-name",
                "io.vertx.core.logging.Log4j2LogDelegateFactory");

        Vertx vertx = Vertx.vertx();
        HttpServer httpServer = vertx.createHttpServer();
        Router router = Router.router(vertx);
        //放在第一个handler
        router.route().handler(LoggerHandler.create(LoggerFormat.DEFAULT));
        httpServer.requestHandler(router::accept).listen(8080);
    }
}
```

## Cluster

https://www.cubrid.org/blog/understanding-vertx-architecture-part-2

## Vertx-Unit

https://vertx.io/blog/unit-and-integration-tests/
https://vertx.io/docs/vertx-unit/java/
https://mp.duan8.com/a/ruzkabo.html

The methods annotated with @BeforeClass and @AfterClass are invoked once before / after all tests of the class. 
The setUp method is invoked before each test (as instructed by the @Before annotation)
http://vertx.io/blog/unit-and-integration-tests/
在CI的启动代码里可以去调用shell script
https://stackoverflow.com/questions/525212/how-to-run-unix-shell-script-from-java-code

http://vertx.io/blog/my-first-vert-x-3-application/
http://vertx.io/blog/vert-x-application-configuration/
http://vertx.io/blog/some-rest-with-vert-x/
https://github.com/vert-x3/vertx-examples/tree/master/unit-examples

## 常见问题

* vertx中，如果`response`已经`end()`，就不要再执行`routingContext.next()`了，否则会抛出异常。
* 如果`router`设置的`TimeoutHandler`触发超时，vertx服务端会返回http 503状态码。



