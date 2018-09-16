# ElasticSearch Install

version: 5.6.3

## config

```shell
# Virtual memory
# https://www.elastic.co/guide/en/elasticsearch/reference/current/vm-max-map-count.html
cat /proc/sys/vm/max_map_count
sudo sysctl -w vm.max_map_count=262144
sudo sh -c "echo 'vm.max_map_count=262144' >> /etc/sysctl.conf"

# Open files
# https://www.elastic.co/guide/en/elasticsearch/reference/master/setting-system-settings.html
# https://www.elastic.co/guide/en/elasticsearch/reference/current/file-descriptors.html
sudo sh -c "echo '* soft nofile 1048576' >> /etc/security/limits.conf'"
sudo sh -c "echo '* hard nofile 1048576' >> /etc/security/limits.conf'"
# /etc/pam.d/su /etc/pam.d/common-session
```
如果内存不够，需要修改`config/jvm.options`的参数`Xms/Xmx`。
如果允许其它主机访问，需要修改`elasticsearch.yml`的`network.host: 0.0.0.0`。

[ulimit -u 4096](https://www.elastic.co/guide/en/elasticsearch/reference/current/max-number-of-threads.html)


## run

`./bin/elasticsearch`

## shutdown

新版ES已经删除`_shutdown`API。应该通过`rpm/deb`安装为服务，通过服务管理启动、关闭。

## cluster

[网络配置文档](https://www.elastic.co/guide/en/elasticsearch/reference/current/modules-network.html)
如果要配置集群，要修改`elasticsearch.yml`中的`discovery.zen.ping.unicast.hosts`，并修改防火墙配置。




服务端默认keep_alive，客户端最好也设置keep_alive。




集群设置，节点如何接入集群，如何通信。分片个数，集群名称，节点名称。

索引构建，分词器设置
广告导入。广告的标签、索引设置
查询构造，与索引设置相关。包括时间、次数的过滤。
查询结果的排序


广告存入ES时，广告活动信息作为一个文档（JSON对象）存入，广告活动ID作为文档ID，广告位ID、广告主等信息可以作为字段。
如果是集群配置，请求应发给哪个IP？还是预定义IP列表轮换，如果某个ip失效了呢？
如何查询索引、type、文档的anylyzer、分词器、mapping？

took字段记录了处理毫秒数。P97
利用from 和 size 参数，可以实现多个批次的检索，避免一次装入内存。P101
P114 映射(mapping)机制用于进行字段类型确认，将每个字段匹配为一种确定的数据类型( string , number , booleans , date 等P117)。分析(analysis)机制用于进行全文文本(Full Text)的分词，以建立供搜索用的反向索引。
	查询文档mapping：`GET /gb/_mapping/tweet`
	核心数据类型(strings, numbers, booleans及dates)以不同的方式进行索引。P107
	https://www.elastic.co/guide/en/elasticsearch/reference/current/mapping-types.html
P106 查询时如果不指定字段，就是查询`_all`字段。
Elasticsearch中的数据可以大致分为两种类型：确切值 及 全文文本。P108
空格分析器P114/120。自定义分析器P173-175。除非我们告诉 Elasticsearch 在哪里使用，否则分析器不会起作用。P176。要显式定义某个type或field的分析器？
P117索引中每个文档都有一个类型(type)。 每个类型拥有自己的映射(mapping)或者模式定义(schema definition)。一个映射定义了字段类型，每个字段的数据类型，以及字段被Elasticsearch处理的方式。映射还用于设置关联到类型上的元数据。
type P177 类型 在 Elasticsearch 中表示一组相似的文档。类型 由一个 名称（比如 user 或blogpost ）和一个类似数据库表结构的映射组成
P120可以在第一次创建索引的时候指定映射的类型。此外，你也可以晚些时候为新类型添加映射
P121在创建索引时指定映射和分析器（分词）。
terms https://www.elastic.co/guide/cn/elasticsearch/guide/current/_finding_multiple_exact_values.html
range 过滤 P135
bool 过滤 P136。如果广告忽略某类标签，是否可用should的or逻辑，结合missing？missing已经被移除 https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-exists-query.html
	这样如果新增加标签类型，原有广告可以不受影响。但是如果广告忽略某标签、后台自动增加全部标签值，新增标签类型后，要自动增加原有所有广告。如果不自动增加，适合的用户就无法匹配该广告。
query & filter https://www.elastic.co/guide/en/elasticsearch/reference/current/query-filter-context.html
match 与 term 的区别是什么？P138.match 是query，term是filter？
search API中只能包含 query 语句。P139
The filtered query is replaced by the bool query. https://www.elastic.co/guide/en/elasticsearch/reference/current/query-dsl-filtered-query.html
sort 排序。P148
能否一次搜索所有的广告位？ https://www.elastic.co/guide/en/elasticsearch/client/java-api/current/java-search-msearch.html
日期格式 https://www.elastic.co/guide/en/elasticsearch/reference/current/mapping-date-format.html



=============================================================================
创建索引sxs，使用空白分词器、小写。应该可以写在yml配置中作为全局配置。
curl -XDELETE 'localhost:9200/sxs?pretty'
curl -XPUT 'localhost:9200/sxs' -d '
{

	"settings": {
		"number_of_shards" : 5,
		"number_of_replicas" : 3,
		"analysis": {
			"analyzer": {
				"ad_label": {
					"type": "custom",
					"tokenizer": "whitespace",
					"filter": ["lowercase"]
				}
			}
		}
	},
	"sxs": {
		"mappings": {
			"adcampaign": {
				"properties": {
					"posID": {
						"type": "string",
						"index": "not_analyzed"
					},
					"ontime": {"type": "long"},
					"offtime": {"type": "long"},
					"prc": {"type": "double"},
					"img": {"type": "string"},
					"redirect": {"type": "string"},
					"title": {"type": "string"},
					"desc": {"type": "string"},
					"label": {
						"type": "string",
						"analyzer": "ad_label"
					}
				}
			}
		}
	}
}
'





查询 132 131 135 140
curl -XPOST "localhost:9200/sxs/adcampaign/_search" -d '

'










