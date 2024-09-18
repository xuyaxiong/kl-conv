# Dll 链接库函数声明转换为 JS 函数声明

## 1. 打包

```bash
# 1. 创建虚拟环境
python -m venv venv
# 2. 激活虚拟环境
venv\Scripts\activate
# 3. 安装依赖
pip install -r requirements.txt
# 4. 执行打包指令
python setup.py
```

## 2. 测试

```bash
pytest
```

## 3. 示例

- 3.1 输入

```C++
int updateTransformParams(double* calib, double* rectify, double* update);

/*
@brief //图像坐标转换至机械坐标
@param[in] n 需转换坐标数量
@param[in] imgCoor 图像坐标(double[2 * n], x/y坐标)
@param[in] params 映射矩阵(double[11], 图像中心x/y + 投影矩阵9);
@param[in] motorPos 采图时的x/y轴位置读数(double[2])
@param[out] worldCoor 机械坐标(double[2 * n], x/y坐标)
*/
PS_API int transformBiaxial(int n, double* imgCoor, double* params, double* motorPos, double* worldCoor);

/*
@brief //机械坐标反变换至图像坐标
@param[in] n 需转换坐标数量
@param[in] worldCoor 机械坐标(double[2 * n], x/y坐标)
@param[in] params 映射矩阵(double[11], 图像中心x/y + 投影矩阵9, 正变换矩阵)
@param[in] motorPos 采图时的x/y轴位置读数(double[2])
@param[out] imgCoor 图像坐标(double[2 * n], x/y坐标)
*/
int transformInvBiaxial(int n, double* worldCoor, double* params, double* motorPos, double* imgCoor);


PS_API int transformInvBiaxialCenter(double* worldCoor, double* params, double* motorPos);

/*
@brief //机械坐标转换至行列坐标
@param[in] n 需转换坐标数量
@param[in] worldCoor 机械坐标(double[2 * n], x/y坐标)
@param[in] params 投影矩阵(double[9])
@param[out] mapCoor 行列坐标(double[2 * n], 列/行坐标)
*/
int transformMapping(int n, double* worldCoor, double* params, double* mapCoor);

/*
@brief //行列坐标反变换至机械坐标
@param[in] n 需转换坐标数量
@param[in] mapCoor 行列坐标(double[2 * n], 列/行坐标)
@param[in] params 投影矩阵(double[9], 正变换矩阵)
@param[out] worldCoor 机械坐标(double[2 * n], x/y坐标)
*/
int transformInvMapping(int n, double* mapCoor, double* params, double* worldCoor);
```

- 3.2 输出

```javascript
updateTransformParams: ['int', ['double*', 'double*', 'double*']],

/*
@brief //图像坐标转换至机械坐标
@param[in] n 需转换坐标数量
@param[in] imgCoor 图像坐标(double[2 * n], x/y坐标)
@param[in] params 映射矩阵(double[11], 图像中心x/y + 投影矩阵9);
@param[in] motorPos 采图时的x/y轴位置读数(double[2])
@param[out] worldCoor 机械坐标(double[2 * n], x/y坐标)
*/
transformBiaxial: ['int', ['int', 'double*', 'double*', 'double*', 'double*']],

/*
@brief //机械坐标反变换至图像坐标
@param[in] n 需转换坐标数量
@param[in] worldCoor 机械坐标(double[2 * n], x/y坐标)
@param[in] params 映射矩阵(double[11], 图像中心x/y + 投影矩阵9, 正变换矩阵)
@param[in] motorPos 采图时的x/y轴位置读数(double[2])
@param[out] imgCoor 图像坐标(double[2 * n], x/y坐标)
*/
transformInvBiaxial: ['int', ['int', 'double*', 'double*', 'double*', 'double*']],


transformInvBiaxialCenter: ['int', ['double*', 'double*', 'double*']],

/*
@brief //机械坐标转换至行列坐标
@param[in] n 需转换坐标数量
@param[in] worldCoor 机械坐标(double[2 * n], x/y坐标)
@param[in] params 投影矩阵(double[9])
@param[out] mapCoor 行列坐标(double[2 * n], 列/行坐标)
*/
transformMapping: ['int', ['int', 'double*', 'double*', 'double*']],

/*
@brief //行列坐标反变换至机械坐标
@param[in] n 需转换坐标数量
@param[in] mapCoor 行列坐标(double[2 * n], 列/行坐标)
@param[in] params 投影矩阵(double[9], 正变换矩阵)
@param[out] worldCoor 机械坐标(double[2 * n], x/y坐标)
*/
transformInvMapping: ['int', ['int', 'double*', 'double*', 'double*']],
```
