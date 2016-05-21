#人工智能导论第5次作业报告
计45 孟垂正 2013010952

## 要求
使用朴素贝叶斯和支持向量机方法进行文本分类.

## 编程语言与环境
采用Python实现, 支持向量机使用的是[libSVM]提供的工具包. 运行环境为Python2.7.11, Arch Linux.

## 朴素贝叶斯方法
### 原理
对于每个分类$c_i$, 在独立性假设下计算出现串$x_1x_2...x_n$的概率:
$$P(X=x_1x_2...x_n)=P(C=c_i)P(X_1=x_1|C=c_i)...P(X_n=x_n|C=c_i)$$
其中$P(C=c_i)=\dfrac{N_{c_i类文档数}}{N_{总文档数}}$

[libSVM]: http://www.csie.ntu.edu.tw/~cjlin/libsvm/ "libSVM"
