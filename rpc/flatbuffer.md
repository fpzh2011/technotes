# flatbuffer

## [概述](https://google.github.io/flatbuffers/index.html#flatbuffers_overview)

* 类似protobuf
* 一般的项目只需要依赖header file
* 需要flatc将schema文件编译为源代码

## [install](https://google.github.io/flatbuffers/flatbuffers_guide_building.html)

安装内容：
* flatc
* header files
* libflatbuffers.a 800+kB for [parser or reflection](https://github.com/google/flatbuffers/issues/4008)

macos:
```shell
brew install flatbuffers
flatc
```

centos:
```shell
git clone -b v1.11.0 https://github.com/google/flatbuffers.git
cd flatbuffers
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make
make install
```

python:
```shell

```



TODO:
* 编写vector的idl文件
* 生成python代码
* 输出float32到文件
* 生成c++代码
* 从文件读取float32
* c++解析为vector<float>

比较与protobuf的差异。

测试protobuf是否支持移动语义。
flatbuffer呢？
