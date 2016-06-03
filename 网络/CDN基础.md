# CDN基础

## 为什么DNS对CDN效果很重要

CDN中的智能DNS对服务器调度很关键。

而客户端使用的DNS，对客户的使用效果同样关键。因为一般都是递归DNS查询，最终是客户端的本地DNS向CDN智能DNS发起域名查询。
如果客户是电信的线路，但使用了联通的DNS，CDN的智能DNS可能会返回联通的节点IP，导致访问速度慢。

## 回源IP

CDN的缓存节点上如果没有请求的内容，会向源站发起资源获取请求。
可能不是CDN的缓存节点向源站发起请求，而是由一组固定的回源服务器向源站发起请求。这些回源服务器的IP，就是回源IP。

阿里云的CDN配置说明中，明确指出，如果源站服务器配置了安全狗等系统，需要在白名单中添加回源IP，否则可能回源失败。

## 参考资料

	http://baike.baidu.com/view/10698702.htm
	http://cndefu.blog.163.com/blog/static/593931882011422115513240/
	http://history.programmer.com.cn/12144/
	http://docs.upyun.com/cdn/
	http://14294725.blog.hexun.com/100803883_d.html
	http://up-file.b0.upaiyun.com/article/dynamic_cdn.html
	https://help.aliyun.com/knowledge_detail/6697641.html

