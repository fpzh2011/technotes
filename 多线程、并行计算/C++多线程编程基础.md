# C++���̱߳�̻���

���ļ�����C++11��׼��

## ֻ���������̰߳�ȫ��

һ�� STL Container ���������һ��ʱ������T�ڣ���ֵ���ı䣬��ô��ʱ��T�ڣ����̲߳���**ֻ��**��������������̰߳�ȫ�ġ�

�ο���
http://en.cppreference.com/w/cpp/container
http://stackoverflow.com/questions/7455982/is-stl-vector-concurrent-read-thread-safe
https://msdn.microsoft.com/en-us/library/c9ceah3b.aspx
http://www.sgi.com/tech/stl/thread_safety.html
https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_concurrency.html
http://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm


ʹ��C++11��thread�ӿڣ�
`g++ -c -pthread test.cc`
`g++ -o test -thread`
��
`g++ -pthread test.cc`

ʹ��C��pthread�ӿڣ�
`g++ -c test.cc`
`g++ -o test -lpthread test.o`



## ԭ�����ͣ�C++11��

http://www.cnblogs.com/haippy/p/3301408.html
http://www.cnblogs.com/haippy/p/3252056.html
http://en.cppreference.com/w/cpp/atomic/atomic
http://www.cplusplus.com/reference/atomic/


## unique_lock vs lock_guard

lock_guard ֻ��һ���Եļӽ������ڹ��캯���м����������������н�����lock_guard��ҪΪ����RAII��ʽ��mutex���й����ṩ������
����lock_guard������������������

unique_lock���Ժ���������һ��ʹ�á�unique_lock�Ĺ��캯���У�����ѡ���Ǽ������ǲ�������std::adopt_lock, std::defer_lock�ȣ���
�ں���ʹ���У����Զ�һ��unique_lock�����μ��������������ٽ������и�ϸ���ȵĿ��ơ�
���⣬���Ժ�std::lock()���ʹ�ã��Զ������RAII��ʽ���й���

std::lock() �ɶԶ����ͬʱ���������Թ��������������

http://en.cppreference.com/w/cpp/thread/unique_lock
http://www.cplusplus.com/reference/mutex/unique_lock/
http://www.cplusplus.com/reference/mutex/unique_lock/unique_lock/
http://en.cppreference.com/w/cpp/thread/lock
http://stackoverflow.com/questions/20516773/stdunique-lockstdmutex-or-stdlock-guardstdmutex
http://stackoverflow.com/questions/17113619/whats-the-best-way-to-lock-multiple-stdmutexes/17113678#17113678

## thread ʹ��ע������

���������joinable�߳�û�е���join()�������˳�ʱ�ᱨ���쳣ת����`terminate called without an active exception`

��������ѡ�
`g++ -Wl,--no-as-needed -std=c++11 -pthread test.cc -o test`
�������лᱨ��`Enable multithreading to use std::thread: Operation not permitted`

http://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g

### ��������

C++��threadʵ�֣���ʹ��������Ϊ���ã�Ҳ����п��������ԣ����ڲ��ܡ����ʺϿ����Ķ��󣬾���ͨ������ָ�봫�ݣ���������ͨ����ָͨ�봫�ݡ�

���ڿ��ܽ��ж��̴߳�������ݣ�����ͨ������ָ�����


## �̲߳�������

by default the thread constructor will copy all arguments passed to it
http://stackoverflow.com/questions/10673585/start-thread-with-member-function
http://www.cplusplus.com/reference/thread/thread/thread/

## pthread��̬��������

C++11��thread�����̬���ӣ���Ҫָ��`-std=c++11 -static -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive`��
���򣬿��ܱ���
```
terminate called after throwing an instance of 'std::system_error'
  what():  Enable multithreading to use std::thread: Operation not permitted
�ѷ��� (������ת��)
```
��`�δ��� (������ת��)`��gdb���Ա���`Thread 2 "test" received signal SIGSEGV, Segmentation fault.`��

http://stackoverflow.com/questions/8649828/what-is-the-correct-link-options-to-use-stdthread-in-gcc-under-linux
https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52590#c4

## �����Ա��������/�����߳�

ʾ�����룺
```cpp
std::thread t1(&SomeClass::threadFunction, *this, arg1, arg2);
//����lamda���ʽ
std::thread( [this] { this->test(); } );
```

http://stackoverflow.com/questions/10673585/start-thread-with-member-function
http://stackoverflow.com/questions/21059115/c11-thread-class-how-to-use-a-class-member-function
http://stackoverflow.com/questions/15734598/how-to-execute-a-class-member-function-in-a-separate-thread-using-c11-thread-c

## �������

https://github.com/dougbinks/enkiTS









