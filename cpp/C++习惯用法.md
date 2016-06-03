# C++习惯用法

## 智能指针资源释放

```cpp
std::shared_ptr<int> p = std::make_shared<int>(10);
p = nullptr;	//释放p对资源的引用计数。或者 p.reset();
```

http://stackoverflow.com/questions/16151550/c11-when-clearing-shared-ptr-should-i-use-reset-or-set-to-nullptr


