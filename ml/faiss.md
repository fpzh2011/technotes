# faiss

A library for efficient similarity search and clustering of dense vectors.
https://github.com/facebookresearch/faiss

## 优点

相对于itoi/utoi等：
* 可以准实时更新
* 相似向量检索策略可以有多种选择
* 支持itoi/utoi等多种策略模型

## 基本原理

* 每个item(视频、文章等)，需要先被预训练的模型转换为一个dense embedding向量，再存入向量搜索引擎的index。
* 检索时，先将query item转换为embedding向量，将其作为query向量，再输入给向量搜索引擎查询，返回若干最相似的item。
* 所以，模型在这里的输出不是score，而是一个vector。相对于搜索引擎来说，模型是预训练的。
* faiss支持搜索加速，但这意味着要对数据集进行一些预处理，Facebook把这个过程称为索引Index。
* faiss的优势在于，它支持十亿级的内存矢量检索，并在精确度和速度之间权衡。
* Faiss 配有自动调参机制，能扫描参数空间，并对提供了最佳操作点。
* R100@1000: 召回top 1000个，其中有多少个实际距离最近的top 100
https://www.infoq.cn/article/2017/11/Faiss-Facebook
https://cloud.tencent.com/developer/article/1077741
https://www.cnblogs.com/yhzhou/p/10568728.html

训练/构建索引过程
https://blog.razrlele.com/p/2594

## Product Quantization

把原来的向量空间分解为若干个低维向量空间的笛卡尔积，并对分解得到的低维向量空间分别做量化（quantization）。这样每个向量就能由多个低维空间的量化code组合表示。
PQ乘积量化能够加速索引的原理：查询样本与库中全部样本的距离计算，被转化为查询样本到子空间类中心的距离计算。
http://vividfree.github.io/机器学习/2017/08/05/understanding-product-quantization
http://www.fabwrite.com/productquantization
https://yongyuan.name/blog/ann-search.html

### Quantization 量化

简单说，就是将无限的连续值，用有限的离散值表示，而有尽量保持原始信息(如不同分量间的概率分布)。比如数据压缩、模型裁剪压缩。
https://zh.wikipedia.org/wiki/量化_(信号处理)
https://baike.baidu.com/item/Quantization
https://baike.baidu.com/item/矢量量化

## 工程实践

基于gRPC的Faiss server实践
https://juejin.im/post/5d5b8e7be51d4561f777e1b6

Tips
https://blog.razrlele.com/p/2594

### docker

https://hub.docker.com/r/daangn/faiss-server
https://github.com/daangn/faiss-server/blob/master/Dockerfile
https://hub.docker.com/r/daangn/faiss/dockerfile
https://github.com/daangn/faiss/blob/master/Dockerfile
`docker pull daangn/faiss-server`

### install

https://github.com/facebookresearch/faiss/blob/master/INSTALL.md
conda install faiss-cpu -c pytorch

### compile c++

https://github.com/facebookresearch/faiss/blob/master/INSTALL.md

```shell
yum install blas-devel lapack-devel openblas
git checkout v1.5.3
./configure --without-cuda
```

### 错误信息

Adding a numpy array of vectors to the index only works with contiguous arrays
https://github.com/facebookresearch/faiss/issues/459
https://zhuanlan.zhihu.com/p/59767914

### 开源实现

milvus
https://github.com/milvus-io/milvus
https://www.milvus.io/cn/docs/v0.6.0/faq/product_faq.md

京东vearch
https://github.com/vearch/vearch
https://www.infoq.cn/article/gxYOJ0m5rpMpIEwvz9GR

## 参考资料

https://github.com/facebookresearch/faiss/wiki
https://github.com/facebookresearch/faiss/tree/master/tutorial

Getting started:
* row-major storage
* 32-bit floating point

### API

C++
https://rawgit.com/facebookresearch/faiss/master/docs/html/annotated.html

[index读写操作](https://github.com/facebookresearch/faiss/issues/417)：
* write_index/read_index



## 疑问

faiss只支持整数id，但是我们的item id是string。


