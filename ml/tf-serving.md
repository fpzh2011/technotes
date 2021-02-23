# tf-serving

https://sthalles.github.io/serving_tensorflow_models/

## protobuf

proto结构:
* tensorflow_serving/apis/prediction_service.proto
  * PredictionService: 
  * rpc Predict(PredictRequest) returns (PredictResponse)
* tensorflow_serving/apis/predict.proto
  * PredictRequest 
  * PredictResponse
* tensorflow_serving/apis/model.proto
  * ModelSpec
* tensorflow/core/framework/tensor.proto
  * TensorProto
  * VariantTensorDataProto
* tensorflow/core/framework/types.proto
  * DataType
* tensorflow/core/framework/tensor_shape.proto
  * TensorShapeProto
    * Dim
* tensorflow/core/framework/resource_handle.proto
  * ResourceHandleProto
    * DtypeAndShape

### 结构细节

PredictRequest
* model_spec: 至少要填name
* output_filter: 其中包含的output才会被计算并返回
* inputs
  * key: feature_name
  * value: feature_name的所有item的特征值
* TensorProto
  * dtype: 数据类型。比如sparse/sequence是int64，dense是float或double
  * tensor_shape: 一般是2维，也就是两个Dim。第一个是item size，第二个是特征实际长度。所有item的长度相同，因为value是一维数组，需要固定的dim确定各个item的特征值。
  * 对于sequence/dense等含有多值的特征，一个item的特征连在一起。

PredictResponse.outputs
* key: output_name。一个model可以有多个output，比如ctr, cvr
* value: 对于某个output_name，所有的items的预估score都在TensorProto中，比如float_val。item的顺序与PredictRequest.inputs一致。
* tensor_shape: 

## REST API

检查tf-serving加载模型是否成功
curl http://localhost:8503/v1/models/dnn_model_t22
wget -O - http://localhost:8503/v1/models/dnn_model_t1
wget -O - http://localhost:8503/v1/models/dnn_model_t1/metadata
wget -O - http://localhost:8503/v1/models/dnn_model_t1/versions/20201102/metadata

wget -O - http://localhost:8503/v1/models/dnn_model_t45/versions/20210124/metadata | \
jq --tab ".metadata.signature_def.signature_def.serving_default.inputs" | grep tensor_shape | wc -l

https://www.tensorflow.org/tfx/serving/api_rest

## docker

https://www.jianshu.com/p/afe80b2ed7f0

```shell
git clone https://github.com/tensorflow/serving --recursive
git clone https://github.com/tensorflow/tensorflow --recursive

# enter tensorflow docker, mount serving repo
python tensorflow_serving/example/mnist_saved_model.py models/mnist

docker stack deploy -c docker-compose.yml serving

# docker exec -it tensorflow bash
apt-get install -y ipython3
pip install grpcio-tools grpcio ipython
cd /mnt
python -m grpc_tools.protoc -I. -I/tensorflow --python_out=. --grpc_python_out=. tensorflow_serving/apis/*.proto

# import tensorflow.compat.v1 as tf
python tensorflow_serving/example/mnist_client.py --num_tests=10 --server=serving:8500
```

request.inputs['images'].CopyFrom(
    tf.make_tensor_proto(image, shape=[1, image.size]))
stub.Predict(request)

## signature_name

signature_def_map
https://zhuanlan.zhihu.com/p/113527219

## 性能优化

爱奇艺 - 推荐业务中TF Serving模型更新毛刺的完全优化实践
https://www.mdeditor.tw/pl/p5fS
https://mp.weixin.qq.com/s/vw3CMAc7ZDdm2JKcZuW4wg

inter_op_parallelism_threads, intra_op_parallelism_threads
https://stackoverflow.com/questions/41233635/meaning-of-inter-op-parallelism-threads-and-intra-op-parallelism-threads

## 编译

tensorflow_serving/g3doc/setup.md
一般设置`--config=nativeopt`开启处理器支持的所有指令优化。

修改代码编译的注意事项，以及一些优化tips
https://farer.org/2020/12/28/tensorflow-serving/

### prometheus定制

https://yuerblog.cc/2021/01/15/tensorflow-serving二次开发-增加模型流量监控/

### saved_model

编译saved_model protobuf，在tensorflow根目录下执行
```
python -m grpc_tools.protoc -I. \
  --python_out=src --grpc_python_out=src \
  tensorflow/core/framework/graph.proto \
  tensorflow/core/framework/op_def.proto \
  tensorflow/core/framework/tensor_shape.proto \
  tensorflow/core/framework/types.proto \
  tensorflow/core/protobuf/meta_graph.proto \
  tensorflow/core/protobuf/saved_object_graph.proto \
  tensorflow/core/protobuf/saved_model.proto \
  tensorflow/core/protobuf/saver.proto \
  tensorflow/core/protobuf/struct.proto
```

查看saved_model，在tensorflow docker中运行，需要tensorflow python package
```python
import sys

from google.protobuf.json_format import MessageToJson
from tensorflow.core.protobuf.saved_model_pb2 import SavedModel

def main():
    model = SavedModel()
    model.ParseFromString(sys.stdin.buffer.read())  # for python3
    print(MessageToJson(model))

if __name__ == '__main__':
    main()
```

saved_model的输入、输出
```shell
# inputs
jq '.metaGraphs[0].signatureDef.serving_default.inputs' saved_model.json | jq keys
# outputs
jq '.metaGraphs[0].signatureDef.serving_default.outputs' saved_model.json
```

## 配置

https://www.tensorflow.org/tfx/serving/serving_config#monitoring_configuration

## 监控

例子
tensorflow_serving/util/prometheus_exporter_test.cc
tensorflow_serving/servables/tensorflow/saved_model_warmup.cc
```c++
#include "tensorflow/core/lib/monitoring/counter.h"
#include "tensorflow/core/lib/monitoring/gauge.h"
#include "tensorflow/core/lib/monitoring/sampler.h"
```

## graph protobuf

./tensorflow/core/framework/graph.proto
./tensorflow/core/protobuf/saved_model.proto


## 疑问

request.model_spec.signature_name
SavedModel提供了SignatureDefs，简化了这一过程。SignatureDefs定义了一组TensorFlow支持的计算签名，便于在计算图中找到适合的输入输出张量。简单的说，使用这些计算签名，可以准确指定特定的输入输出节点。
https://zhuanlan.zhihu.com/p/60685482

MODEL_NAME环境变量可以设置多个模型吗？
只能加载一个模型？ --model_name=mnist --model_base_path=/models/mnist

可视化浏览一个模型，包括inputs/outputs




## 代码分析

tensorflow_serving/model_servers/server.h 这个是整体server

tensorflow_serving/model_servers/model_service_impl.h 应该是检索模型状态、管理模型更新的

tensorflow_serving/model_servers/prediction_service_impl.h 这个应该是预测服务，还可以返回model元数据
* 在 tensorflow_serving/model_servers/server.cc 中构造
* 实际执行预测的是 tensorflow_serving/servables/tensorflow/predict_impl.h

tensorflow_serving/servables/tensorflow/predict_impl.h
* 












