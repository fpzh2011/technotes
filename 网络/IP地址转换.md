# IP地址转换

## IP地址转换

```c
#include <arpa/inet.h>

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

int inet_pton(int af, const char *src, void *dst);
```

`inet_ntop`函数用于将地址的网络表示，转换为打印形式。其中
af 是`AF_INET`或`AF_INET6`；
src 是指向`struct in_addr`或`struct in6_addr`的指针；
dst 是存储转换后的字符串的缓冲区地址；
size 是缓冲区dst的长度，`INET_ADDRSTRLEN`和`INET6_ADDRSTRLEN`分别表示ipv4和ipv6地址打印形式的最大长度。

`inet_pton`用于将地址的打印形式，转换为网络形式。其中
src 是存储IP地址的字符串的地址；
dst 是指向`struct in_addr`或`struct in6_addr`的指针，用于存储转换后的结果。

### 参考代码

```cpp
#include <arpa/inet.h>
#include <iostream>

int main(int argc, char *argv[]) {
	std::cout << "INET_ADDRSTRLEN = " << INET_ADDRSTRLEN << std::endl;
	struct in_addr addr;
	inet_pton(AF_INET, "192.0.2.33", &(addr));
	std::cout << "addr is " << addr.s_addr << std::endl;
	return 0;
}
```

注意，`struct in_addr addr`是按网络字节序存储数据，地址`192.0.2.33`转换后的结果为：`33*256*256*256 + 2*256*256 + 192 = 553779392`。

IPv4地址的数值类型为`uint32_t`，IPv6地址的类型为`unsigned char[16]`。

## 过时的转换方式

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char *inet_ntoa(struct in_addr in);
int inet_aton(const char *cp, struct in_addr *inp);
in_addr_t inet_addr(const char *cp);
```

## 参考资料

http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
http://beej.us/guide/bgnet/output/html/multipage/inet_ntoaman.html
http://stackoverflow.com/questions/5328070/how-to-convert-string-to-ip-address-and-vice-versa
man 7 ip
man 7 ipv6
man inet_ntop
man inet_ntoa

