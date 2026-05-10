////////////////////////////////////////////////////////////////////
/// Copyright (c)2016, 山东新北洋信息技术股份有限公司
/// All rights reserved.
/// @file        DebugLog.h
/// @brief       输出日志头文件
/// @version     V1.0.1
/// @author      xuchunkai
/// @date        2016-11-10
////////////////////////////////////////////////////////////////////

#ifndef __DEBUGLOG_H__
#define __DEBUGLOG_H__

///#include "../TestSuite.h"
#include "DebugType.h"
#include "LogPrint/PrintService.h"
#include "RingBufferModule.h"
#include "DebugConfig.h"

#define DEBUG_LOG_FULL_REWRITE            0  //覆盖旧数据
#define DEBUG_LOG_FULL_STOP               1    //新数据丢失

/////////////////////////////////////////////////////////////////////
/// @brief          日志模块初始化
/// @author       xuchunkai
/// @version      v1.01
/// @date           2016-12-15
/// @param [in]  pLogBufStart                :日志模块存储缓冲区首地址
/// @param [in]  LogBufSize                   :日志缓冲区大小,总大小
/// @param [in]  LogBufFullReserveSize   :日志缓冲区满判断预留大小,推荐为最大日志大小
/// @param [in]  DebugLogFullMode        :日志满处理方式,DEBUG_LOG_FULL_REWRITE/DEBUG_LOG_FULL_STOP
/// @param [in]  pDisableInt                  :日志涉及多进程时必须传入关中断函数指针,其他可为NULL
/// @param [in]  pEnableIn                    :日志涉及多进程时必须传入关中断函数指针,其他可为NULL
/// @param [in]  pDebugLogTime           :日志时间计数函数指针,需要自行封装时间函数,其他可为NULL
/// @return        0:成功, 非0,失败错误码
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
extern U32 DebugLogInit(U8 *pLogBufStart, U32 LogBufSize, U32 LogBufFullReserveSize, U8 DebugLogFullMode
				 ,pVoidFun pDisableInt, pVoidFun pEnableInt, pU32Fun pDebugLogTime);

/////////////////////////////////////////////////////////////////////
/// @brief           调试数据输出
/// @author        xuchunkai
/// @version       v1.00
/// @date            2016-11-6
/// @param [in]   pbuf	       :调试数据保存地址
/// @param [in]   ReadLength   :调试数据读取块大小,设备缓冲区支持的大小
/// @param [in]   RealLength    :调试数据实际返回大小
/// @return           0正常返回, 1异常
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
extern U32	DebugLogDataBufOut(U8 *pbuf, U32 ReadLength, U32 *RealLength);

/////////////////////////////////////////////////////////////////////
/// @brief		获取模块版本号
/// @author        xuchunkai
/// @version      v1.01
/// @date           2016-12-15
/// @param [in]  VersionBuf       :缓冲区结构体
/// @param [in]  VersionLen       :缓冲区起始地址指针
/// @return        void
/// @attention	无
/// @see		无
/////////////////////////////////////////////////////////////////////
extern void DebugLogGetVersion(U8 *const VersionBuf, U32 *const VersionLen);


/////////////////////////////////////////////////////////////////////
/// @brief          日志模块全局等级设置
/// @author       xuchunkai
/// @version      v1.01
/// @date           2016/11/21
/// @param [in]   Level     :全局等级设置;支持全局等级的模块只要模块等级低于设置值,
///                       即可显示；高于该等级时，按照自身设计等级显示。
/// @return
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
extern void DebugLogGlobalLevelSet(U8 Level);


/////////////////////////////////////////////////////////////////////
/// @brief       获取当前已经记录的日志大小
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/04/19
/// @param [in]   U32 *pSize :传入用于保存日志大小的指针
/// @return
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
extern U32 DebugLogGetUsedSize(U32 *pSize);


/////////////////////////////////////////////////////////////////////
/// @brief       获取日志当前缓冲区读写指针
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/04/19
/// @param [in]  U8 *pReadPt:读指针
/// @param [in]  U8 *pWritePt:写指针
/// @return
/// @attention     读写指针的缓冲区首位地址在初始化时已知，本处并未再外传。
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
extern U32 DebugLogGetDataPt(U8 *pReadPt, U8 *pWritePt);

#ifndef DEBUG_LOG_LEVEL_NULL
#define DEBUG_LOG_LEVEL_NULL		0
#endif

#ifndef DEBUG_LOG_LEVEL_ERR
#define DEBUG_LOG_LEVEL_ERR			1
#endif

#ifndef DEBUG_LOG_LEVEL_WARNING
#define DEBUG_LOG_LEVEL_WARNING		2
#endif

