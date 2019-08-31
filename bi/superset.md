# superset: bi tools

https://superset.incubator.apache.org
https://superset.incubator.apache.org/installation.html

## docker

https://hub.docker.com/r/amancevice/superset/
https://github.com/amancevice/superset

## 安装后配置

https://superset.incubator.apache.org/installation.html#superset-installation-and-initialization

## Configuration

`superset_config.py`定义了各种配置，详情可参考官方文档[Configuration](https://superset.incubator.apache.org/installation.html#configuration)和[默认配置](https://github.com/apache/incubator-superset/blob/master/superset/config.py)
如果通过docker部署，`superset_config.py`一般要在镜像外面挂载。

一些重要的配置选项列出如下：

### metadata database

SQLALCHEMY_DATABASE_URI

### webserver port

SUPERSET_WEBSERVER_PORT

## Upgrading

https://superset.incubator.apache.org/installation.html#upgrading
```shell
pip install superset --upgrade
superset db upgrade
superset init
```

## filter 筛选条件

https://superset.incubator.apache.org/faq.html#how-to-add-dynamic-filters-to-a-dashboard

## 增加新的列column

https://superset.incubator.apache.org/faq.html#what-if-the-table-schema-changed
https://superset.incubator.apache.org/faq.html#how-do-i-get-superset-to-refresh-the-schema-of-my-table




https://superset.incubator.apache.org/
https://tech.glowing.com/cn/superset-redash-metabase-1/
http://lxw1234.com/archives/2018/03/904.htm
http://jackpgao.github.io/2018/05/30/Superset-Introduce/
http://www.xraywu.me/2017/08/06/an-introduction-to-superset/
https://sanyuesha.com/2017/11/14/superset/

