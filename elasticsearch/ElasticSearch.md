# ElasticSearch概要

## 索引

### 创建索引

`curl -XPUT 'localhost:9200/zjh?pretty'`

### 列出索引

`curl 'localhost:9200/_cat/indices?v'`

### 索引的打开、关闭

https://www.elastic.co/guide/en/elasticsearch/reference/current/indices-open-close.html
```shell
curl -XPOST 'localhost:9200/my_index/_close?pretty'
curl -XPOST 'localhost:9200/my_index/_open?pretty'
```

### 查看mapping

```shell
curl -XGET 'localhost:9200/sxs/_mapping/_all?pretty'
curl -XGET 'localhost:9201/intern/_mapping/intern?pretty'
```
https://www.elastic.co/guide/en/elasticsearch/reference/current/indices-get-mapping.html

### 修改mapping

https://www.elastic.co/guide/en/elasticsearch/reference/current/indices-put-mapping.html

### 删除索引

`curl -XDELETE 'localhost:9200/zjh?pretty'`

## 文档

### 创建文档

```shell
curl -XPUT 'localhost:9200/zjh/test/999?pretty' -d '
{
"months": [{"info": "1"}, {"info": "2"}], 
"new_name": "zjh"
}'
```

### 查看文档

`curl -XGET 'localhost:9200/zjh/test/999?pretty'`

### 删除文档

`shell
# 删除单个文档
curl -XDELETE 'localhost:9200/zjh/test/999?pretty'
# 删除type的所有文档
# https://stackoverflow.com/questions/23917327/delete-all-documents-from-index-type-without-deleting-type
curl -XPOST 'localhost:9200/sxs/resume4w/_delete_by_query' -d' { "query": { "match_all": {} } }'
`

### 统计文档个数

`curl -XGET 'localhost:9200/zjh/test/_count?pretty'`

## 查询

```shell
# 限定字段major
curl -s -G "localhost:9200/sxs/resume4w/_search?pretty" --data-urlencode "q=doc.major:通信" -o - | more
# 不限定字段
curl -s -G "localhost:9200/sxs/resume4w/_search?pretty" --data-urlencode "q=分析" -o - 
# 不需要urlencode
curl -s "localhost:9200/sxs/resume4w/_search?pretty&q=java" -o - | more
# 设置结果记录条数、范围
curl -s -G "localhost:9200/sxs/resume4w/_search?pretty" --data-urlencode "size=20" --data-urlencode "q=分析" -o -
curl -s -G "localhost:9200/sxs/resume4w/_search?pretty&size=20" --data-urlencode "q=分析" -o -
curl -s -G "localhost:9200/sxs/resume4w/_search?pretty&from=10&size=20" --data-urlencode "q=分析" -o -
```

### 查询方式

#### URI Search