#ifndef DEBUG_LOG_LEVEL_INFO
#define DEBUG_LOG_LEVEL_INFO		3
#endif

#ifndef DEBUG_LOG_LEVEL_TRACE
#define DEBUG_LOG_LEVEL_TRACE		4
#endif

///
///@defgroup Module_LogSet 用户可用的日志记录方式
///@{
///@par 不同形式的日志记录
///
#ifdef _TEST_DEBUG_LOG

#ifdef DEBUG_LOG_TIME_OUTPUT_STRING_ENABLE

/////////////////////////////////////////////////////////////////////
/// @brief        错误级别日志输出
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/11/18
/// @param [in]   type:
/// @param [in]   message:待输出的字符串,与printf类似
/// @return
/// @attention     输出的字符串需要额外增加个括号;只有定义的模块等级不低于该级别时才会输出(ERR/INFO/TRACE时输出日志)
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////

	#define DEBUG_LOG_ERR(type,message)  do { if ((type) >= DEBUG_LOG_LEVEL_ERR) {									\
												 if (pDebugLogDI != STD_NULL)	pDebugLogDI();						\
												 DEBUG_LOG_PRINTF(LOG_HEAD"[E]:%s\t",pfDebugLogTime()); 			\
												 DEBUG_LOG_PRINTF message;											\
												 if (pDebugLogEI != STD_NULL) 	pDebugLogEI();}} while (0)

												 /////////////////////////////////////////////////////////////////////
/// @brief        警告级别日志输出
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/11/18
/// @param [in]   type:
/// @param [in]   message:待输出的字符串,与printf类似
/// @return
/// @attention     输出的字符串需要额外增加个括号;只有定义的模块等级不低于该级别时才会输出(ERR/INFO/TRACE时输出日志)
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////

	#define DEBUG_LOG_WARNING(type,message)  do { if ((type) >= DEBUG_LOG_LEVEL_WARNING) {							\
												 if (pDebugLogDI != STD_NULL)	pDebugLogDI();						\
												 DEBUG_LOG_PRINTF(LOG_HEAD"[W]:%s\t",pfDebugLogTime()); 			\
												 DEBUG_LOG_PRINTF message;											\
												 if (pDebugLogEI != STD_NULL) 	pDebugLogEI();}} while (0)
												 
/////////////////////////////////////////////////////////////////////
/// @brief        提示级别日志输出
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/11/18
/// @param [in]   type:
/// @param [in]   message:待输出的字符串,与printf类似
/// @return
/// @attention     输出的字符串需要额外增加个括号;只有定义的模块等级高于该级别时才会输出(INFO/TRACE时输出日志)
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////

	#define DEBUG_LOG_INFO(type,message)  do { if ((type) >= DEBUG_LOG_LEVEL_INFO) {								\
 												 if (pDebugLogDI != STD_NULL)	pDebugLogDI();						\
												 DEBUG_LOG_PRINTF(LOG_HEAD"[I]:%s\t",pfDebugLogTime());				\
												 DEBUG_LOG_PRINTF message;											\
												 if (pDebugLogEI != STD_NULL) 	pDebugLogEI();}} while (0)

/////////////////////////////////////////////////////////////////////
/// @brief        记录级别日志输出
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/11/18
/// @param [in]   type:
/// @param [in]   message:待输出的字符串,与printf类似
/// @return
/// @attention     输出的字符串需要额外增加个括号;只有定义的模块等级高于该级别时才会输出(TRACE时输出日志)
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
	#define DEBUG_LOG_TRACE(type,message)  do { if ((type) >= DEBUG_LOG_LEVEL_TRACE) {  							\
	 											 if (pDebugLogDI != STD_NULL)	pDebugLogDI();						\
												 DEBUG_LOG_PRINTF(LOG_HEAD"[T]:%s\t",pfDebugLogTime()); 			\
												 DEBUG_LOG_PRINTF message;											\
												 if (pDebugLogEI != STD_NULL) 	pDebugLogEI();}} while (0)
												 
#else

/////////////////////////////////////////////////////////////////////
/// @brief        错误级别日志输出
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/11/18
/// @param [in]   type:
/// @param [in]   message:待输出的字符串,与printf类似
/// @return
/// @attention     输出的字符串需要额外增加个括号;只有定义的模块等级不低于该级别时才会输出(ERR/INFO/TRACE时输出日志)
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////

	#define DEBUG_LOG_ERR(type,message)  do { if ((type) >= DEBUG_LOG_LEVEL_ERR) {									\
												 if (pDebugLogDI != STD_NULL)	pDebugLogDI();						\
												 DEBUG_LOG_PRINTF(LOG_HEAD"[E]:%d\t",pfDebugLogTime()); 			\
												 DEBUG_LOG_PRINTF message;											\
												 if (pDebugLogEI != STD_NULL) 	pDebugLogEI();}} while (0)

												 /////////////////////////////////////////////////////////////////////
