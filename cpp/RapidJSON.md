# RapidJSON

## 类关系

* `Document`是utf-8的`GenericDocument`
* `GenericDocument`继承自`GenericValue`



```c++
typedef GenericDocument<UTF8<> > Document;

class GenericDocument : public GenericValue<Encoding, Allocator>
```




## 参考资料

http://rapidjson.org/zh-cn/
https://github.com/Tencent/rapidjson