[URI Search](https://www.elastic.co/guide/en/elasticsearch/reference/current/search-uri-request.html)
缺点是不支持全部的查询特性。

#### Request Body Search

[Request Body Search](https://www.elastic.co/guide/en/elasticsearch/reference/current/search-request-body.html)

#### Search Template

[Search Template](https://www.elastic.co/guide/en/elasticsearch/reference/current/search-template.html)
如果使用`Conditional clauses`，因为模板不再是标准的json格式，所以要么以ES script文件形式定义模板，要么使用字符串转义。查询仍是标准的json格式。
https://stackoverflow.com/questions/26606853/elasticsearch-search-templates

#### nested query

https://www.elastic.co/guide/en/elasticsearch/reference/current/nested.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-nested-query.html
nested查询结合外部文档的过滤
https://stackoverflow.com/questions/35652661/combine-nested-query-with-filter

### explain

https://www.elastic.co/guide/en/elasticsearch/reference/current/search-explain.html
```shell
curl -XPOST "localhost:9200/ik/intern/_search?explain&pretty" -H "Content-Type: application/json"  -d '
{
	"query": {
		"match": {"name" : "产品经理"}
	}
}'
```

### 查询操作符

https://www.elastic.co/guide/cn/elasticsearch/guide/current/bool-query.html#_%E6%8E%A7%E5%88%B6%E7%B2%BE%E5%BA%A6
* must: 所有条件必须满足
* should: 如果有must，所有should都可以不满足（or）。如果没有must，只好一个should满足。更准确的说，是用过参数`minimum_should_match`控制。

## type

type在ES6为deprecated， 将在ES7中删除。
https://www.elastic.co/guide/en/elasticsearch/reference/current/removal-of-types.html
在同一个index内，不同types的同名field，底层对应同一个Lucene field。所以，将type视为关系数据库的table是一种误解。
换句话说，同一index下同名的field，必须有相同的定义。

一个index下的entities最好有相近的fields。

you can implement your own custom `type` field which will work in a similar way to the old `_type`.

## 分词

查看文档的term vertors`_termvectors`
https://www.elastic.co/guide/en/elasticsearch/reference/5.6/docs-termvectors.html
```shell
curl -X GET "localhost:9200/twitter/tweet/1/_termvectors"
curl -X GET "localhost:9200/twitter/tweet/1/_termvectors?fields=message"
curl -X GET "localhost:9200/mtg2nzuk/material/1/_termvectors?pretty" -H 'Content-Type: application/json' -d'{"fields" : ["名称"]}'
```

## ik分词

https://github.com/medcl/elasticsearch-analysis-ik

有三个分词器，ik_syno, ik_base_smart, ik_syno_smart
ik_syno分词是最细致的分词，比如 “微信公众号的运营” 会分成 “微信”，“公众号”，“公众”，“号”，“的”， “运营”。
ik_base_smart是比较合理的分词，不会出现细粒度重复的token，比如上面的 “微信公众号的运营” 会分成 “微信”，“公众号”，“的”， “运营”。
ik_syno_smart 会在 ik_base_smart的基础上 过滤掉不需要的token，只保留关键词，所以 “微信公众号的运营” 会得到 “微信”，“公众号”，“运营”。

### ansj分词

https://github.com/NLPchina/elasticsearch-analysis-ansj

查询指定分词方法：
```shell
curl -s -G "localhost:9200/sxs/_search?pretty&analyzer=index_ansj" --data-urlencode "q=计算机科学与技术软件工程" -o - > 1.txt
# 不指定分词方法，与index_ansj一致
curl -s -G "localhost:9200/sxs/_search?pretty" --data-urlencode "q=计算机科学与技术软件工程" -o - > 2.txt

curl -s -G "localhost:9200/sxs/_search?pretty&analyzer=query_ansj" --data-urlencode "q=计算机科学与技术软件工程" -o - > 3.txt
curl -s -G "localhost:9200/sxs/_search?pretty&analyzer=dic_ansj" --data-urlencode "q=计算机科学与技术软件工程" -o - > 4.txt

# 以下3种分词方法，返回 analyzer xxxx not found 。也许是配置文件没有设置。
curl -s -G "localhost:9200/sxs/_search?pretty&analyzer=to_ansj" --data-urlencode "q=计算机科学与技术软件工程" -o - > 5.txt
curl -s -G "localhost:9200/sxs/_search?pretty&analyzer=user_ansj" --data-urlencode "q=计算机科学与技术软件工程" -o - > 6.txt
curl -s -G "localhost:9200/sxs/_search?pretty&analyzer=search_ansj" --data-urlencode "q=计算机科学与技术软件工程" -o - > 7.txt

# 查看分词结果
curl -XPOST 'localhost:9201/intern/_analyze?pretty' -H "Content-Type: application/json" -d'
{
  "analyzer": "ik_max_word",
  "text": "硕士abc DEF 产品经理"
}'
```

## function_score & script

https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-function-score-query.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/search-request-script-fields.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/modules-scripting-fields.html

https://stackoverflow.com/questions/33345878/what-is-the-best-way-to-query-the-document-closest-to-a-date-time-on-elasticsear
https://www.elastic.co/guide/en/elasticsearch/reference/current/modules-scripting.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/search-request-sort.html#_script_based_sorting
https://www.elastic.co/guide/en/elasticsearch/painless/6.2/index.html

## synonym

https://www.elastic.co/guide/en/elasticsearch/reference/5.6/analysis-synonym-tokenfilter.html
https://www.elastic.co/guide/en/elasticsearch/guide/current/synonyms.html
https://www.elastic.co/guide/en/elasticsearch/guide/current/synonym-formats.html
https://www.elastic.co/guide/en/elasticsearch/guide/current/using-synonyms.html
https://www.elastic.co/guide/en/elasticsearch/guide/current/synonyms-expand-or-contract.html

## suggester 搜索关键词提示

https://www.elastic.co/guide/en/elasticsearch/reference/current/search-suggesters.html
* term suggester类似拼写纠正
* completion suggester自动补全，可以从文本中间开始匹配。但返回的是完整的提示文本。如果要返回提示文本的前半部分，需要自己处理。





## 客户端选择

https://www.elastic.co/blog/found-interfacing-elasticsearch-picking-client

## 常用参数

### discovery.type

`discovery.type=single-node`一般用于测试客户端，这种情况下，节点选举自身作为master，不会加入其它节点的集群。
https://www.elastic.co/guide/en/elasticsearch/reference/current/bootstrap-checks.html#single-node-discovery
https://www.elastic.co/guide/en/elasticsearch/reference/current/modules-discovery-zen.html

### Index Modules

https://www.elastic.co/guide/en/elasticsearch/reference/current/index-modules.html

`index.max_result_window`: `from+size`的最大值，默认是10000。
`index.max_rescore_window`: rescore的window_size最大值。
`index.refresh_interval`: 数据变动对搜索可见的时间间隔，默认是1秒。

### bootstrap.memory_lock

ElasticSearch 如果设置 `bootstrap.memory_lock: true` ，会禁止使用swap，如果heap size不超限，性能会很好。但是一旦heap内存不够，JVM会退出。
在`Xmx20g`的配置、超过1k qps高并发查询的场景下，观察到`java.lang.OutOfMemoryError: Java heap space`
ElasticSearch 的 Xmx 和 Xms 需要设置相同值，否则启动时`bootstrap checks failed`。官方文档建议不要超过物理内存的一半。
https://www.elastic.co/guide/en/elasticsearch/reference/master/heap-size.html
https://github.com/elastic/elasticsearch/issues/18462

不使用swap
https://www.elastic.co/guide/en/elasticsearch/reference/current/important-settings.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/setup-configuration-memory.html#mlockall

https://www.elastic.co/guide/en/elasticsearch/reference/current/important-settings.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/setup-configuration-memory.html#mlockall
https://www.elastic.co/guide/en/elasticsearch/reference/current/_memory_lock_check.html
https://timonweb.com/posts/elasticsearch-fails-with-error-this-can-result-in-part-of-the-jvm-being-swapped-out-increase-rlimit_memlock-ulimit-what-to-do/
Increase RLIMIT_MEMLOCK, soft limit: 65536, hard limit: 65536
https://www.elastic.co/guide/en/elasticsearch/reference/5.6/docker.html

## 集群Cluster管理

### 节点类型

https://www.elastic.co/guide/en/elasticsearch/reference/current/modules-node.html
https://www.elastic.co/guide/en/elasticsearch/reference/5.4/modules-node.html
Master Electionedit
https://www.elastic.co/guide/en/elasticsearch/reference/current/modules-discovery-zen.html

### 查看节点

https://www.elastic.co/guide/en/elasticsearch/reference/current/cluster-nodes-info.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/modules-node.html
```shell
curl -XGET 'localhost:9200/_nodes?pretty'
curl -XGET 'localhost:9200/_nodes/nodeId1,nodeId2?pretty'
curl -XGET 'http://localhost:9200/_cluster/state?pretty'
```

## 优化

### ES的内存设置优化

* 确保Xms与Xmx是相同的，防止程序在运行时改变堆内存大小， 这是一个很耗系统资源的过程。
* 把50%的可用内存作为Elasticsearch的堆内存。把内存的（少于）另一半给 Lucene。
* 堆内存不要超过32G，以免java内存指针压缩失效(compressed oops)。
* 堆内存不要高于26/30G，以免zero-based compressed oops失效。
https://www.elastic.co/guide/cn/elasticsearch/guide/current/heap-sizing.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/heap-size.html

### 通用优化

优化经典文章
https://www.elastic.co/blog/found-optimizing-elasticsearch-searches
https://www.elastic.co/blog/found-elasticsearch-in-production

http://zhaoyanblog.com/archives/319.html
https://www.jianshu.com/p/4c57a246164c
https://wenku.baidu.com/view/82b082b5998fcc22bcd10df4.html
https://www.easyice.cn/archives/207

OS设置：
https://www.elastic.co/guide/cn/elasticsearch/guide/current/_file_descriptors_and_mmap.html
https://www.elastic.co/guide/cn/elasticsearch/guide/current/heap-sizing.html

资源消耗
https://www.elastic.co/guide/en/elasticsearch/reference/master/tune-for-search-speed.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/cluster-nodes-stats.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/cat.html

## 日期date加减计算

https://www.elastic.co/guide/en/elasticsearch/reference/current/common-options.html#date-math
https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-range-query.html#ranges-on-dates

## 聚合

https://www.elastic.co/guide/cn/elasticsearch/guide/current/aggregations.html
https://www.elastic.co/guide/en/elasticsearch/reference/current/search-aggregations.html

## 待研究

size参数

web查询工具
http://mobz.github.io/elasticsearch-head/

建text index的时候配置tokenizer，retrieval model
https://www.elastic.co/guide/en/elasticsearch/guide/current/scoring-theory.html

分词器
http://www.cnblogs.com/xing901022/p/5910139.html
https://github.com/medcl/elasticsearch-analysis-ik

为Elasticsearch添加中文分词，对比分词器效果
http://keenwon.com/1404.html
Elasticsearch——分词器对String的作用
http://www.cnblogs.com/xing901022/p/5235993.html
elasticsearch 查询
http://www.cnblogs.com/yjf512/p/4897294.html
play的分词插件。是否需要修改配置
https://github.com/NLPchina/elasticsearch-analysis-ansj

nested object
https://stackoverflow.com/questions/34882915/json-fields-imports-from-postgres-to-elasticsearch
http://blog.csdn.net/allenalex/article/details/45044107
https://www.elastic.co/guide/en/elasticsearch/guide/current/nested-objects.html
https://stackoverflow.com/questions/8140651/how-to-search-nested-objects-with-elastic-search


## 参考资料

http://blog.csdn.net/cnweike/article/details/33736429
http://www.ruanyifeng.com/blog/2017/08/elasticsearch.html

