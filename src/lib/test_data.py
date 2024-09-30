docstrings = """
//**********************************1. 粗定位**********************************//
int updateTransformParams(double* calib, double* rectify, double* update);

/*=================================step1:初始化图像检测类对象, 并重新计算背景平均灰度值======================================*/
/*
@brief //图像坐标转换至机械坐标
@param[in] n 需转换坐标数量
@param[in] imgCoor 图像坐标(double[2 * n], x/y坐标)
@param[in] params 映射矩阵(double[11], 
    图像中心x/y + 投影矩阵9);
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

DLL_API int getFullTransform(double* calibParams, double* rectifyParams, double* motorPos, double* mapParams, double* transParams = NULL, double* invParams = NULL);

/*
@brief 获取标定参数，根据标定图像坐标偏移量、机械坐标偏移量，计算变换矩阵
@param[in] n 输入imgCoors、motorBiases对的数量，需要n>=6(数量稍多较好)，其中至少2张是仅有旋转的，至少4张是带x/y偏移的
@param[in] tplCoor 模板基准点坐标及旋转角度(double[3]，第0/1位：x/y，第2位：theta，来自Registration接口)
@param[in] imgCoors 标定图像基准点坐标及旋转角度(double[n*3]，第0/1位：x/y，第2位：theta，来自Registration接口)
@param[in] motorBiases 各标定位到基准点的机械偏移量(double[n*3]：x/y/theta，x/y为毫米单位，theta为角度单位)
@    hint 即标定图电机读数减模板图电机读数的差
@param[out] params 从图像变换到机械变换之间的变换参数(double[13]，第0~2位：tplCoor，第3/4位：旋转中心x/y，第5~12位：x/y平移变换的8参数投影矩阵)
@return 0：成功，其他：失败
*/
DLL_API	int estimateTriaxialTransform(int n, double* tplCoor, double *imgCoors, double* motorBiases, double* params);
"""

docstring = """
/*
@brief 获取标定参数，根据标定图像坐标偏移量、机械坐标偏移量，计算变换矩阵
@param[in] n 输入imgCoors、motorBiases对的数量，需要n>=6(数量稍多较好)，其中至少2张是仅有旋转的，至少4张是带x/y偏移的
@param[in] tplCoor 模板基准点坐标及旋转角度(double[3]，第0/1位：x/y，第2位：theta，来自Registration接口)
@param[in] imgCoors 标定图像基准点坐标及旋转角度(double[n*3]，第0/1位：x/y，第2位：theta，来自Registration接口)
@param[in] motorBiases 各标定位到基准点的机械偏移量(double[n*3]：x/y/theta，x/y为毫米单位，theta为角度单位)
@    hint 即标定图电机读数减模板图电机读数的差
@param[out] params 从图像变换到机械变换之间的变换参数(double[13]，第0~2位：tplCoor，第3/4位：旋转中心x/y，第5~12位：x/y平移变换的8参数投影矩阵)
@return 0：成功，其他：失败
*/
DLL_API	int estimateTriaxialTransform(int n, double* tplCoor, double *imgCoors, double* motorBiases, double* params);
"""

comm = """
/*
    @brief
    @param[in] markId 两个点的顺序，0-左上，1-右上，2-右下，3-左下(int[2])
    @param[in] markCoor 检测区域定位mark的机械坐标(double[4]：x/y, 
            左上[0/1], 右上[2/3])
        @param[in] calibParams 像素标定参数
        @param[in] rotateCenter 载台旋转中心(double[2]:x/y)
        @param[in] refCoor 定位建模时两个定位模版的世界坐标(double[4], 手动时为null)
        @param[in/out] rectifyParams 坐标变换矩阵(double[9]，手动时为新坐标double[4])
        @param[out] angle 载台旋转角度(double[1])
*/
"""

decl = """
PS_API  int  transformInvMapping(int n, double* mapCoor, double* params, double* worldCoor);
"""

formatted_comm = """/*
@brief
@param[in] markId 两个点的顺序，0-左上，1-右上，2-右下，3-左下(int[2])
@param[in] markCoor 检测区域定位mark的机械坐标(double[4]：x/y, 
    左上[0/1], 右上[2/3])
@param[in] calibParams 像素标定参数
@param[in] rotateCenter 载台旋转中心(double[2]:x/y)
@param[in] refCoor 定位建模时两个定位模版的世界坐标(double[4], 手动时为null)
@param[in/out] rectifyParams 坐标变换矩阵(double[9]，手动时为新坐标double[4])
@param[out] angle 载台旋转角度(double[1])
*/"""

parsed_decl = (
    """transformInvBiaxialCenter: ['int', ['double*', 'double*', 'double*']]"""
)

formatted_docstring = """
/*
@brief //机械坐标转换至行列坐标
@param[in] n 需转换坐标数量
@param[in] worldCoor 机械坐标(double[2 * n], x/y坐标)
@param[in] params 投影矩阵(double[9])
@param[out] mapCoor 行列坐标(double[2 * n], 列/行坐标)
*/
transformMapping: ['int', ['int', 'double*', 'double*', 'double*']],
"""
