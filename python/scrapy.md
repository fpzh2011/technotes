# scrapy概要

## 起步

创建项目：`scrapy startproject example`
项目目录结构：
```
.
└── example
    ├── example
    │   ├── __init__.py
    │   ├── items.py
    │   ├── middlewares.py
    │   ├── pipelines.py
    │   ├── settings.py
    │   └── spiders
    │       └── __init__.py
    └── scrapy.cfg
```

spider对象需要放在`spiders`目录。

运行爬虫：`scrapy crawl books -o books.csv`

scrapy shell
```
scrapy shell "http://books.toscrape.com/"
scrapy shell file:///absolute/path/to/file.html
```

## 重要配置

https://doc.scrapy.org/en/latest/topics/settings.html
修改`settings.py`:
```python
USER_AGENT = ''
ROBOTSTXT_OBEY = True
CONCURRENT_REQUESTS = 16
CONCURRENT_REQUESTS_PER_DOMAIN = 8
CONCURRENT_REQUESTS_PER_IP = 0
DEPTH_PRIORITY = 0
DEPTH_STATS_VERBOSE = False
DOWNLOAD_DELAY = 0
LOG_LEVEL = INFO
REDIRECT_MAX_TIMES = 20
LOGSTATS_INTERVAL = 60
```

`USER_AGENT`可以在spider中设置`custom_settings`，因为PC和移动的值不同。`USER_AGENT`的值可以定义在自己的配置文件中。
```python
# self是spider对象
print self.settings['USER_AGENT']
```

## xpath

http://www.w3school.com.cn/xpath/index.asp
https://www.w3.org/TR/1999/REC-xpath-19991116/

## bloomfilter+redis

python3才支持scrapy-redis-bloomfilter
https://github.com/Python3WebSpider/ScrapyRedisBloomFilter
http://www.voidcn.com/article/p-uspqjmkk-bro.html

## 限速

https://docs.scrapy.org/en/latest/topics/autothrottle.html

## 限制

* 根据[crawl](https://docs.scrapy.org/en/latest/topics/commands.html#crawl)命令的文档，应该不能同时运行多个project的spider。
* 不同的spider应该使用不同的名字。否则会导致unexpected behavior。
* 如果使用`CrawlSpider`的[rules](https://docs.scrapy.org/en/latest/topics/spiders.html#crawling-rules)，就不能override parse。

## scrapy check

`scrapy check`可以对程序进行基本测试。

参考：
https://docs.scrapy.org/en/latest/topics/contracts.html
https://doc.scrapy.org/en/latest/topics/commands.html#check
https://doc.scrapy.org/en/latest/topics/debug.html

## scrapy-redis

`RedisSpider`默认从redis获取`start_urls`。如果redis没有就一直等待，push到redis后就开始执行。
如果不依赖redis设置初始页面，可以覆盖`start_requests`方法。
去重逻辑是用sha1计算url指纹，包括method等，但剔除参数顺序等影响。详见`dupefilter.py`

https://piaosanlang.gitbooks.io/spiders/07day/README7.html
https://github.com/rmax/scrapy-redis

## 进程

默认情况下，每个spider一个进程。
https://doc.scrapy.org/en/latest/topics/practices.html#running-multiple-spiders-in-the-same-process

## 测试

可以考虑用scrapy shell inspect辅助测试。 https://docs.scrapy.org/en/latest/topics/shell.html#topics-shell-inspect-response
或者定义Spider的子类，覆盖parse方法，校验当前的页面元素结构与代码中的parse逻辑是否一致；或者通过item pipeline等机制检查解析结果。

可以考虑用scrapy parse命令测试。 https://docs.scrapy.org/en/latest/topics/debug.html#parse-command

## http cache

https://docs.scrapy.org/en/latest/topics/downloader-middleware.html#module-scrapy.downloadermiddlewares.httpcache


## 其它

scrapy可以自动删除url中的fragment(named anchor)，如`abc.html?#1`。
https://github.com/scrapy/scrapy/blob/master/scrapy/utils/url.py

[css选择器](http://www.w3school.com.cn/cssref/css_selectors.asp)

### custom_settings无法继承

https://github.com/scrapy/scrapy/issues/3001

### 自动支持gzip压缩

即使设置了request header的其它参数，也会自动添加accept-encoding=gzip

### ssl错误问题

ubuntu 16.04下通过pip安装，运行时可能会报如下错误。原因是pip安装的pyopenssl和操作系统的openssl版本不一致。
解决办法如下：
```shell
sudo apt-get install libssl-dev
sudo pip install pyopenssl --upgrade
```
https://stackoverflow.com/questions/42573038/scrapy-module-object-has-no-attribute-op-single-ecdh-use
https://blog.csdn.net/syh_486_007/article/details/71600514

错误描述：
```
  File "/usr/local/lib/python2.7/dist-packages/twisted/internet/_sslverify.py", line 1583, in __init__
    self._options |= SSL.OP_SINGLE_DH_USE | SSL.OP_SINGLE_ECDH_USE
AttributeError: 'module' object has no attribute 'OP_SINGLE_ECDH_USE'
```



