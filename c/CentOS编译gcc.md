# 编译GCC

## 环境

操作系统：CentOS 6.8, x86-64

gcc版本：4.9.3

## 步骤

```shell
sudo yum install texinfo-tex flex zip libgcc glibc-devel bison 

# 安装必要的依赖包
./contrib/download_prerequisites

mkdir build

cd build

../configure --prefix=/opt/gcc_4_9 --program-suffix=-4.9 --enable-languages=c,c++ --disable-multilib --build=x86_64-linux-gnu --enable-checking=release

# 4是CPU核数
make -j4

make install

/opt/gcc_4_9/bin/g++-4.9 --version
```

## 修改链接

编译后的可执行程序中，c++-4.9 、g++-4.9、x86_64-linux-gnu-c++-4.9、x86_64-linux-gnu-g++-4.9的内容是一样的。其它程序也类似。

为便于使用，可以进行如下更名和链接修改：
```shell
sudo mv /usr/bin/gcc /usr/bin/gcc-4.4.7
sudo mv /usr/bin/g++ /usr/bin/g++-4.4.7
sudo mv /usr/bin/cpp /usr/bin/cpp-4.4.7
sudo ln -s /opt/gcc_4_9/bin/gcc-4.9 /usr/bin/gcc
sudo ln -s /opt/gcc_4_9/bin/g++-4.9 /usr/bin/g++
sudo ln -s /opt/gcc_4_9/bin/cpp-4.9 /usr/bin/cpp
# 解决g3log构建问题
sudo mv /usr/bin/c++ /usr/bin/c++-4.4.7
sudo ln -s /opt/gcc_4_9/bin/c++-4.9 /usr/bin/c++
sudo mv /usr/libexec/gcc/x86_64-redhat-linux/4.4.4/cc1plus /usr/libexec/gcc/x86_64-redhat-linux/4.4.4/cc1plus-4.4.4
sudo ln -s /opt/gcc_4_9/libexec/gcc/x86_64-linux-gnu/4.9.3/cc1plus /usr/libexec/gcc/x86_64-redhat-linux/4.4.4/cc1plus
sudo mv /usr/libexec/gcc/x86_64-redhat-linux/4.4.4/cc1 /usr/libexec/gcc/x86_64-redhat-linux/4.4.4/cc1-4.4.7
sudo ln -s /opt/gcc_4_9/libexec/gcc/x86_64-linux-gnu/4.9.3/cc1 /usr/libexec/gcc/x86_64-redhat-linux/4.4.4/cc1
sudo mv /usr/libexec/gcc/x86_64-redhat-linux/4.4.4/collect2 /usr/libexec/gcc/x86_64-redhat-linux/4.4.4/collect2-4.4.7
sudo ln -s /opt/gcc_4_9/libexec/gcc/x86_64-linux-gnu/4.9.3/collect2 /usr/libexec/gcc/x86_64-redhat-linux/4.4.4/collect2
```

## 编译、连接与运行

连接时可以设置`export LD_RUN_PATH=/opt/gcc_4_9/lib64`。运行时可以设置`export LD_LIBRARY_PATH=/opt/gcc_4_9/lib64`。

## 参考资料

http://zisxks.com/2014/04/11/compile-and-install-GCC-4.9-in-Ubuntu-12.04/

