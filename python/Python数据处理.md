# Python数据处理

## json数据加载

```python
import json
path='usagov_bitly_data2012-03-16-1331923249.txt'
# 用列表解析表达式创建列表。[2]，P101
records = [json.loads(line) for line in open(path)]
# 下面是延迟求值（懒加载），record是一个generator object（不是元组）。
# 参考：Python学习手册，P506，生成器表达式
record = (json.loads(line) for line in open(path))
```

## csv数据加载

```python
import pandas as pd
# 第一行是列名
my_data = pd.read_csv("train.csv", header=0)
```

## pandas数据处理

### 选择列，选择行

通过索引方式返回的行、列都是数据的视图，而不是拷贝。对返回的Series进行修改会反映到源DataFrame上。
```python
# 利用Python进行数据分析，P120
import pandas as pd
# 利用字典构建DataFrame，key作为columns
data = {'state': ['a', 'a', 'a', 'b', 'b'], 'year': [2000, 2001, 2002, 2001, 2002], 'pop': [1.5, 1.7, 3.6, 2.4, 2.9]}
frame = pd.DataFrame(data)
# []运算符选择的是列。多个列可用 frame[['state', 'year']]
frame['state']
# 选择行用ix属性（P132）。选择第0、2行： frame.ix[[0, 2]]
# 如果用[]选择行，会报错。如frame[0]
frame.ix[0]
# 利用布尔表达式选择行。利用Python进行数据分析，P131
frame[frame['pop'] > 2]
# 转置
frame.T
```

### 添加、删除列

```python
# 可以将列表、数组、Series等直接赋值给frame的某个列。如果列不存在，就新建该列。利用Python进行数据分析，P121
frame['debt'] = frame['pop'] / 2
# 删除列，返回删除后的拷贝
frame = frame.drop('debt', axis=1)
```

### 修改数据

可以对整行、整列进行数据修改（利用Python进行数据分析，P121）。但是`frame.ix[0]['state']`返回的是标量值，无法修改frame中的单个元素。
修改单个元素：

```python
# 参数依次是：行index，列，要赋的值。 http://stackoverflow.com/questions/13842088/
frame.set_value(0, 'pop', 99)
# apply函数可以对Series或DataFrame中的元素运行函数进行修改，也可以用map函数。P137/138
frame['pop'] = frame['pop'].apply(lambda x: 1/x)
```

### 按列join多个DataFrame

merge 函数，P187-。可以设置外连接。
也可以用 join 方法（按index join，P193）。

### pandas简单统计

```python
# P142-。median, max, min, var, std, mode, quantile, corr, cov, unique, describe
frame.count()
frame.sum()
frame.mean()
```

### join操作

merge函数可以进行类似关系数据库的join操作。P187

### concat操作

concat函数可以将两个数据集合并为一个数据集，可以在行的方向合并，或者在列的方向合并。P194





选择列，根据现有列生成新列, sum(b)/count(b) group by a, 连续数据离散化、面元划分，交叉表，join，

条形图、饼图、折线图、概率密度图。。。。

panel

seaborn countbar 
http://www.cnblogs.com/kylinlin/p/5236601.html



## 缺失值处理




## 数据可视化

pandas, seaborn, matplotlib


## 参考资料

1. 利用Python进行数据分析
2. Python学习手册




