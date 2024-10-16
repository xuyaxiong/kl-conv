#pragma once

#if defined(DLL_EXPORTS)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	//��ӡ��汾��Ϣ
	DLL_API char* get_dll_version();

	/*-----------------------�������-----------------------*/
	/*
	@brief ����db���ڴ�
	@param name db��
	@return db����������
	*/
	DLL_API int load_DB(const char* name);

	/*
	@brief ɾ���ͷ��ڴ��е�db
	@param name db��
	*/
	DLL_API void release_DB(const char* name);

	/*
	@brief ����db���ڴ棬�������ڴ���dbʵ����ָ��
	@param name db��
	@return ָ���ڴ�dbʵ���Ŀ�ָ��
	*/
	void* DB_instance(const char* name);

	/*
	@brief ��db��������
	@param name db ��
	@param n �������������
	@param flaw_ids �����������Ӧ��ȱ��id
	@param flaw_types �����������Ӧ��ȱ������
	@param features �������������
	*/
	DLL_API int insert(const char* name, int n, int* flaw_ids, int* flaw_types, float* features);

	/*
	@brief ��db����һ������
	@param name db ��
	@param flaw_id �����������Ӧ��ȱ��id
	@param flaw_type �����������Ӧ��ȱ������
	@param feature ���������
	*/
	DLL_API int insert_one(const char* name, int flaw_id, int flaw_type, float* feature);

	/*
	@brief ɾ��db�е�����
	@param name db��
	@param n ɾ������������
	@param flaw_ids ɾ������������Ӧ��ȱ��id����
	*/
	DLL_API int erase(const char* name, int n, int* flaw_ids);

	/*
	@brief ��ȱ��id��ɾ��db�е�һ������
	@param name db��
	@param flaw_ids ɾ������������Ӧ��ȱ��id
	*/
	DLL_API int erase_one(const char* name, int flaw_id);

	/*
	@brief ��ȱ�����ͣ�ɾ��db�е�һ��������
	@param name db��
	@param flaw_ids ɾ������������Ӧ��ȱ������
	*/
	DLL_API int erase_type(const char* name, int flaw_type);

	/*
	@brief ����db�е�����
	@param name db��
	@param flaw_ids �豻���µ�������Ӧȱ��id����(��id����)
	@param flaw_types �豻���µ�������Ӧȱ�����ͼ��ϡ�ȱ��������д������ʾ�����£�ָ���Ϊ��
	@param features �豻���µ���������
	*/
	DLL_API bool update(const char* name, int n, int* flaw_ids, int* flaw_types = 0, float* features = 0);

	/*
	@brief ����db�е�һ������
	@param name db��
	@param flaw_id �豻���µ�������Ӧȱ��id(��id����)
	@param flaw_type �豻���µ�������Ӧȱ�����͡�ȱ��������д������ʾ�����£�ָ���Ϊ��
	@param feature �豻���µ�����
	*/
	DLL_API bool update_one(const char* name, int flaw_id, int flaw_type, float* feature);

	// ��ѯ����
	/*
	@brief ��ѯ������db�е����������dists��types��ids
	@param name db��
	@param n �����ѯ��������Ŀ
	@param features �����ѯ����������
	@param dists �鵽���������ж�Ӧ��������Ҿ���
	@param types �鵽���������ж�Ӧ��ȱ������
	@param ids �鵽���������ж�Ӧ��ȱ��id
	*/
	DLL_API bool query(const char* name, int n, const float* features, float* dists = 0, int* types = 0, int* ids = 0);

	/*-----------------------�������-----------------------*/
	/*
	@breif ��ʼ������
	@param dir ��onnxģ�ͻ�engine���ļ���
	@param setN ������
	@param setC ����ͼƬ��ͨ����
	@param setH ����ͼƬ�ĸ�
	@param setW ����ͼƬ�Ŀ�
	@param setD ����ά��
	@return ��ʼ���Ƿ�ɹ�
	*/
	DLL_API bool initEngine(const char* dir, int setN, int setC, int setH, int setW, int setD = 384);

	/*
	@brief ��ȡ����ά��
	@return ����ά��
	*/
	DLL_API int getFeatureDim();

	/*
	@brief ���ûص�
	@param cb �ص�����ָ��
	@param[out] updateMap ����λͼ��ȱ��(��mark)������/������ int[](row/col)(����ռ䰴������������)
	@	��ά����Ѽ쾧������(n) + ���о������к�/�к�/���ͣ�
	@	���ͣ��Ѽ�ok: 0, mark:-1, δ֪ȱ��(distMap<):-2, ng: typeMap(id)[typeMap(id)û�в鵽ʱ,��normal_id]
	@return �����Ƿ�ɹ�
	*/
	DLL_API bool setCallback(void(*cb)(const char*, bool, int, float*, int *updateMap));



	/*
	@brief ���ȱ�ݣ�����Ĥ��
	@param[in] imgName ͼƬ��
	@param[in] engineName ������
	@param[in] src_ptr ͼƬ�����׵�ַ
	@param[in] height ��
	@param[in] width ��
	@param[in] channel ͨ��
	@param[in] max_num ���ȱ����
	@param[in] threshold ������ֵ
	@param[in] set_norm_ids ��������ȱ��id����(��1��Ϊ�����������͵�id)
	@param[in] set_n ��������id�����С
	@param[in] is_first �Ƿ��ǵ�һ֡ͼ(1:true, 0:false)
	@param[in] shield_bg �Ƿ����α���(1/0)
	@param[in] shield_block_num ���μ���block����
	@param[in] shield_block ���μ���block
	�����飬����Ϊ4*shield_block_num��ÿһ��4������Ϊ��block��ʼdie�� | ��ʼdie�� | ����die�� | ����die�� ��
	@param[in] shield_die_num ���μ���die����
	@param[in] shield_die ���μ���die
	�����飬����Ϊ2*shield_die_num��ÿһ��2������Ϊ��die�� | die�� ��
	@param[in] result �����(x0,y0,w0,h0,feanture,x_lu_feat,y_lu_feat,col_lt, row_lt, col_rb, row_rb,type,score)
	@param[in] motorPos �����Ļ�е����(double[2]: x/y)
	@param[in] calibParams ���ر궨����(double[13])
	@param[in] rectifyParams ����任����(double[9])
	@param[in] mapParams ��е��������������ı任����(double[9])
	@param[in] roiCornerPoint
	@param[in] chipNum
	@param[in] chip_cfg chip��ROI��die�еı���(double[4*chipNum+3]: chip���ϱ���(0.1, 0.1), ���±���(0.9, 0.9)) * [LX,LY,RX,RY, �Ƿ�������ۼ��(1/0)���Ƿ����β���ƫ��(1/0)�� �Ƿ����β�����ת(1/0)]
	@param[in] src_file
	@param[in] mask_file
	@return ���гɹ�
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
@brief ���ȱ�ݣ�����Ĥ��; ZXK CHANGE �����Ϊ col , row , chipid, ���� col��row�����ĵ����ڵ�die
@param[in] imgName ͼƬ��
@param[in] engineName ������
@param[in] src_ptr ͼƬ�����׵�ַ
@param[in] height ��
@param[in] width ��
@param[in] channel ͨ��
@param[in] max_num ���ȱ����
@param[in] threshold ������ֵ
@param[in] set_norm_ids ��������ȱ��id����(��1��Ϊ�����������͵�id)
@param[in] set_n ��������id�����С
@param[in] is_first �Ƿ��ǵ�һ֡ͼ(1:true, 0:false)
@param[in] shield_bg �Ƿ����α���(1/0)
@param[in] dieNum die����������readDetectRegionInfo���
@param[in] dieInfo die�������У�����readDetectRegionInfo���
@param[in] result �����(x0,y0,w0,h0,feanture,x_lu_feat,y_lu_feat, col , row , chipid ,type , score)
@param[in] motorPos �����Ļ�е����(double[2]: x/y)
@param[in] calibParams ���ر궨����(double[13])
@param[in] rectifyParams ����任����(double[9])
@param[in] mapParams ��е��������������ı任����(double[9])
@param[in] roiCornerPoint
@param[in] chipNum
@param[in] chip_cfg chip��ROI��die�еı���(double[7*chipNum]: chip���ϱ���(0.1, 0.1), ���±���(0.9, 0.9)) * [LX,LY,RX,RY, �Ƿ�������ۼ��(1/0)���Ƿ����β���ƫ��(1/0)�� �Ƿ����β�����ת(1/0)]
@param[in] src_file
@param[in] mask_file
@return ���гɹ�
*/
	DLL_API bool anomalyDetect_FULL2(const char* imgName, const char* engineName,
		unsigned char* src_ptr, int height, int width, int channel,
		int max_num, float threshold, int *set_norm_ids, int set_n,
		int is_first, int shield_bg, int dieNum, double* dieInfo, float *result,
		double *motorPos, double* calibParams, double* rectifyParams, double* mapParams,
		double* roiCornerPoint, int chipNum, double* chip_cfg, const char* src_file, const char* mask_file);
