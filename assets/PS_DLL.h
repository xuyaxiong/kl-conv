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
	
	/*=================================step1:初始化图像检测类对象, 并重新计算背景平均灰度值======================================*/
	/*
	@param[in]   const char*  objectName     类实例名称（等于工台位名称/相机名称）开始、复位时调用。
	@param[in]   const char*  logDir               日志存放目录。D:\\LOG\\    ,  生成后的日志等于D:\\LOG\\objectName.xxxx
	@param[in]   uchar*       backgroudData   传入无物料时的背景图像数据
	@param[in]   int*         imgHWC                图像高宽通道数
	*/
	PS_API bool createPsObject(const char* objectName, const char* logDir, uchar* backgroudData, int* imgHWC);



	/*..........................step2: 初始化对应实例的基础参数,初始化中的参数软件端自行设计对应的窗口.................................*/
	/*
	@param[in]  const char*  objectName     类实例名称（等于工台位名称/相机名称）
	@param[in]  int*      grabImgHW              采集图像的像素高宽
	@param[in]  bool    dark2Light                 物料从低灰度值变为高灰度值为物料的起始方向
	@param[in]  float    startOrStopThred      物料的启停位置判定阈值
	@param[in]  int       overlap                      拼接的overlap高度
	@param[in]  int       noiseThred                背景抑制阈值，不处理设置为0
	@param[in]  float*  spotParams               斑点缺陷检出参数[contrastRange, segThreshX, segThreshY]
	@param[in]  float*  scratchParams           划痕检出参数[lowThresh, highThresh, scratchSigma]
	@param[in]  float    materialSegBias        分割物料和背景，当前景比背景亮时使用正值(工位1和2)， 相反使用负值(工位3)。
	*/
	PS_API bool initPsParams(const char* objectName, int* grabImgHW, bool dark2Light, float startOrStopThred, int overlap, int noiseThred, float* spotParams, float* scratchParams, float materialSegBias);




	/*=================step3: 时时将采集的图像数据给该接口,判定是否送检测。 如果外部能够判断启停，那么依然需要调用该接口，只不过返回值被外部判断值替换======================*/
	/*
	brief: 判定当前图像的状态：处于开始位置、结束位置、中间位置、背景位置
	@param[in]     const char*     objectName          类实例名称（等于工台位名称/相机名称）
	@param[in]     uchar*             imgData                当前图像数据,时时抓取的图像
	@param[in]     int*                  imgHWC               图像的高宽通道
	@param[in]     bool                hardJudgment      是否开启使用传感器判断，若开启将会自动关闭软判断。
	@param[in]     bool                trigger                   当前帧图像是否触发（每片物料只需要触发一次）
	@param[in]     int                   capturedPictures  当前物料需要采集几张图片，比计算的实际值多1张
	@param[out]    uchar*           detetedImg           生成的待送检测图像。
	return  -1: 出现异常，0：背景位置，1：开始位置，2：结束位置， 3：物料中间位置。
	*/
	PS_API int  determineImgStatus(const char* objectName, uchar*  imgData, int* imgHWC, bool  hardJudgment, bool trigger, int capturedPictures, uchar*  detetedImg);


	/*
	brief: 生成送Anomaly和传统检测算法中需要的mask. 可以选择对图像是否做背景过滤操作。
	@param[in]          const char*     objectName       类实例名称（等于工台位名称）
	@param[in]          uchar*   detetedImg  待送检测图像。(检测图像高等于原始+overlap，单通道到图像数据)，该输入等于determineImgStatus的输出。
	@param[in]          int*     imgHWC      送检图像的高宽通道
	@param[in/out]   uchar*   maskImg     生成的送anomaly的检测掩码图像数据
	@param[in]		int      dilateBand      外扩的像素数（默认80）
	@param[in]     int      erodeBand      内缩的像素数（默认120）
	@param[out]   int*     dilateRoi         对应的物料区域[x, y, w, h]，物料的anomaly检测区域
	@param[out]   int*     erodeRoi         返回传统检测的图像目标范围[x,y,w,h]，传统检测算法区域。更改，替换为原始的定位区域坐标
	return -1： 警告码（定位到多个区域，不影响结果）。 0表示匹配失败。 1表示匹配结果正常
	*/
	PS_API int generateMask(const char * objectName, uchar* detetedImg, int* imgHWC, uchar* maskImg, int  dilateBand, int  erodeBand, int* dilateRoi, int* erodeRoi);


	/*
	brief: 生成送Anomaly和传统检测算法中需要的mask和roi.
	@param[in]          const char*     objectName       类实例名称（等于工台位名称）
	@param[in]          uchar*            detetedImg        待送检测图像。(检测图像高等于原始+overlap，单通道到图像数据)，该输入等于determineImgStatus的输出。
	@param[in]          int*                 imgHWC            送检图像的高宽通道
	@param[out]       uchar*            deepMask          生成的送anomaly的检测掩码图像数据(单通道图像数据，size和输入的图像大小一致)
	@param[out]       uchar*            traMask              生成的送传统检测的掩码图像数据(单通道图像数据，size和输入的图像大小一致)
	return -1： 警告码（定位到多个区域，不影响结果）。 0表示匹配失败。 1表示匹配结果正常
	*/
	PS_API int generateMaskOpt(const char * objectName, uchar* detetedImg, int* imgHWC, uchar* deepMask, uchar* traMask);




	/*================step4: 将determineImgStatus返回的图像送入传统检测接口中，如果上面接口返回值大于0======================*/
	/*
	@param[in]     const char*     objectName     类实例名称（等于工台位名称）
	@param[in]     uchar*            detetedImg      生成的待送检测图像,如果为为true。
	@param[in]     int*                 imgHWC              生成图像的高宽通道数
	@param[in]     int*                erodeRoi              返回传统检测的图像目标范围[x,y,w,h]上面接口输出。
	@param[in]     int                 startFunction        启动功能（0：不使用传统检测，1：都使用，2：使用孤立点检测，3：使用划痕检测）	
	@param[out]    int*              resultRectangles  检测结果包围框（长度被限制在100以内，左上角和宽高(x,y,w,h)）
	return  返回的是检测到的结果数量
	*/
	PS_API int performTraDetection(const char* objectName, uchar* detetedImg, int*  imgHWC, int* erodeRoi, int startFunction,  int* resultRectangles);

	/*
	@param[in]         const char*     objectName     类实例名称（等于工台位名称）
	@param[in]         uchar*             detetedImg      生成的待送检测图像,如果为为true。
	@param[in]         int*                  imgHWC              生成图像的高宽通道数
	@param[in]         uchar*            deepMask          生成的送anomaly的检测掩码图像数据(单通道图像数据，size和detetedImg的大小一致)
	@param[in]         uchar*            traMask              生成的送传统检测的掩码图像数据(单通道图像数据，size和detetedImg的大小一致)
	@param[in]         int                  startFunction        启动功能（0：不使用传统检测，1：都使用，2：仅使用孤立点检测，3：仅使用划痕检测）
	@param[out]      int*                 resultRectangles  检测结果包围框（长度被限制在100以内，左上角和宽高(x,y,w,h)）
	return  返回的是检测到的结果数量
	*/
	PS_API int performTraDetectionOpt(const char* objectName, uchar* detetedImg, int*  imgHWC, uchar*  deepMask, uchar*   traMask, int startFunction, int* resultRectangles);



	/*=================获取指定工作台已经处理完成的物料数量，以及当前正在处理新物料的图像张数==========================*/
	/*
	@param[in]     const char*     objectName              类实例名称（等于工台位名称）
	@param[out]   int*                 scannedNums           已经扫描完成的物料数量
	@param[out]   int*                 currentIndex              当前正在扫描物料对应的图像张数（也可利用该参数用于判定当前工位是否存在物料）
	*/
	PS_API void  getScannedPsNum(const char*     objectName,   int*  scannedNums, int* currentIndex);


	/*=================绘制缺陷到物料，传统和深度检测结果使用不同的颜色绘制=========================*/
	/*
	@param[in]  uchar* detetedImg     送检测的图像数据
	@param[in]  int*      imgHWC         图像的高宽通道数
	@param[in]  int*      dilateRoi         物料所在区域[x,y,w,h]。 如果将w,h设置0表示会进行绘制
	@param[in]  int*      erodeRoi         传统算法的检测区域[x,y,w,h]，如果将w,h设置0表示会进行绘制
	@param[in]  int*      traResult         传统图像检测结果[x,y,w,h]
	@param[in]  int       traLen    传统图像结果数据长度
	@param[in]  int*      deepResult      anomaly检测结果[x,y,w,h]
	@param[in]  int       deepLen   传统图像结果数据长度     
	@param[out]   uchar* drawImg    被绘制结果的三通道图像数据。
	*/
	PS_API void drawDfectRectangle(uchar* detetedImg, int*  imgHWC, int* dilateRoi , int*  erodeRoi, int* traResult, int traLen, int*  deepResult, int  deepLen, uchar* drawImg);


	/*
	@param[in]  uchar* detetedImg           送检测的图像数据
	@param[in]  int*            imgHWC         图像的高宽通道数
	@param[in]  uchar*       deepMask       生成的送anomaly的检测掩码图像数据(单通道图像数据，size和detetedImg的大小一致), 可以传递null
	@param[in]  uchar*       traMask           生成的送传统检测的掩码图像数据(单通道图像数据，size和detetedImg的大小一致), 可以传递null
	@param[in]  int*            traResult         传统图像检测结果[x,y,w,h]
	@param[in]  int             traLen              传统图像结果数据长度
	@param[in]  int*            deepResult     anomaly检测结果[x,y,w,h]
	@param[in]  int             deepLen          传统图像结果数据长度
	@param[out]   uchar*   drawImg          被绘制结果的三通道图像数据。传统是红色，anomaly是绿色
	*/
	PS_API void drawDfectRectangleOpt(uchar* detetedImg, int*  imgHWC, uchar* deepMask, uchar* traMask, int* traResult, int traLen, int*  deepResult, int  deepLen, uchar* drawImg);


#ifdef __cplusplus
}
#endif
