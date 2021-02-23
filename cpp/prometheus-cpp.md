# prometheus-cpp

## Builder

Builder只是构建prometheus对象（histogram/counter等）的辅助对象。构建完prometheus对象后，Builder对象就析构了。
Builder存储以下字段
* name
* labels
* help

## Registry

所有的prometheus监控对象（Family）都在Registry中。包括
* counter
* gauge
* histogram
* summary
每种对象是一个`vector<Family>`

Add创建/返回Family对象
* 在增加/搜索一个对象时会加锁，如histogram
* Add传入的labels就是Family的constant_labels
* 顺序扫描同类型所有对象
  * 如果name和labels都一致，就返回已经创建的对象
  * 否则新建一个对象
所以，最好不要频繁调用Add方法。

## Family

一个Family对应一个metric_name
* 一个Family可能对应多个Histogram。每个Histogram对应一个labels
* constant_labels是metric_name固定的labels，所有时间序列都有这些labels
* labels是除constant_labels之外的、额外的label

Family.Add返回具体的监控对象，比如Histogram，对应`metric_name + labels`
* Add先计算labels的hash
* 加锁
  * 如果hash存在，则返回Histogram
  * 否则，新建Histogram并返回
* Add方法不宜频繁调用
  * 因为每次都要新建一个Histogram，会拷贝buckets
  * Counter每次会建一个Gauge（包含一个atomic）
  * summary自己包含锁

## 具体的监控对象

以下对象都是`metric_name + labels`
* Counter
* Gauge
* Histogram
* Summary

## metric name

prometheus服务端其实是不区分metric type的，histogram等type只是便于client处理用的。
所以，同名的name不能出现在不同的metric type中，否则数据会错乱。
进而，最好以集中统一的方式定义metric name。name确定了，其实就可以集中统一的定义family。

## facebook common stats

设计要点
* 通过宏定义，整数+string表示metric
  * 宏定义enum常量，给各个metric name确定int编号
  * 通过宏定义，初始化metric name vector
  * 在main函数中通过init调用，初始化stats。可以预先创建所有metric
* counter
  * stats用atomic fetch_add，比prometheus-cpp性能好
labels可以请求时再创建。






