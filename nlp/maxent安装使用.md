# maxent安装使用

## 安装

张乐博士的[maxent](http://homepages.inf.ed.ac.uk/lzhang10/maxent_toolkit.html)是较常使用的最大熵实现，以下是编译安装步骤。

系统环境：
* CentOS Linux release 7.1.1503 (Core) x86_64
* gcc/g++/gfortran 4.8.3

```shell
sudo yum install boost-devel zlib-devel python-devel
git clone https://github.com/lzhang10/maxent.git
cd maxent
# Building C++ Library
./configure
make
sudo make install
sudo chmod o+r /usr/local/bin/maxent
# Building Python Extension
cd python
python setup.py build
sudo python setup.py install
# 测试python
cd ../test/
python test_pyext.py
```


http://www.cs.columbia.edu/~mcollins/crf.pdf
http://homepages.inf.ed.ac.uk/csutton/publications/crftut-fnt.pdf
https://www.zhihu.com/question/35866596
https://www.google.com/search?safe=strict&q=%E5%90%89%E5%B8%83%E6%96%AF%E9%87%87%E6%A0%B7&oq=%E5%90%89%E5%B8%83%E6%96%AF%E9%87%87%E6%A0%B7&gs_l=serp.3...12323535.12323535.0.12324343.1.1.0.0.0.0.340.340.3-1.1.0....0...1.1.64.serp..0.0.0.igfGErnapFI
https://www.zhihu.com/question/23326710




看看CRF的资料
POS
命名实体识别

python
C++命令行
试验最大熵分词
再完整看一遍最大熵资料，整理文档，梳理最大熵的优缺点
再看一遍手册，以及berger的论文

params的第一个字段，是该行outcome的个数。





