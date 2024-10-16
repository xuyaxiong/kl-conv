#pragma once

#if defined(DLL_EXPORTS)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	//打印库版本信息
	DLL_API char* get_dll_version();

	/*-----------------------数据相关-----------------------*/
	/*
	@brief 加载db如内存
	@param name db名
	@return db中特征条数
	*/
	DLL_API int load_DB(const char* name);

	/*
	@brief 删除释放内存中的db
	@param name db名
	*/
	DLL_API void release_DB(const char* name);

	/*
	@brief 加载db如内存，并返回内存中db实例的指针
	@param name db名
	@return 指向内存db实例的空指针
	*/
	void* DB_instance(const char* name);

	/*
	@brief 向db插入特征
	@param name db 名
	@param n 插入的特征数量
	@param flaw_ids 插入的特征对应的缺陷id
	@param flaw_types 插入的特征对应的缺陷类型
	@param features 插入的特征序列
	*/
	DLL_API int insert(const char* name, int n, int* flaw_ids, int* flaw_types, float* features);

	/*
	@brief 向db插入一条特征
	@param name db 名
	@param flaw_id 插入的特征对应的缺陷id
	@param flaw_type 插入的特征对应的缺陷类型
	@param feature 插入的特征
	*/
	DLL_API int insert_one(const char* name, int flaw_id, int flaw_type, float* feature);

	/*
	@brief 删除db中的特征
	@param name db名
	@param n 删除的特征数量
	@param flaw_ids 删除的特征所对应的缺陷id集合
	*/
	DLL_API int erase(const char* name, int n, int* flaw_ids);

	/*
	@brief 按缺陷id，删除db中的一条特征
	@param name db名
	@param flaw_ids 删除的特征所对应的缺陷id
	*/
	DLL_API int erase_one(const char* name, int flaw_id);

	/*
	@brief 按缺陷类型，删除db中的一部分特征
	@param name db名
	@param flaw_ids 删除的特征所对应的缺陷类型
	*/
	DLL_API int erase_type(const char* name, int flaw_type);

	/*
	@brief 更新db中的特征
	@param name db名
	@param flaw_ids 需被更新的特征对应缺陷id集合(按id更新)
	@param flaw_types 需被更新的特征对应缺陷类型集合。缺陷类型填写负数表示不更新，指针可为空
	@param features 需被更新的特征序列
	*/
	DLL_API bool update(const char* name, int n, int* flaw_ids, int* flaw_types = 0, float* features = 0);

	/*
	@brief 更新db中的一条特征
	@param name db名
	@param flaw_id 需被更新的特征对应缺陷id(按id更新)
	@param flaw_type 需被更新的特征对应缺陷类型。缺陷类型填写负数表示不更新，指针可为空
	@param feature 需被更新的特征
	*/
	DLL_API bool update_one(const char* name, int flaw_id, int flaw_type, float* feature);

	// 查询数据
	/*
	@brief 查询特征在db中的最近邻特征dists，types，ids
	@param name db名
	@param n 输入查询的特征数目
	@param features 输入查询的特征序列
	@param dists 查到的特征序列对应的最大余弦距离
	@param types 查到的特征序列对应的缺陷类型
	@param ids 查到的特征序列对应的缺陷id
	*/
	DLL_API bool query(const char* name, int n, const float* features, float* dists = 0, int* types = 0, int* ids = 0);

	/*-----------------------引擎相关-----------------------*/
	/*
	@breif 初始化引擎
	@param dir 放onnx模型或engine的文件夹
	@param setN 批量数
	@param setC 推理图片的通道数
	@param setH 推理图片的高
	@param setW 推理图片的宽
	@param setD 特征维度
	@return 初始化是否成功
	*/
	DLL_API bool initEngine(const char* dir, int setN, int setC, int setH, int setW, int setD = 384);

	/*
	@brief 获取特征维度
	@return 特征维度
	*/
	DLL_API int getFeatureDim();

	/*
	@brief 设置回调
	@param cb 回调函数指针
	@param[out] updateMap 单点位图中缺陷(或mark)晶粒行/列坐标 int[](row/col)(数组空间按晶粒总数开辟)
	@	二维表格：已检晶粒数量(n) + 所有晶粒（行号/列号/类型）
	@	类型：已检ok: 0, mark:-1, 未知缺陷(distMap<):-2, ng: typeMap(id)[typeMap(id)没有查到时,查normal_id]
	@return 设置是否成功
	*/
	DLL_API bool setCallback(void(*cb)(const char*, bool, int, float*, int *updateMap));



	/*
	@brief 检测缺陷（带掩膜）
	@param[in] imgName 图片名
	@param[in] engineName 引擎名
	@param[in] src_ptr 图片数据首地址
	@param[in] height 高
	@param[in] width 宽
	@param[in] channel 通道
	@param[in] max_num 最大缺陷数
	@param[in] threshold 距离阈值
	@param[in] set_norm_ids 跳过检测的缺陷id数组(第1个为“正常”类型的id)
	@param[in] set_n 跳过检测的id数组大小
	@param[in] is_first 是否是第一帧图(1:true, 0:false)
	@param[in] shield_bg 是否屏蔽背景(1/0)
	@param[in] shield_block_num 屏蔽检测的block个数
	@param[in] shield_block 屏蔽检测的block
	（数组，长度为4*shield_block_num；每一组4个数据为：block起始die行 | 起始die列 | 结束die行 | 结束die列 ）
	@param[in] shield_die_num 屏蔽检测的die个数
	@param[in] shield_die 屏蔽检测的die
	（数组，长度为2*shield_die_num；每一组2个数据为：die行 | die列 ）
	@param[in] result 检测结果(x0,y0,w0,h0,feanture,x_lu_feat,y_lu_feat,col_lt, row_lt, col_rb, row_rb,type,score)
	@param[in] motorPos 拍摄点的机械坐标(double[2]: x/y)
	@param[in] calibParams 像素标定参数(double[13])
	@param[in] rectifyParams 坐标变换矩阵(double[9])
	@param[in] mapParams 机械坐标与行列坐标的变换矩阵(double[9])
	@param[in] roiCornerPoint
	@param[in] chipNum
	@param[in] chip_cfg chip的ROI在die中的比例(double[4*chipNum+3]: chip左上比例(0.1, 0.1), 右下比例(0.9, 0.9)) * [LX,LY,RX,RY, 是否屏蔽外观检测(1/0)，是否屏蔽测量偏移(1/0)， 是否屏蔽测量旋转(1/0)]
	@param[in] src_file
	@param[in] mask_file
	@return 运行成功
	*/
	DLL_API bool anomalyDetect_FULL(const char* imgName, const char* engineName,
		unsigned char* src_ptr, int height, int width, int channel,
		int max_num, float threshold, int *set_norm_ids, int set_n,
		int is_first, int shield_bg, int shield_block_num, int *shield_block, int shield_die_num, int *shield_die, float *result,
		double *motorPos, double* calibParams, double* rectifyParams, double* mapParams,
		double* roiCornerPoint, int chipNum, double* chip_cfg, const char* src_file, const char* mask_file);

	DLL_API bool anomalyDetect(const char* imgName, const char* engineName,
		unsigned char* src_ptr, unsigned char* mask_ptr, int height, int width, int channel,
		int max_num, float threshold, int *set_norm_ids, int set_n, float *result,
		double *motorPos, double* calibParams, double* rectifyParams, double* mapParams);

	//DLL_API bool anomalyDetect_FULL(const char* imgName, const char* engineName,
	//	unsigned char* src_ptr, int height, int width, int channel,
	//	int max_num, float threshold, int *set_norm_ids, int set_n, float *result,
	//	double *motorPos, double* calibParams, double* rectifyParams, double* mapParams,
	//	double* roiCornerPoint, int chipNum, double* chipSize, const char* src_file, const char* mask_file);

