# Fruit Project的整体设计
fruit整体会被设计成一个基于python的数据分析框架。
考虑到对于不同的工作我们需要使用合适的语言，这个项目会对C++也进行支持。
项目希望能够最大化地做到代码的重用。并且使用最简洁的语法。

fruit主要包括两部分：
+ 用来管理项目（正在做的分析）的可执行程序
+ 一系列python函数库

## 管理工具
用一个fruit命令来完成代码的构建管理和运行。主要功能包括以下：
### 库文件下载

    fruit get somehead.py

下载完成后，在程序中就可以直接使用

    import somehead

### 程序生成
例如

    fruit generate selection

生成事例选择的框架

### 程序运行

    fruit somecode.py

可以直接对这个程序进行执行
## Python函数库
目前准备制作下列函数库：
+ 拟合
+ 绘图
+ 数据管理

