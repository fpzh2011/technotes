# ElasticSearch概要

## 索引

### 创建索引

`curl -XPUT 'localhost:9200/zjh?pretty'`

### 列出索引

`curl 'localhost:9200/_cat/indices?v'`

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

## 分词

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

# 查看分词方法
curl -s -G "localhost:9200/sxs/_search?explain&pretty&analyzer=index_ansj" --data-urlencode "q=计算机科学与技术软件工程" -o - > 1.txt
```



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