/*
@brief ����die�е�ָ��chipid����������ֵ;
@param[in] col ָ��die����
@param[in] row ָ��die����
@param[in] chipid ָ��chipid���������������chip_cfg��˳���й�
@param[in] motorPos �����Ļ�е����(double[2]: x/y)
@param[in] calibParams ���ر궨����(double[13])
@param[in] rectifyParams ����任����(double[9])
@param[in] mapParams ��е��������������ı任����(double[9])
@param[in] chipNum
@param[in] chip_cfg chip��ROI��die�еı���(double[7*chipNum]: chip���ϱ���(0.1, 0.1), ���±���(0.9, 0.9)) * [LX,LY,RX,RY, �Ƿ�������ۼ��(1/0)���Ƿ����β���ƫ��(1/0)�� �Ƿ����β�����ת(1/0)]
@param[out] chipcoors ����chipid ����������λ�� (x_lt,y_lt,x_rb,y_rb)
*/
	DLL_API void getChipCoors(int col, int row, int chipid,
		double *motorPos, double* calibParams, double* rectifyParams, double* mapParams,
		int chipNum, double* chip_cfg, double * chipcoors);

	/*
	@brief ��ȡȱ��Сͼ����
	@param ptr Сͼ����ָ��
	@param height Сͼ�ĸ�
	@param width Сͼ�Ŀ�
	@param channel Сͼ��ͨ��
	@param feature ��ȡ��������
	@return ��ȡ�Ƿ�ɹ�
	*/
	DLL_API bool featureExtract(unsigned char *ptr, int height, int width, int channel, float *feature);

	/*
	@brief ��ȡȫͼ������ѧϰcoreset(����Ĥ)
	@param engine ������
	@param ptr ͼƬ������ָ��
	@param ptr_mask ��Ĥ������ָ��
	@param height ͼƬ�ĸ�
	@param width ͼƬ�Ŀ�
	@param channel ͼƬ��ͨ����
	@param norm_id �������͵�id
	@param max_num ������ȡ�����ȱ��������
	@param threshold ���Ҿ�����ֵ
	@param result ��ȡ���(0-3n: flaw_id, ����x����-128px(��ӦСͼ���Ͻ�), ����y����-128px(��ӦСͼ���Ͻ�))
	@return ��ȡ��ȱ��������
	*/
	DLL_API int coreLearn(const char* engine, unsigned char *ptr, unsigned char *ptr_mask, int height, int width, int channel, int norm_id, int max_num, float threshold, int* results);

	/*
	@brief ��������
	@return �����Ƿ�ɹ�
	*/
	DLL_API bool destroyEngine();

	/*-----------------------�������-----------------------*/
	/*
	@brief �滭���
	@param src_data ԭͼ������ָ��
	@param src_h ԭͼ�ĸ�
	@param src_w ԭͼ�Ŀ�
	@param img_name ͼƬ��
	@param hr �ɹ����
	@param bbox_size ȱ������
	@param result ���buffer
	@param dst_path ���ͼ·��
	@return �ɹ����
	*/
	DLL_API bool draw_result(unsigned char *src_data, int src_h, int src_w, int channel,
		const char *img_name, bool hr, int bbox_size, float *result,
		const char *dst_path);


#ifdef __cplusplus
}
#endif
