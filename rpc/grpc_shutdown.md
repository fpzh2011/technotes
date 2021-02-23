# grpc server shutdown

测试环境：docker swarm
client: python
proxy: C++
server: C++


pthon client启动时，如果server不可用，不会报错。连接时server不可用才报错。

client指定round robin负载均衡策略、dnsrr，连接多个server实例，server每60秒自动断开。部分实例失效后，不会报错：
* proxy设置round robin的load balance策略，server有2个实例，server实例设置每分钟自动断开。
* python client持续请求proxy，两个server实例的ip交替显示。
* server每1分钟自动断开连接，但proxy收到的status仍都是ok。底层屏蔽了这种网络错误。netstat显示socket确实会断开，但是会自动重连（需要显式设置keey alive?）。
* docker kill一个server实例后，server会自动重启。持续请求，proxy收到的status仍都是ok，但是只显示原来旧的实例的ip。
* 超过1分钟后，继续请求，原来旧的server连接自动断开，proxy重新获取dns ip。再持续请求，两个server ip持续交替。
* 在1分钟之内将原来的全部实例都删除后(有时不会收到报错)，proxy收到的是`status=14 (UNAVAILABLE); detail=Socket closed`。proxy原封不动转发这个status给python，python抛出异常。python直接返回response，通过throw excpetion表达错误。
* 如果删掉server整个服务(docker service rm)：
  * proxy收到的信息是`status=14; detail=Connect Failed`。有时是`Socket closed`，或者`Connection reset by peer`
  * 再继续请求，收到的信息是`status=14; detail=channel is in state TRANSIENT_FAILURE`。
  * 重新启动server服务，proxy收到2个`channel is in state TRANSIENT_FAILURE`后，后续可以正常请求。
  * 但有时，需要大约20秒后，proxy开始正常收到server的请求。(rm后隔一段时间再重新启动)
  * 从几次测试看，如果请求持续不断，server可以自动重启，还是较快自动恢复的
* 如果client想针对server重启这种情况重试，可以针对`status=14 (UNAVAILABLE)`这种情况重试。一般不会整个服务所有实例挂掉。
* 但是如果整个服务挂掉，`Connect Failed`的耗时可能从300ms到十几秒。
* 如果各个服务实例依次逐个滚动重启，client的影响基本可以忽略。

如果client不指定负载均衡策略，或者既不使用dnsrr、也不设置round robin，报错和恢复情况和上面差不多。




