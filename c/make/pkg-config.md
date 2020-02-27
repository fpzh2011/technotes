# pkg-config

http://linux.die.net/man/1/pkg-config

pkg-config最常用的使用方式，是输出库的头文件、库文件位置。例如:
* `pkg-config --cflags protobuf grpc`输出头文件位置`-pthread -DCARES_STATICLIB -I/usr/local/include`
* `pkg-config --libs protobuf grpc`输出库文件位置`-pthread -L/usr/local/lib -lprotobuf -lgrpc`

通常把cflags放到CPPFLAGS，如:
```makefile
CPPFLAGS += `pkg-config --cflags protobuf grpc log4cplus` -I./common/
```
把libs放到LDFLAGS，如:
```makefile
LDFLAGS += `pkg-config --libs protobuf grpc++ grpc`\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed
```

## PKG_CONFIG_PATH

man 1 pkg-config

pkg-config通过环境变量PKG_CONFIG_PATH，读取库配置文件，如grpc.pc:
```
prefix=/usr/local
exec_prefix=${prefix}
includedir=${prefix}/include
libdir=${exec_prefix}/lib

Name: gRPC
Description: high performance general RPC framework
Version: 7.0.0
Cflags: -I${includedir}
Requires.private: gpr  zlib libcares openssl
Libs: -L${libdir} -lgrpc
Libs.private:
```
注意其中的字段:
* Name。命令行中指定的package name，是pc文件名。
* Cflags
* Libs

## 参考资料

https://blog.csdn.net/hunanchenxingyu/article/details/17358265




如何让自己的库生成pc配置文件？比如cmake？



