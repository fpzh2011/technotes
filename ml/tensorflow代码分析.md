# tensorflow源代码分析

TensorFlow技术内幕
https://www.jianshu.com/p/1dc3907596b3
数据并行模式中，模型在不同的设备上存在相同多份拷贝，共享相同的参数，采用不同的训练数据并行训练。根据共享参数的更新方式，又分为同步更新模式与异步更新模式。
模型并行模式中，是将模型的不同部分别放在不同的机器上进行训练。模型并行需要模型本身有大量的可以并行的，互相不依赖的或则依赖程度不高的子图。
流水线并行模式与数据并行模式类似，区别在于流水线并行模式是在单机上训练的。
https://blog.csdn.net/gaofeipaopaotang/article/details/80499025 编译与安装
https://blog.csdn.net/gaofeipaopaotang/article/details/80540630 源码结构
https://blog.csdn.net/gaofeipaopaotang/article/details/80540721 混合编程。主要讲python binding
https://blog.csdn.net/gaofeipaopaotang/article/details/80598840 核心概念的实现分析。这篇比较重要。
在我们的运算的定义中，没有定义运算的实现逻辑；然后在注册运算的时候，也没有提供任何运算实现的函数，这是为什么呢？
在tf的设计中，运算和运算实现是两个分开的概念，通过引入的运算核(OpKernel)的概念来表示运算的具体实现。
这么设计的原因是，运算的语义是平台不相关的，是不变的，而运算的实现运算核是跟具体的平台（CPU、GPU、TPU）相关的。
这样，就可以很方便的对语义不变的运算提供不同平台的实现了。
https://blog.csdn.net/gaofeipaopaotang/article/details/80621902 模型优化之Grappler
https://blog.csdn.net/gaofeipaopaotang/article/details/80679100 模型优化之XLA（上）
https://blog.csdn.net/gaofeipaopaotang/article/details/80703367 模型优化之XLA（下）
https://blog.csdn.net/gaofeipaopaotang/article/details/80736452 模型优化之分布式执行
https://blog.csdn.net/gaofeipaopaotang/article/details/81171832 梯度的计算。很重要，包括梯度逆向传播、自动求导的一些资料。
https://blog.csdn.net/gaofeipaopaotang/article/details/81186891 模型优化之量化（Quantize）
https://blog.csdn.net/gaofeipaopaotang/article/details/81335178 Estimator
https://blog.csdn.net/gaofeipaopaotang/article/details/88777678 模型保存与恢复

梯度下降等各种学习算法总结
https://blog.csdn.net/gaofeipaopaotang/article/details/81147064

这里是一个资料合集，有些内容和本文档的其它资料重复。但是也有一些不错的资料。
http://itren.xiaolee.net/p/1502735.html

Tensorflow代码解析
https://zhuanlan.zhihu.com/p/25646408
https://zhuanlan.zhihu.com/p/25927956
https://zhuanlan.zhihu.com/p/25929909
https://zhuanlan.zhihu.com/p/25932160
https://zhuanlan.zhihu.com/p/26031658

tensorflow源码分析
https://www.cnblogs.com/jimchen1218/p/11608640.html

tensorflow-internals
https://github.com/horance-liu/tensorflow-internals


实际执行模型加载卸载管理的是AspiredVersionsManager


疑问
* batching的作用是什么？对降低latency有没有作用？


https://raw.githubusercontent.com/horance-liu/tensorflow-internals/master/tensorflow-internals.pdf