/// @brief        警告级别日志输出
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/11/18
/// @param [in]   type:
/// @param [in]   message:待输出的字符串,与printf类似
/// @return
/// @attention     输出的字符串需要额外增加个括号;只有定义的模块等级不低于该级别时才会输出(ERR/INFO/TRACE时输出日志)
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////

	#define DEBUG_LOG_WARNING(type,message)  do { if ((type) >= DEBUG_LOG_LEVEL_WARNING) {							\
												 if (pDebugLogDI != STD_NULL)	pDebugLogDI();						\
												 DEBUG_LOG_PRINTF(LOG_HEAD"[W]:%d\t",pfDebugLogTime()); 			\
												 DEBUG_LOG_PRINTF message;											\
												 if (pDebugLogEI != STD_NULL) 	pDebugLogEI();}} while (0)
												 
/////////////////////////////////////////////////////////////////////
/// @brief        提示级别日志输出
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/11/18
/// @param [in]   type:
/// @param [in]   message:待输出的字符串,与printf类似
/// @return
/// @attention     输出的字符串需要额外增加个括号;只有定义的模块等级高于该级别时才会输出(INFO/TRACE时输出日志)
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////

	#define DEBUG_LOG_INFO(type,message)  do { if ((type) >= DEBUG_LOG_LEVEL_INFO) {								\
 												 if (pDebugLogDI != STD_NULL)	pDebugLogDI();						\
												 DEBUG_LOG_PRINTF(LOG_HEAD"[I]:%d\t",pfDebugLogTime());				\
												 DEBUG_LOG_PRINTF message;											\
												 if (pDebugLogEI != STD_NULL) 	pDebugLogEI();}} while (0)

/////////////////////////////////////////////////////////////////////
/// @brief        记录级别日志输出
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/11/18
/// @param [in]   type:
/// @param [in]   message:待输出的字符串,与printf类似
/// @return
/// @attention     输出的字符串需要额外增加个括号;只有定义的模块等级高于该级别时才会输出(TRACE时输出日志)
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
	#define DEBUG_LOG_TRACE(type,message)  do { if ((type) >= DEBUG_LOG_LEVEL_TRACE) {  							\
	 											 if (pDebugLogDI != STD_NULL)	pDebugLogDI();						\
												 DEBUG_LOG_PRINTF(LOG_HEAD"[T]:%d\t",pfDebugLogTime()); 			\
												 DEBUG_LOG_PRINTF message;											\
												 if (pDebugLogEI != STD_NULL) 	pDebugLogEI();}} while (0)
												
#endif

/////////////////////////////////////////////////////////////////////
/// @brief        无头格式的原始日志输出
/// @author       xuchunkai
/// @version      v1.01
/// @date         2017/11/18
/// @param [in]   type:
/// @param [in]   message:待输出的字符串,与printf类似
/// @return
/// @attention    没有格式的文件输出,一般用于输出十六进制数据流等情况
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
	#define DEBUG_LOG(type,message)  do { if ((type) >= DEBUG_LOG_LEVEL_TRACE) {							\
	 										     if (pDebugLogDI != STD_NULL)	pDebugLogDI();				\
												 DEBUG_LOG_PRINTF message;									\
												 if (pDebugLogEI != STD_NULL) 	pDebugLogEI();}} while (0)

///
///@}
///
#else

	#define DEBUG_LOG_ERR(type,message)
	#define DEBUG_LOG_WARNING(type,message)
	#define DEBUG_LOG_INFO(type,message)
	#define DEBUG_LOG_TRACE(type,message)
	#define DEBUG_LOG(type,message)

#endif


#ifdef _TEST_DEBUG_ASSERT
///
///@defgroup Module_ASSERT 用户可用的断言
///@{
///@par 用于主动检查代码执行到一定位置时的变量、内存等是否符合预期。
///
	#define DEBUG_LOG_ASSERT(EX)   if (!(EX)) {volatile char dummy = 0;					\
							if (pDebugLogDI != STD_NULL)	pDebugLogDI();				\
                            DEBUG_LOG_PRINTF("(%s) assert failed at %s:%d \n", 			\
                            #EX, __FUNCTION__, __LINE__);								\
							if (pDebugLogEI != STD_NULL) 	pDebugLogEI();				\
							while (dummy != 0);}
///
///@}
///
#else
	#define DEBUG_LOG_ASSERT(EX)
#endif

#endif

