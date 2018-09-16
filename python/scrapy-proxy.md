# scrapy-proxy

## 代理类型

https://www.coder4.com/archives/4434
https://www.cnblogs.com/craig/archive/2008/11/18/1335809.html

## proxy list

http://www.xicidaili.com/
https://www.kuaidaili.com/free/
https://www.zhihu.com/question/55807309
https://www.zhihu.com/question/47419361
https://www.zhihu.com/question/21235723

## 代理获取

https://github.com/qiyeboy/IPProxyPool
https://github.com/jhao104/proxy_pool
https://www.zhihu.com/question/47464143
https://www.zhihu.com/question/25566731

## 验证代理的使用

```python
# export http_proxy="http://114.113.126.87:80"
# scrapy shell
import json
fetch(scrapy.Request('http://httpbin.org/ip'))
json.loads(response.text)
fetch(scrapy.Request('https://httpbin.org/ip'))
json.loads(response.text)
```

## https proxy

```shell
export http_proxy="http://39.134.10.4:8080/"
export https_proxy="14.118.253.118:6666/"
curl "http://www.sina.com.cn/"
curl "https://www.baidu.com/"
```

## proxy provider

http://www.data5u.com/buy/long.html
http://www.goubanjia.com/buy/long.html
http://h.zhimaruanjian.com/newrecharge/
https://www.kuaidaili.com/pricing/#kps

蘑菇代理只有短时效的、动态代理ip。
data5u和goubanjia是一家。

## socks

scrapy无法直接使用socks
https://github.com/scrapy/scrapy/issues/747
https://blog.michaelyin.info/2014/02/19/scrapy-socket-proxy/

proxychains
https://www.hi-linux.com/posts/48321.html

### privoxy

https://blog.jessfraz.com/post/tor-socks-proxy-and-privoxy-containers
jess/privoxy

### delegated

```shell
./linux2.6-dg9.9.12 -P8080/http,8081/https SOCKS=104.219.251.135:24571
```

```
# redis
sadd proxy:http  172.16.0.50:8080
sadd proxy:https 172.16.0.50:8081
```

https://superuser.com/a/423573
https://hub.docker.com/r/vimagick/delegated/
http://www.delegate.org/delegate/Manual.shtml
http://www.delegate.org/delegate/HowToDG.html

### free socks proxy

https://www.socks-proxy.net/
https://sockslist.net/

### 本地安装socks代理

https://github.com/figroc/devops/blob/master/devel/dante.sh

