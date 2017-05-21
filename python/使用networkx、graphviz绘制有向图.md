# 使用networkx、graphviz绘制有向图

绘制分层的有向图，用graphviz的dot布局效果不错，需要dot格式的文件作为输入。networkx可以很方便的构建图对象，并直接输出dot格式文件。

NetworkX是一个用Python语言开发的图论与复杂网络建模工具，内置了常用的图与复杂网络分析算法，可以方便的进行复杂网络数据分析、仿真建模等工作。networkx可以利用matplotlib导出图片，但是不如用graphviz命令行的布局效果好。

https://en.wikipedia.org/wiki/Graph_drawing

## 中文问题

https://www.zhihu.com/question/25404709

环境：Windows10/CentOS7.3/Python2.7.5

安装必要的包：
```shell
sudo yum install python-devel python2-pip tkinter graphviz graphviz-devel
sudo pip install networkx matplotlib pygraphviz
#graphviz需要中文字体
sudo yum groupinstall "fonts"
```

拷贝微软雅黑字体文件`C:\Windows\Fonts\Microsoft YaHei UI\msyh.ttc`到`/usr/lib64/python2.7/site-packages/matplotlib/mpl-data/fonts/ttf/`。
操作系统的字体文件夹是`/usr/share/fonts/`。目前matplotlib没有使用这个文件夹，而是使用自己的字体文件夹。

备份配置文件matplotlibrc：
`sudo cp /usr/lib64/python2.7/site-packages/matplotlib/mpl-data/matplotlibrc /usr/lib64/python2.7/site-packages/matplotlib/mpl-data/matplotlibrc.bak`

修改matplotlib配置文件：`/usr/lib64/python2.7/site-packages/matplotlib/mpl-data/matplotlibrc`：
```
删除font.family和font.sans-serif两行前的#
在font.sans-serif后添加中文字体 Microsoft YaHei
```

备份font_manager.py：
`sudo cp /usr/lib64/python2.7/site-packages/matplotlib/font_manager.py /usr/lib64/python2.7/site-packages/matplotlib/font_manager.py.bak`
修改font_manager.py：`sudo vi /usr/lib64/python2.7/site-packages/matplotlib/font_manager.py`，将`return {'ttf': ('ttf', 'otf')`改为`return {'ttf': ('ttf', 'otf', 'ttc')`以增加对字体扩展名`ttc`的支持。

删除cache文件`rm ~/.cache/matplotlib/fontList.py3k.cache ~/.cache/matplotlib/fontList.cache`

## graphviz

备份nx_pylab.py：`cp  /usr/lib/python2.7/site-packages/networkx/drawing/nx_pylab.py /usr/lib/python2.7/site-packages/networkx/drawing/nx_pylab.py.bak`
增加`from networkx.drawing.nx_agraph import graphviz_layout`。
修改nx_pylab.py，将`nx.drawing.graphviz_layout(G, prog)`改为`graphviz_layout(G, prog)`。

## 在没有X Windows的情况下运行

http://stackoverflow.com/questions/2801882/generating-a-png-with-matplotlib-when-display-is-undefined

```python
# -*- coding:utf-8 -*-
import networkx as nx
import matplotlib
# use函数必须在导入pyplot前调用 
matplotlib.use('Agg')
import matplotlib.pyplot as plt
g = nx.Graph()
g.add_edge('挪威'.decode('utf8'), '瑞典'.decode('utf8'))
nx.draw(g, node_size=1000, node_color='white',with_labels=True, font_family='Microsoft YaHei')
plt.savefig('plot.png')
```

