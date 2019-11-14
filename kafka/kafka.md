# kafka

## python

https://github.com/confluentinc/confluent-kafka-python
https://github.com/dpkp/kafka-python
https://www.infoq.cn/article/2017/09/kafka-python-confluent-kafka

## go

https://github.com/confluentinc/confluent-kafka-go

## zookeeper

kafka使用zookeeper维护集群成员的关系([1] P73)
消费者把每个分区最后读取的偏移量保存在zookeeper或kafka。([1]P6)
新版本已经不用zookeeper保存偏移量(P80)。

## key键

key并不是表明消息唯一性的id。key的作用只是用于确定分区。相同的key在同一个分区内。([1]P4,45,85)

## 参考资料

[1] Kafka权威指南


