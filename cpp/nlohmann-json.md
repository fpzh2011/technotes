# nlohmann-json说明

## 引用

关于引用的几点说明
* 引用声明时必须带`&`
* `iterator.value()`和`operator[]`可返回引用，修改这个引用会改变json内的值。int等也是这样
* 默认情况下，`j["string"]`的类型就是string，不用auto、直接声明为string也可以
* 如果j本身就是一个string，`j.get_ref<std::string&>()`可以返回引用
```c++
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

int main() {
  auto j = nlohmann::json::parse(R"(
    {
      "string": "abc",
      "array": [1, 2],
      "object": {"o": 1},
      "int": 1
    }
  )");
  // 返回引用，两个引用的地址一致
  const auto& str = j["string"];
  std::cout << "addr " << &str << std::endl;
  auto& str2 = j["string"];
  std::cout << "addr " << &str2 << std::endl;
  // 修改引用的值，会改变json内的值
  str2 = "zjh";
  std::cout << j.dump() << std::endl;

  // iterator
  auto it = j.find("string");
  auto itv = it.value();
  std::cout << "itv addr " << &itv << std::endl;
  auto& itv2 = it.value();
  std::cout << "itv addr " << &itv2 << std::endl;
  std::cout << "is ref " << std::is_reference<decltype(itv2)>::value << std::endl;
}
```
https://github.com/nlohmann/json/issues/91

