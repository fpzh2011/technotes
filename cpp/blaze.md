# blaze笔记

https://bitbucket.org/blaze-lib/blaze/wiki/Home

## 依赖

blaze可以不依赖BLAS库，但性能要差一些。所以，最好安装BLAS库。
如果需要行列式、矩阵分解、逆矩阵、特征值、奇异值等计算，需要安装LAPACK库，否则会报连接错误。

## 安装

`cp -r ./blaze /usr/local/include`

## 编译

至少使用`c++14`，否则会报错。
`g++ -std=c++14 test.cc`

## eval/evaluate

`evaluate`适用于`auto`自动推断类型，根据rvo会减少临时对象和拷贝操作。如：`auto c = evaluate( a * b );`
`eval`适用于含不同向量类型的、复杂表达式的中间步骤，比如`d = a + eval( b * c );`
https://bitbucket.org/blaze-lib/blaze/wiki/Vector%20Operations
https://bitbucket.org/blaze-lib/blaze/wiki/Intra-Statement%20Optimization

## column-major/row-major

矩阵与向量相乘，column-major的矩阵会进行vectorized computation。
https://bitbucket.org/blaze-lib/blaze/wiki/Adaptors


