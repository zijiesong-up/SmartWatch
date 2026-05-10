#ifndef __SEGGER_RTT_H__
#define __SEGGER_RTT_H__

#include "StdSnbc.h"
#include "SEGGER_RTT.H"

/////////////////////////////////////////////////////////////////////
/// @brief		   Segger RTT基本初始化函数
/// @author       qizhilong
/// @version      v1.00
/// @date         2018-04-14
/// @return
/// @attention	无
/// @see		无
/// @todo
/////////////////////////////////////////////////////////////////////

void SeggerRttInit(void);

/////////////////////////////////////////////////////////////////////
/// @brief		向Segger RTT通道中写入数据
/// @author       qizhilong
/// @version      v1.00
/// @date         2018-04-14
/// @param [in]  BufferIndex       :通道编号
/// @param [in]  Buff      		:缓冲区起始地址指针
/// @param [in]  Size       		:缓冲区大小
/// @return
/// @attention	无
/// @see		无
/// @todo
/////////////////////////////////////////////////////////////////////

U8 SeggerRttWrite(U8 BufferIndex, const void* Buff, U8 Size);

/////////////////////////////////////////////////////////////////////
/// @brief		配置Segger RTT up通道参数
/// @author       qizhilong
/// @version      v1.00
/// @date         2018-04-14
/// @param [in]  BufferIndex       :通道编号
/// @param [in]  sName      		:通道名称（字符串）
/// @param [in]  pBuffer       	:缓冲区起始地址指针
/// @param [in]  BufferSize       	:缓冲区大小
/// @param [in]  Flags       		:配置标志
/// 								SEGGER_RTT_MODE_NO_BLOCK_SKIP    	// Skip. Do not block, output nothing. (Default)
///									SEGGER_RTT_MODE_NO_BLOCK_TRIM     	// Trim: Do not block, output as much as fits.
///									SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL	// Block: Wait until there is space in the buffer.
///									SEGGER_RTT_MODE_MASK
/// @return
/// @attention	无
/// @see		无
/// @todo
/////////////////////////////////////////////////////////////////////

S32 SeggerRttConfigUpBuffer(U32 BufferIndex, const char* sName, void* pBuffer, U32 BufferSize, U32 Flags);

/////////////////////////////////////////////////////////////////////
/// @brief		配置Segger RTT Down通道参数
/// @author       qizhilong
/// @version      v1.00
/// @date         2018-04-14
/// @param [in]  BufferIndex       :通道编号
/// @param [in]  sName      		:通道名称（字符串）
/// @param [in]  pBuffer       	:缓冲区起始地址指针
/// @param [in]  BufferSize       	:缓冲区大小
/// @param [in]  Flags       		:配置标志
/// 								SEGGER_RTT_MODE_NO_BLOCK_SKIP    	// Skip. Do not block, output nothing. (Default)
///									SEGGER_RTT_MODE_NO_BLOCK_TRIM     	// Trim: Do not block, output as much as fits.
///									SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL	// Block: Wait until there is space in the buffer.
///									SEGGER_RTT_MODE_MASK
/// @return
/// @attention	无
/// @see		无
/// @todo
/////////////////////////////////////////////////////////////////////

S32 SeggerRttConfigDownBuffer(U32 BufferIndex, const char* sName, void* pBuffer, U32 BufferSize, U32 Flags);

#endif
