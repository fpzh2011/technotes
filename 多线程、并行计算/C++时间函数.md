# C++ʱ�亯��

## chronoͷ�ļ�

chronoͷ�ļ�������ʱ����ص����뺯����

duration��ʾһ��ʱ�䣬time_point��ʾһ��ʱ�㡣

## duration

duration ��һ����ģ�塣��׼��Ԥ������ hours/minutes/seconds ��duration��ʵ���ࡣ

## time_point �� time_t ��ʱ������

```cpp
time_point std::chrono::system_clock::now(); //���ر�ʾ��ǰʱ���time_point����
time_t std::chrono::system_clock::to_time_t(time_point&); //��time_pointת��ΪC��׼��time_t����
time_point std::chrono::system_clock::from_time_t(time_t t); //��time_tת��Ϊtime_point����

time_point tp;
tp += std::chrono::hours(24); //tp����24Сʱ
auto tp2 = tp;
auto d = tp2 - tp1; //d��һ��duration����
```

http://www.cplusplus.com/reference/chrono/
http://www.cplusplus.com/reference/chrono/time_point/operators/





