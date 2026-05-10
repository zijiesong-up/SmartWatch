////////////////////////////////////////////////////////////////////
/// Copyright (c)2016, 山东新北洋信息技术股份有限公司
/// All rights reserved.
/// @file        PrintService.h
/// @brief       实现调试信息格式化功能,实现printf,printf格式化实现部分移植自rtt;并引用环形缓冲区模块RingBuff
/// @version     V1.0.0
/// @author      xuchunkai
/// @date        2016-11-10
////////////////////////////////////////////////////////////////////

#ifndef __PRINT_SERVICE_H__
#define __PRINT_SERVICE_H__

#include "../DebugLog.h"

///
///@defgroup Module_Info 日志模块版本号
///@{
///@par 获取日志模块版本号
///
void DebugLogGetVersion(U8 *const VersionBuf, U32 *const VersionLen);
///
///@}
///

///
///@defgroup Module_Init 日志模块初始化
///@{
///@par 初始化日志模块
///
U32 DebugLogInit(U8 *pLogBufStart, U32 LogBufSize, U32 LogBufFullReserveSize, U8 DebugLogFullMode
				 ,pVoidFun pDisableInt, pVoidFun pEnableInt, pU32Fun pDebugLogTime);
///
///@}
///


///
///@ingroup Module_LevelSet
///
void DebugLogGlobalLevelSet(U8 Level);


///
///@defgroup Module_LogPut 日志格式化输入到缓冲区
///@{
///@par 将日志数据写入环形缓冲区
///
void DEBUG_LOG_PRINTF(const char *fmt, ...);
///
///@}
///

///
///@defgroup Module_LogOut 日志模块调试数据输出
///@{
///@par 从环形缓冲区输出日志数据
///
U32	DebugLogDataBufOut(U8 *pbuf, U32 ReadLength, U32 *RealLength);
///
///@}
///

///
///@defgroup Module_LogOut 日志模块调试数据输出
///@{
///@par 获取已使用的日志空间大小
///
U32 DebugLogGetUsedSize(U32 *pSize);
///
///@}
///

///
///@defgroup Module_LogOut 日志模块调试数据输出
///@{
///@par 获取日志当前缓冲区读写指针
///
U32 DebugLogGetDataPt(U8 *pReadPt, U8 *pWritePt);
///
///@}
///

///
///@defgroup Module_DataPut 直接数据输出
///@{
///@par 将数据直接写入环形缓冲区
///
void DEBUG_DATA_PRINTF(U8 *Buf,U32 Len);
///
///@}
///

///
///@defgroup Module_CallbackSet 日志输出回调设置
///@{
///@par 日志输出回调设置
///
void DebugLogPrintfCallbackSet(void (*CallbackFunction)(U8* DataBuffer, U32 DataLength));
///
///@}
///

extern pU32Fun pfDebugLogTime;
extern pVoidFun pDebugLogDI;
extern pVoidFun pDebugLogEI;

#ifdef Test1
U8 DebugLogDataBufPutOtherTest1(void);
U8 DebugLogDataBufPutOtherTest2(void);
U32 RtVsnprintfTest(const char *fmt, ...);
U32 DebugLogDataBufPut1(void *pData, const U32 Length);
#endif


#endif
