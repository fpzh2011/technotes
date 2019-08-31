# go-micro

## consule

brew install consul
consul agent -dev
consul members
http://localhost:8500/

## micro

export GOBIN=$HOME/bin/go
export GOPROXY=https://goproxy.io
go mod init zjh/test
go get github.com/micro/micro

## protobuf

brew install protobuf
go get github.com/micro/protobuf/{proto,protoc-gen-go}
go get github.com/micro/protoc-gen-micro


## proto file

```proto
syntax = "proto3";
service Hello {
    rpc Ping(Request) returns (Response) {}
}
message Request {
    string name = 1;
}
message Response {
    string msg = 1;
}
```

```shell
protoc --go_out=plugins=micro:. ./proto/hello.proto
```

## 参考资料

https://segmentfault.com/a/1190000017572032
https://micro.mu/docs/cn/go-micro.html



## 疑问

如何指定consule地址？

