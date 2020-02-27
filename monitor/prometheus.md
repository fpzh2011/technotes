# prometheus

https://blog.gmem.cc/prometheus-study-note




配置
https://prometheus.io/docs/prometheus/latest/configuration/configuration/
metric name and label naming 命名
https://prometheus.io/docs/practices/naming/



## METRIC TYPES

prometheus忽略metric type, 如counter/histogram
https://prometheus.io/docs/concepts/metric_types/

counter只会单调递增，不会减少；如果重启，counter reset为0。err等counter，应该是prometheus在处理query时做的数据处理。
客户端对不同type的metric，在数据输出时，是不是也有处理？比如counter和Gauge？

Gauge可用于表示任意升降的值，比如温度。

## HISTOGRAMS AND SUMMARIES

The essential difference between summaries and histograms is that summaries calculate streaming φ-quantiles on the client side and expose them directly, while histograms expose bucketed observation counts and the calculation of quantiles from the buckets of a histogram happens on the server side using the histogram_quantile() function.
https://prometheus.io/docs/practices/histograms/

## query

https://prometheus.io/docs/prometheus/latest/querying/basics/

```
# 耗时-正则表达式
histogram_quantile(0.99, sum(rate(module_request_duration_seconds_bucket{module=~"ssdfsf|asfsdf.*"}[1m])) by(le, module, component, method))
histogram_quantile(0.99, sum(rate(module_request_duration_seconds_bucket{module!~"ssdfsf|asfsdf.*"}[1m])) by(le, module, component, method))
# 基于histogram计算qps
sum(irate(phoenix_count{method_name="PD.phoenix.get_feed"}[1m]))
# 求avg
sum(delta(video_count_request_duration_seconds_sum{srv_name="recommend-model-server", method="Predict"}[1m]) / delta(video_count_request_duration_seconds_count{srv_name="recommend-model-server", method="Predict"}[1m])) / count(video_count_request_duration_seconds_count{srv_name="recommend-model-server", method="Predict"})
```

关于上述表达式的说明：
* 类似下面的histogram的bucket时间序列，是一个counter。
* rate函数的结果，是时点以前1分钟的增长情况: 1分钟内每秒增长的平均。因为counter通常是只增不减的，所以相当于是该bucket每秒平均请求个数。
* sum是operator，对各个维度label进行合计，但是保留le这个label，以及其它指定的label。le是lowe or equal。
* histogram_quantile计算分位点，并做必要的插值平滑。
* irate相对于rate，用于变化更剧烈的情况，比如qps。irate只看两个点，rate是所有点的平均。irate适合长尾场景。
```
api_request_ms_bucket{api="processRequest",step="item",le="5.000000"} 576234
```
https://prometheus.io/docs/concepts/metric_types/#histogram
https://prometheus.io/docs/prometheus/latest/querying/functions/#rate
https://prometheus.io/docs/prometheus/latest/querying/functions/#irate
https://prometheus.io/docs/prometheus/latest/querying/operators/#aggregation-operators
irate(v range-vector) calculates the per-second instant rate of increase of the time series in the range vector. This is based on the last two data points. Breaks in monotonicity (such as counter resets due to target restarts) are automatically adjusted for.

rate(v range-vector) calculates the per-second average rate of increase of the time series in the range vector. Breaks in monotonicity (such as counter resets due to target restarts) are automatically adjusted for. Also, the calculation extrapolates to the ends of the time range, allowing for missed scrapes or imperfect alignment of scrape cycles with the range's time period.



