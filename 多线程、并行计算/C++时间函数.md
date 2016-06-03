# C++时间函数

## chrono头文件

chrono头文件定义与时间相关的类与函数。

duration表示一段时间，time_point表示一个时点。

## duration

duration 是一个类模板。标准库预定义了 hours/minutes/seconds 等duration的实例类。

## time_point 、 time_t 与时间运算

```cpp
time_point std::chrono::system_clock::now(); //返回表示当前时间的time_point对象
time_t std::chrono::system_clock::to_time_t(time_point&); //将time_point转换为C标准的time_t类型
time_point std::chrono::system_clock::from_time_t(time_t t); //将time_t转换为time_point对象

time_point tp;
tp += std::chrono::hours(24); //tp增加24小时
auto tp2 = tp;
auto d = tp2 - tp1; //d是一个duration对象
```

http://www.cplusplus.com/reference/chrono/
http://www.cplusplus.com/reference/chrono/time_point/operators/





