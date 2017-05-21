# python数据处理

## 加载数据

numpy
```python
from numpy import genfromtxt
my_data = genfromtxt('sample.txt', delimiter='\t')
```




pandas
```python
import os
import pandas as pd

occupancy = os.path.join('data','occupancy_data','datatraining.txt')
# read_csv执行完后，occupancy是DataFrame类型
occupancy = pd.read_csv(occupancy, sep=',', header=None)
key = open("key", "r").read().split('\n')
occupancy.columns = ['date', 'temp', 'humid', 'light', 'co2', 'hratio', 'occupied']
# occupancy.values 就是ndarray
# View the occupancy details
print(occupancy.head())
print(occupancy.describe())
# Load the credit card default dataset
credit = os.path.join('data','credit.xls')
credit = pd.read_excel(credit, header=1)
credit.columns = ['id', 'limit', 'sex', 'edu', 'married', 'age', 'apr_delay', 'may_delay']
```





