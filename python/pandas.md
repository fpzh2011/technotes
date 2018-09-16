# pandas

## DataFrame

### 属性

http://pandas.pydata.org/pandas-docs/stable/api.html#dataframe

#### DataFrame.index

`RangeIndex`类型。如果row有label，DataFrame.index也可能是`Index`类型？
`DataFrame.index.values`是一个`numpy.ndarray`，即row labels。

#### DataFrame.columns

`Index`类型。
`DataFrame.columns.values`是一个`numpy.ndarray`，即column labels。

### 操作

#### []/items/iteritems/keys

`[]`操作返回DataFrame的一列(Series对象)，比如`data['ID']`返回ID列。
`data[['ID', 'score']]`返回一个只包含两列的、新的DataFrame。

类似的，`items/iteritems`都是按列遍历数据。
`keys`返回列名集合(Index对象)。

#### DataFrame.at

http://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.at.html#pandas.DataFrame.at
可以引用某个元素，也可以对某个元素赋值：
```
df.at[4, 'B']
df.at[4, 'B'] = 10
```

#### DataFrame.loc

http://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.loc.html#pandas.DataFrame.loc
`loc`可以筛选行/列，也可以同时筛选行、列，还可以改变df元素的值。
```python
# 选择单行. viper是行标记
df.loc['viper']
# 选择多行。注意是[]套[]。如果行标签不存在，返回的相应行都是NaN。
df.loc[['viper', 'sidewinder']]
# 选择行、列。row label和column label用逗号分隔
df.loc['cobra', 'shield']
```

`iloc`与`loc`类似，只是按整数对行、列进行索引。例如：
```python
# 2-6行，0-2列
data.iloc[2:7, 0:3]
```
http://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.iloc.html#pandas.DataFrame.iloc

#### DataFrame.sample

http://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.sample.html#pandas.DataFrame.sample
沿row或column方向采样。默认是对row采样。
主要参数：
* n: 采样条数
* frac: 采样比例。frac与n不能同时提供。
* axis: 采样轴。默认是对row采样。






## demo

import pandas as pd

data = pd.read_csv('~/temp/adaptive/data416.csv')
data.columns



