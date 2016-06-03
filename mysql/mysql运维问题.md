# mysql的C语言接口

## api文档(5.7)

如何避免InnoDB的共享表空间文件ibdata1文件大小暴增
http://mp.weixin.qq.com/s?__biz=MjM5NzAzMTY4NQ==&mid=405219884&idx=1&sn=f5ec1ebf36270d1facb44667bd8d613f&scene=1&srcid=0324xFhlLtcUs3KFXT77A5im&from=groupmessage&isappinstalled=0#wechat_redirect
ibdata1现在100多个G。如果用pt-osc就没事了。
事务也要及时提交。
开启独立表空间也有机会暴涨


