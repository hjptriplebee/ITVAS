# IVAN
智能视频分析网络--Intelligent video analysis network

南京大学[R&L研究组](https://cs.nju.edu.cn/rl/)所有

## 依赖

- GCC 4.8.5 
- Python 3.6
- Opencv 3.2 (build with opencv_contrib-3.2.0)
- Qt 5.9.1
- Tensorflow 1.4.1

## 开发说明

### 开发人员名单
| 姓名 | github  | 详情情况 |
| ------------ | ------------ | ------------ |
| xxx | xxx | xxx |

### 命名规则

- 变量命名。第一个单词首字母小写，其余大写。例如：
```cpp
int maxDepth,int depth
```

- 函数命名。第一个单词首字母小写，其余大写。例如：
```cpp
getTrainSamples(…)
```

- 固定变量、宏定义。所有字母大写，字母间用‘ _’隔开。例如:
```cpp
const int MAX_TREE_DEPTH = 32;
#define HAVE_OPENCV_CALIB3D
```

- 结构体、类。每个单词首字母大写。例如：
``` cpp
struct SubTree
{
    int first;
    int last;
    int nodeIdx;
    int depth;
};
class TrainData
{
    ......
}
```

### 注释说明

- 通常头文件要对功能和用法作简单说明， 源文件包含了更多的实现细节或算法讨论。

- 文件头部应进行注释，注释必须列出： 文件名、生成日期、 作者姓名、功能说明等。
```cpp
/*************************************************
    Copyright R&L group of Nanjing University. 2017. All rights reserved.
    文件名: hello.cpp
    作者: 张三
    完成日期: 2017-9-20
    描述: 用于详细说明此程序文件完成的主要功能，与其他模块或函数的接口，输出值、取值范围、含义及参数间的控制、顺序、独立或依赖等关系
*************************************************/
```

- 函数头注释（ 注意：函数的实现在cpp文件中）
```cpp
/*****************************************
*  @brief calculate score according to bBox and type 说明
*  @param A: boundingbox 输入参数和对应含义
*  @param B: type 
*  @return score 输出参数和对应含义
*****************************************/
double fun(A, B)
```

### 代码模块化

- 一个模块，需要一个独立的工程。

- 每个工程只有一个对外开放的.h文件和dll（编译好的动态链接库）文件。

- 每个模块（工程）之间，要尽可能的减少依赖，是的各个模块之间尽可能的独立。

- 工程名和文件名的命名规范，需要谨慎，请参照opencv工程。 注：对外开放的.h文件的名称要与工程名称一致。

### 一些细节

- 符号两边要有空格，括号两边不要空格（ opencv里加的，这边我把它去掉了）。例如：
```cpp
if(vec.cols == 1 || vec.rows == 1)
{
    dims = 1;
    m = vec.cols + vec.rows - 1;
}
```

- 函数定义::前后没有空格，函数中逗号后面要加一个空格。例如：
```cpp
Mat TrainData::getSubVector(const Mat& vec, const Mat& idx)
```

- 类的继承 :前后有空格.例如：
```cpp
class TrainDataImpl : public TrainData{}
```
