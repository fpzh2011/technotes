# librdkafka

## 要点概述
* C/C++ kafka client
* [线程安全](https://github.com/edenhill/librdkafka/blob/master/INTRODUCTION.md#threads-and-callbacks)
* 可以配置高吞吐或低延迟
* librdkafka is asynchronous in its nature and performs most operation in its background threads.
* delivery report callback is optional but highly recommended
* supports the idempotent producer which provides strict ordering and and exactly-once
* A delivery error will occur if the message could not be produced within message.timeout.ms.
* Messages are considered in-queue from the point they are accepted by produce() until their corresponding delivery report callback/event returns.
* The delivery report is trigged by the application calling `rd_kafka_poll()` (at regular intervals) or `rd_kafka_flush()` (at termination). 
* idle连接超时后会关闭。

## 重要配置

* message.max.bytes
* linger.ms
* batch.num.messages
* compression.codec
* request.required.acks
* message.send.max.retries

[最重要的配置属性是linger.ms](https://github.com/edenhill/librdkafka/blob/master/INTRODUCTION.md#performance)

message.max.bytes=10MB时，会报Broker: Message size too large。可能是和服务端设置、以及librdkafka的batch有关。

## delivery report callback

The delivery report is trigged by the application calling `rd_kafka_poll()` (at regular intervals) or `rd_kafka_flush()` (at termination).

## 重要函数

* rd_kafka_produce
* rd_kafka_queue_poll
* rd_kafka_poll
* rd_kafka_consumer_poll
* rd_kafka_outq_len
* rd_kafka_queue_poll_callback
* rd_kafka_conf_set_dr_msg_cb

poll函数应该是查询、处理队列中的回调事件；这些事件也会占用队列长度限制。

## 报错

message size大于local或remote设置的`message.max.bytes`: Broker: Message size too large

队列满(包括回调事件): Local: Queue full

remote在扩容磁盘、维护:
```
Broker: Disk error when trying to access log file on disk
Local: Message timed out
Connect to ipv4#10.20.2.217:9092 failed: Connection refused (after 0ms in state CONNECT)
Connect to ipv4#10.20.2.217:9092 failed: Connection refused (after 0ms in state CONNECT)
Disconnected (after 577997ms in state UP)
```

## 参考资料

https://github.com/edenhill/librdkafka
https://github.com/edenhill/librdkafka/blob/master/INTRODUCTION.md
https://github.com/edenhill/librdkafka/blob/master/src/rdkafka.h
https://github.com/edenhill/librdkafka/blob/master/src-cpp/rdkafkacpp.h
https://github.com/edenhill/librdkafka/wiki/FAQ

## 疑问

produce full queue死循环是否合适？
10MB的Broker: Message size too large，如何处理？size too large是否与scale等参数相关？是否消息太多，poll的超时需要更大一些？

