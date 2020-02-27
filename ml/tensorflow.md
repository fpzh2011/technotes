# tensorflow

使用TensorFlow, 你必须明白TensorFlow:
* 使用图(graph)来表示计算任务.
* 在被称之为会话(Session)的上下文(context)中执行图.
* 使用tensor表示数据.
* 通过变量(Variable)维护状态.
* 使用feed和fetch可以为任意的操作(arbitrary operation)赋值或者从其中获取数据.
* 图中的节点被称之为op(operation的缩写).一个op获得0个或多个Tensor, 执行计算, 产生0个或多个Tensor.
* 会话将图的op分发到诸如CPU或GPU之类的设备上, 同时提供执行op的方法. 这些方法执行后, 将产生的tensor返回.

## tensor

https://www.tensorflow.org/guide/tensors

The eval method only works when a default tf.Session is active.
it is possible to feed any tf.Tensor, not just placeholders.
TensorFlow can't directly evaluate tf.Tensors defined inside functions or inside control flow constructs.

https://www.tensorflow.org/api_docs/python/tf/Tensor
A Tensor is a symbolic handle to one of the outputs of an Operation. It does not hold the values of that operation's output, but instead provides a means of computing those values in a TensorFlow tf.Session.

在一次run的调用过程中，可能会多次引用一个tensor，这期间的tensor是持久的。但是两次run的过程中，一个tensor不是持久的。

## variable

https://www.tensorflow.org/guide/variables
https://www.tensorflow.org/api_docs/python/tf/Variable
https://www.tensorflow.org/api_docs/python/tf/GraphKeys#GLOBAL_VARIABLES

Unlike tf.Tensor objects, a tf.Variable exists outside the context of a single session.run call.
Internally, a tf.Variable stores a persistent tensor. Specific ops allow you to read and modify the values of this tensor. These modifications are visible across multiple tf.Sessions, so multiple workers can see the same values for a tf.Variable.

If you need to create a variable with an initial value dependent on another variable, use the other variable's initialized_value(). This ensures that variables are initialized in the right order.

The convenience function trainable_variables() returns the contents of this collection. The various Optimizer classes use this collection as the default list of variables to optimize.

Avoid writing code which relies on the value of a Variable either changing or not changing as other operations happen. 

A variable is created when you first run the tf.Variable.initializer operation for that variable in a session. It is destroyed when that tf.Session.close
https://www.tensorflow.org/guide/faq

### Sharing variables

https://wizardforcel.gitbooks.io/hands-on-ml-with-sklearn-and-tf/content/docs/9.启动并运行_TensorFlow.html
https://www.tensorflow.org/guide/variables
https://www.tensorflow.org/api_docs/python/tf/variable_scope
https://www.tensorflow.org/api_docs/python/tf/get_variable

## session

概述资料
https://zhuanlan.zhihu.com/p/32869210

During a call to tf.Session.run any tf.Tensor only has a single value.

TensorFlow uses the tf.Session class to represent a connection between the client program and the C++ runtime.

https://www.tensorflow.org/guide/low_level_intro

Sessions can own resources, such as tf.Variable
https://www.tensorflow.org/guide/faq

### gpu_options.allow_growth

逐步增加内存

## tensorboard

The graph visualizer uses name scopes to group operations and reduce the visual complexity of a graph.

```
http://localhost:6006/#graphs

# https://www.tensorflow.org/guide/faq
python tensorflow/tensorboard/tensorboard.py --logdir=path/to/log-directory
```
https://www.tensorflow.org/guide/low_level_intro
https://www.tensorflow.org/guide/graph_viz
https://www.tensorflow.org/guide/graphs#visualizing_your_graph
https://www.tensorflow.org/guide/summaries_and_tensorboard

## device

设备用字符串进行标识. 目前支持的设备包括:
* "/cpu:0": 机器的 CPU.
* "/gpu:0": 机器的第一个 GPU, 如果有的话.
* "/gpu:1": 机器的第二个 GPU, 以此类推.

## Distributed TensorFlow

https://www.tensorflow.org/deploy/distributed
https://www.tensorflow.org/api_docs/python/tf/train/Server

## save & restore

### checkpoint

https://www.tensorflow.org/guide/saved_model

## TensorFlow serving 

https://www.tensorflow.org/serving/serving_basic

## 扩展

### 读取私有格式数据

My data is in a custom format. How do I read it using TensorFlow?
https://www.tensorflow.org/guide/faq

## tricks

```
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
sess_config = tf.ConfigProto(allow_soft_placement=True)
"config.gpu_options.allow_growth":True
```
https://www.zhihu.com/question/268375146/answer/357375938

## 参考资料

https://wizardforcel.gitbooks.io/hands-on-ml-with-sklearn-and-tf/content/docs/9.%E5%90%AF%E5%8A%A8%E5%B9%B6%E8%BF%90%E8%A1%8C_TensorFlow.html
https://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650744619&idx=1&sn=499a1d1c4ed757e33b97a97e11654d34


问题：
多次run之间，tensor会如何？
Layer 中，如何定义损失函数？如何确定真实输出y？
session与graph的关系，是一对一，还是一对多、多对多？一个session只能挂一个graph，一个graph可以挂到多个session？
动态与静态rnn的区别？
多个输出目标时，如何更新梯度？

https://wizardforcel.gitbooks.io/hands-on-ml-with-sklearn-and-tf/content/docs/10.人工神经网络介绍.html
用 TensorFlow 高级 API 训练 MLP

源码分析：
https://www.zhihu.com/question/41667903
https://blog.csdn.net/jxy0123456789/article/details/81084818
https://github.com/yao62995/tensorflow
