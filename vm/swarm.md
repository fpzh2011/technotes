


现象：
grafana的docker service看不到数据，但是`/`、`/docker`等有数据。
influx volume数据超过15G，绝大部分是cadvisor。

解决办法：
清空influxdb的数据。直接rm volume、重新deploy stack

后果：
几分钟后可以看到新数据，但旧数据就丢失了。