/*
@brief 检测缺陷（带掩膜）; ZXK CHANGE 输出改为 col , row , chipid, 其中 col和row是中心点所在的die
@param[in] imgName 图片名
@param[in] engineName 引擎名
@param[in] src_ptr 图片数据首地址
@param[in] height 高
@param[in] width 宽
@param[in] channel 通道
@param[in] max_num 最大缺陷数
@param[in] threshold 距离阈值
@param[in] set_norm_ids 跳过检测的缺陷id数组(第1个为“正常”类型的id)
@param[in] set_n 跳过检测的id数组大小
@param[in] is_first 是否是第一帧图(1:true, 0:false)
@param[in] shield_bg 是否屏蔽背景(1/0)
@param[in] dieNum die个数，调用readDetectRegionInfo获得
@param[in] dieInfo die屏蔽序列，调用readDetectRegionInfo获得
@param[in] result 检测结果(x0,y0,w0,h0,feanture,x_lu_feat,y_lu_feat, col , row , chipid ,type , score)
@param[in] motorPos 拍摄点的机械坐标(double[2]: x/y)
@param[in] calibParams 像素标定参数(double[13])
@param[in] rectifyParams 坐标变换矩阵(double[9])
@param[in] mapParams 机械坐标与行列坐标的变换矩阵(double[9])
@param[in] roiCornerPoint
@param[in] chipNum
@param[in] chip_cfg chip的ROI在die中的比例(double[7*chipNum]: chip左上比例(0.1, 0.1), 右下比例(0.9, 0.9)) * [LX,LY,RX,RY, 是否屏蔽外观检测(1/0)，是否屏蔽测量偏移(1/0)， 是否屏蔽测量旋转(1/0)]
@param[in] src_file
@param[in] mask_file
@return 运行成功
*/
	DLL_API bool anomalyDetect_FULL2(const char* imgName, const char* engineName,
		unsigned char* src_ptr, int height, int width, int channel,
		int max_num, float threshold, int *set_norm_ids, int set_n,
		int is_first, int shield_bg, int dieNum, double* dieInfo, float *result,
		double *motorPos, double* calibParams, double* rectifyParams, double* mapParams,
		double* roiCornerPoint, int chipNum, double* chip_cfg, const char* src_file, const char* mask_file);
