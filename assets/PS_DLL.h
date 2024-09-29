#pragma once
#include<opencv2/opencv.hpp>
#include <iostream>
#include <fstream>


#if defined(GLASS_EXPORTS)
#define PS_API __declspec(dllexport)
#else
#define PS_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif
	
	/*=================================step1:��ʼ��ͼ���������, �����¼��㱳��ƽ���Ҷ�ֵ======================================*/
	/*
	@param[in]   const char*  objectName     ��ʵ�����ƣ����ڹ�̨λ����/������ƣ���ʼ����λʱ���á�
	@param[in]   const char*  logDir               ��־���Ŀ¼��D:\\LOG\\    ,  ���ɺ����־����D:\\LOG\\objectName.xxxx
	@param[in]   uchar*       backgroudData   ����������ʱ�ı���ͼ������
	@param[in]   int*         imgHWC                ͼ��߿�ͨ����
	*/
	PS_API bool createPsObject(const char* objectName, const char* logDir, uchar* backgroudData, int* imgHWC);



	/*..........................step2: ��ʼ����Ӧʵ���Ļ�������,��ʼ���еĲ��������������ƶ�Ӧ�Ĵ���.................................*/
	/*
	@param[in]  const char*  objectName     ��ʵ�����ƣ����ڹ�̨λ����/������ƣ�
	@param[in]  int*      grabImgHW              �ɼ�ͼ������ظ߿�
	@param[in]  bool    dark2Light                 ���ϴӵͻҶ�ֵ��Ϊ�߻Ҷ�ֵΪ���ϵ���ʼ����
	@param[in]  float    startOrStopThred      ���ϵ���ͣλ���ж���ֵ
	@param[in]  int       overlap                      ƴ�ӵ�overlap�߶�
	@param[in]  int       noiseThred                ����������ֵ������������Ϊ0
	@param[in]  float*  spotParams               �ߵ�ȱ�ݼ������[contrastRange, segThreshX, segThreshY]
	@param[in]  float*  scratchParams           ���ۼ������[lowThresh, highThresh, scratchSigma]
	@param[in]  float    materialSegBias        �ָ����Ϻͱ�������ǰ���ȱ�����ʱʹ����ֵ(��λ1��2)�� �෴ʹ�ø�ֵ(��λ3)��
	*/
	PS_API bool initPsParams(const char* objectName, int* grabImgHW, bool dark2Light, float startOrStopThred, int overlap, int noiseThred, float* spotParams, float* scratchParams, float materialSegBias);




	/*=================step3: ʱʱ���ɼ���ͼ�����ݸ��ýӿ�,�ж��Ƿ��ͼ�⡣ ����ⲿ�ܹ��ж���ͣ����ô��Ȼ��Ҫ���øýӿڣ�ֻ��������ֵ���ⲿ�ж�ֵ�滻======================*/
	/*
	brief: �ж���ǰͼ���״̬�����ڿ�ʼλ�á�����λ�á��м�λ�á�����λ��
	@param[in]     const char*     objectName          ��ʵ�����ƣ����ڹ�̨λ����/������ƣ�
	@param[in]     uchar*             imgData                ��ǰͼ������,ʱʱץȡ��ͼ��
	@param[in]     int*                  imgHWC               ͼ��ĸ߿�ͨ��
	@param[in]     bool                hardJudgment      �Ƿ���ʹ�ô������жϣ������������Զ��ر����жϡ�
	@param[in]     bool                trigger                   ��ǰ֡ͼ���Ƿ񴥷���ÿƬ����ֻ��Ҫ����һ�Σ�
	@param[in]     int                   capturedPictures  ��ǰ������Ҫ�ɼ�����ͼƬ���ȼ����ʵ��ֵ��1��
	@param[out]    uchar*           detetedImg           ���ɵĴ��ͼ��ͼ��
	return  -1: �����쳣��0������λ�ã�1����ʼλ�ã�2������λ�ã� 3�������м�λ�á�
	*/
	PS_API int  determineImgStatus(const char* objectName, uchar*  imgData, int* imgHWC, bool  hardJudgment, bool trigger, int capturedPictures, uchar*  detetedImg);


	/*
	brief: ������Anomaly�ʹ�ͳ����㷨����Ҫ��mask. ����ѡ���ͼ���Ƿ����������˲�����
	@param[in]          const char*     objectName       ��ʵ�����ƣ����ڹ�̨λ���ƣ�
	@param[in]          uchar*   detetedImg  ���ͼ��ͼ��(���ͼ��ߵ���ԭʼ+overlap����ͨ����ͼ������)�����������determineImgStatus�������
	@param[in]          int*     imgHWC      �ͼ�ͼ��ĸ߿�ͨ��
	@param[in/out]   uchar*   maskImg     ���ɵ���anomaly�ļ������ͼ������
	@param[in]		int      dilateBand      ��������������Ĭ��80��
	@param[in]     int      erodeBand      ��������������Ĭ��120��
	@param[out]   int*     dilateRoi         ��Ӧ����������[x, y, w, h]�����ϵ�anomaly�������
	@param[out]   int*     erodeRoi         ���ش�ͳ����ͼ��Ŀ�귶Χ[x,y,w,h]����ͳ����㷨���򡣸��ģ��滻Ϊԭʼ�Ķ�λ��������
	return -1�� �����루��λ��������򣬲�Ӱ�������� 0��ʾƥ��ʧ�ܡ� 1��ʾƥ��������
	*/
	PS_API int generateMask(const char * objectName, uchar* detetedImg, int* imgHWC, uchar* maskImg, int  dilateBand, int  erodeBand, int* dilateRoi, int* erodeRoi);


	/*
	brief: ������Anomaly�ʹ�ͳ����㷨����Ҫ��mask��roi.
	@param[in]          const char*     objectName       ��ʵ�����ƣ����ڹ�̨λ���ƣ�
	@param[in]          uchar*            detetedImg        ���ͼ��ͼ��(���ͼ��ߵ���ԭʼ+overlap����ͨ����ͼ������)�����������determineImgStatus�������
	@param[in]          int*                 imgHWC            �ͼ�ͼ��ĸ߿�ͨ��
	@param[out]       uchar*            deepMask          ���ɵ���anomaly�ļ������ͼ������(��ͨ��ͼ�����ݣ�size�������ͼ���Сһ��)
	@param[out]       uchar*            traMask              ���ɵ��ʹ�ͳ��������ͼ������(��ͨ��ͼ�����ݣ�size�������ͼ���Сһ��)
	return -1�� �����루��λ��������򣬲�Ӱ�������� 0��ʾƥ��ʧ�ܡ� 1��ʾƥ��������
	*/
	PS_API int generateMaskOpt(const char * objectName, uchar* detetedImg, int* imgHWC, uchar* deepMask, uchar* traMask);




	/*================step4: ��determineImgStatus���ص�ͼ�����봫ͳ���ӿ��У��������ӿڷ���ֵ����0======================*/
	/*
	@param[in]     const char*     objectName     ��ʵ�����ƣ����ڹ�̨λ���ƣ�
	@param[in]     uchar*            detetedImg      ���ɵĴ��ͼ��ͼ��,���ΪΪtrue��
	@param[in]     int*                 imgHWC              ����ͼ��ĸ߿�ͨ����
	@param[in]     int*                erodeRoi              ���ش�ͳ����ͼ��Ŀ�귶Χ[x,y,w,h]����ӿ������
	@param[in]     int                 startFunction        �������ܣ�0����ʹ�ô�ͳ��⣬1����ʹ�ã�2��ʹ�ù������⣬3��ʹ�û��ۼ�⣩	
	@param[out]    int*              resultRectangles  �������Χ�򣨳��ȱ�������100���ڣ����ϽǺͿ��(x,y,w,h)��
	return  ���ص��Ǽ�⵽�Ľ������
	*/
	PS_API int performTraDetection(const char* objectName, uchar* detetedImg, int*  imgHWC, int* erodeRoi, int startFunction,  int* resultRectangles);

	/*
	@param[in]         const char*     objectName     ��ʵ�����ƣ����ڹ�̨λ���ƣ�
	@param[in]         uchar*             detetedImg      ���ɵĴ��ͼ��ͼ��,���ΪΪtrue��
	@param[in]         int*                  imgHWC              ����ͼ��ĸ߿�ͨ����
	@param[in]         uchar*            deepMask          ���ɵ���anomaly�ļ������ͼ������(��ͨ��ͼ�����ݣ�size��detetedImg�Ĵ�Сһ��)
	@param[in]         uchar*            traMask              ���ɵ��ʹ�ͳ��������ͼ������(��ͨ��ͼ�����ݣ�size��detetedImg�Ĵ�Сһ��)
	@param[in]         int                  startFunction        �������ܣ�0����ʹ�ô�ͳ��⣬1����ʹ�ã�2����ʹ�ù������⣬3����ʹ�û��ۼ�⣩
	@param[out]      int*                 resultRectangles  �������Χ�򣨳��ȱ�������100���ڣ����ϽǺͿ��(x,y,w,h)��
	return  ���ص��Ǽ�⵽�Ľ������
	*/
	PS_API int performTraDetectionOpt(const char* objectName, uchar* detetedImg, int*  imgHWC, uchar*  deepMask, uchar*   traMask, int startFunction, int* resultRectangles);



	/*=================��ȡָ������̨�Ѿ�������ɵ������������Լ���ǰ���ڴ��������ϵ�ͼ������==========================*/
	/*
	@param[in]     const char*     objectName              ��ʵ�����ƣ����ڹ�̨λ���ƣ�
	@param[out]   int*                 scannedNums           �Ѿ�ɨ����ɵ���������
	@param[out]   int*                 currentIndex              ��ǰ����ɨ�����϶�Ӧ��ͼ��������Ҳ�����øò��������ж���ǰ��λ�Ƿ�������ϣ�
	*/
	PS_API void  getScannedPsNum(const char*     objectName,   int*  scannedNums, int* currentIndex);


	/*=================����ȱ�ݵ����ϣ���ͳ����ȼ����ʹ�ò�ͬ����ɫ����=========================*/
	/*
	@param[in]  uchar* detetedImg     �ͼ���ͼ������
	@param[in]  int*      imgHWC         ͼ��ĸ߿�ͨ����
	@param[in]  int*      dilateRoi         ������������[x,y,w,h]�� �����w,h����0��ʾ����л���
	@param[in]  int*      erodeRoi         ��ͳ�㷨�ļ������[x,y,w,h]�������w,h����0��ʾ����л���
	@param[in]  int*      traResult         ��ͳͼ������[x,y,w,h]
	@param[in]  int       traLen    ��ͳͼ�������ݳ���
	@param[in]  int*      deepResult      anomaly�����[x,y,w,h]
	@param[in]  int       deepLen   ��ͳͼ�������ݳ���     
	@param[out]   uchar* drawImg    �����ƽ������ͨ��ͼ�����ݡ�
	*/
	PS_API void drawDfectRectangle(uchar* detetedImg, int*  imgHWC, int* dilateRoi , int*  erodeRoi, int* traResult, int traLen, int*  deepResult, int  deepLen, uchar* drawImg);


	/*
	@param[in]  uchar* detetedImg           �ͼ���ͼ������
	@param[in]  int*            imgHWC         ͼ��ĸ߿�ͨ����
	@param[in]  uchar*       deepMask       ���ɵ���anomaly�ļ������ͼ������(��ͨ��ͼ�����ݣ�size��detetedImg�Ĵ�Сһ��), ���Դ���null
	@param[in]  uchar*       traMask           ���ɵ��ʹ�ͳ��������ͼ������(��ͨ��ͼ�����ݣ�size��detetedImg�Ĵ�Сһ��), ���Դ���null
	@param[in]  int*            traResult         ��ͳͼ������[x,y,w,h]
	@param[in]  int             traLen              ��ͳͼ�������ݳ���
	@param[in]  int*            deepResult     anomaly�����[x,y,w,h]
	@param[in]  int             deepLen          ��ͳͼ�������ݳ���
	@param[out]   uchar*   drawImg          �����ƽ������ͨ��ͼ�����ݡ���ͳ�Ǻ�ɫ��anomaly����ɫ
	*/
	PS_API void drawDfectRectangleOpt(uchar* detetedImg, int*  imgHWC, uchar* deepMask, uchar* traMask, int* traResult, int traLen, int*  deepResult, int  deepLen, uchar* drawImg);


#ifdef __cplusplus
}
#endif
