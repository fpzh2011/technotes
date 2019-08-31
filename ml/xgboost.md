# xgboost

## 特点

* 二阶梯度，收敛快
* OpenMP
* 正则化项避免过拟合
* xgboost 的模型和传统的 GBDT 相比加入了对于模型复杂度的控制以及后期的剪枝处理，使得学习出来的模型更加不容易过拟合
* [supports missing value by default](https://xgboost.readthedocs.io/en/latest/faq.html)

## 参数

https://xgboost.readthedocs.io/en/latest/parameter.html

### scale_pos_weight 

https://blog.csdn.net/h4565445654/article/details/72257538
https://stats.stackexchange.com/questions/243207/what-is-the-proper-usage-of-scale-pos-weight-in-xgboost-for-imbalanced-datasets

## incremental training

https://github.com/dmlc/xgboost/issues/3055
https://gist.github.com/ylogx/53fef94cc61d6a3e9b3eb900482f41e0

## xgboost4j

https://xgboost.readthedocs.io/en/latest/jvm/
http://dmlc.ml/docs/javadocs/index.html?ml/dmlc/xgboost4j/java/XGBoost.html
https://github.com/dmlc/xgboost/tree/master/jvm-packages/xgboost4j-example

## 参考资料

https://cosx.org/2015/03/xgboost/
https://xgboost.readthedocs.io/en/latest/
https://github.com/dmlc/xgboost/tree/master/demo



## 问题

* 正则化项w
* 一阶导数是对f(x)？为什么？
* 二阶导数
* num_round 参数是什么意思？xgboost也有训练轮数吗？还是指树的数量？
* 只在cli版本设置树的数量，其余版本都是自动确定树的数量？



https://xgboost.readthedocs.io/en/latest/parameter.html
https://xgboost.readthedocs.io/en/latest/cli.html
https://homes.cs.washington.edu/~tqchen/pdf/BoostedTree.pdf