/*
@brief 返回die中的指定chipid的像素坐标值;
@param[in] col 指定die的列
@param[in] row 指定die的行
@param[in] chipid 指定chipid的索引，和下面的chip_cfg的顺序有关
@param[in] motorPos 拍摄点的机械坐标(double[2]: x/y)
@param[in] calibParams 像素标定参数(double[13])
@param[in] rectifyParams 坐标变换矩阵(double[9])
@param[in] mapParams 机械坐标与行列坐标的变换矩阵(double[9])
@param[in] chipNum
@param[in] chip_cfg chip的ROI在die中的比例(double[7*chipNum]: chip左上比例(0.1, 0.1), 右下比例(0.9, 0.9)) * [LX,LY,RX,RY, 是否屏蔽外观检测(1/0)，是否屏蔽测量偏移(1/0)， 是否屏蔽测量旋转(1/0)]
@param[out] chipcoors 返回chipid 的像素坐标位置 (x_lt,y_lt,x_rb,y_rb)
*/
	DLL_API void getChipCoors(int col, int row, int chipid,
		double *motorPos, double* calibParams, double* rectifyParams, double* mapParams,
		int chipNum, double* chip_cfg, double * chipcoors);

	/*
	@brief 提取缺陷小图特征
	@param ptr 小图数据指针
	@param height 小图的高
	@param width 小图的宽
	@param channel 小图的通道
	@param feature 提取到的特征
	@return 提取是否成功
	*/
	DLL_API bool featureExtract(unsigned char *ptr, int height, int width, int channel, float *feature);

	/*
	@brief 提取全图特征，学习coreset(带掩膜)
	@param engine 引擎名
	@param ptr 图片的数据指针
	@param ptr_mask 掩膜的数据指针
	@param height 图片的高
	@param width 图片的宽
	@param channel 图片的通道数
	@param norm_id 正常类型的id
	@param max_num 设置提取的最大缺陷特征数
	@param threshold 余弦距离阈值
	@param result 提取结果(0-3n: flaw_id, 特征x坐标-128px(对应小图左上角), 特征y坐标-128px(对应小图左上角))
	@return 提取的缺陷特征数
	*/
	DLL_API int coreLearn(const char* engine, unsigned char *ptr, unsigned char *ptr_mask, int height, int width, int channel, int norm_id, int max_num, float threshold, int* results);

	/*
	@brief 销毁引擎
	@return 销毁是否成功
	*/
	DLL_API bool destroyEngine();

	/*-----------------------测试相关-----------------------*/
	/*
	@brief 绘画结果
	@param src_data 原图的数据指针
	@param src_h 原图的高
	@param src_w 原图的宽
	@param img_name 图片名
	@param hr 成功与否
	@param bbox_size 缺陷数量
	@param result 结果buffer
	@param dst_path 结果图路径
	@return 成功与否
	*/
	DLL_API bool draw_result(unsigned char *src_data, int src_h, int src_w, int channel,
		const char *img_name, bool hr, int bbox_size, float *result,
		const char *dst_path);


#ifdef __cplusplus
}
#endif
