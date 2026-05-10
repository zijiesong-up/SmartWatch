////////////////////////////////////////////////////////////////////
/// Copyright (c)2016, 山东新北洋信息技术股份有限公司
/// All rights reserved.
/// @file         ringbuffermodule.h
/// @brief       环形缓冲区头文件，主要包括向外部提供的全局函数及使用的结构体定义 
/// @version   V1.0.0
/// @author    arlen  
/// @date        2016-11-4
////////////////////////////////////////////////////////////////////

#ifndef _RINGBUFFERMODULE_H
#define _RINGBUFFERMODULE_H
#include "ringbufftypedef.h"

////////////////////////////////////////////////////////////////////
/// @brief		缓冲区结构
/// @note		记录字节型队列缓冲区的相关信息
/// @attention	无
/// @see		RINGBUFF
////////////////////////////////////////////////////////////////////
struct RINGBUFF
{
	/**
	* @brief		缓冲区起始地址指针
	* @note			U8*型数据，指向缓冲区的首个地址
	* @attention	无
	* @see			无
	*/
	U8 *				mStart;				

	/**
	* @brief		缓冲区结束地址指针
	* @note			U8*型数据，指向缓冲区的最后一个地址
	* @attention	无
	* @see			无
	*/
	U8 *				mEnd;					

	/**
	* @brief		缓冲区当前写指针
	* @note			volatile U8	*型数据，指向下一个内容的写入地址
	* @attention	无
	* @see			无
	*/
	U8		*volatile mWrite;				

	/**
	* @brief		缓冲区当前读指针
	* @note			volatile U8	*型数据，指向下一个内容的读取地址
	* @attention	无
	* @see			无
	*/
	U8		*volatile mRead;					

	/**
	* @brief		缓冲区大小
	* @note			U32	型数据，指向下一个内容的读取地址
	* @attention	无
	* @see			无
	*/
	U32				mSize;					

	/**
	* @brief		缓冲区由满状态变为非满状态时空闲字节数
	* @note			U32	型数据，缓冲区由满状态后，变为非满状态需要读取的数据个数
	* @attention	无
	* @see			无
	*/
	U32				mFulltofreenum;		


	/**
	* @brief		缓冲区满时预留字节数
	* @note			U32	型数据，缓冲区由满状态后，缓冲区剩余的最大字节数
	* @attention	无
	* @see			无
	*/
	U32				mFreetofullnum;		

	/**
	* @brief		缓冲区满标志
	* @note			U8型数据，记录缓冲区满状态
	* @attention	- 0:缓冲区非满
	*				- 1:缓冲区满
	* @see			无
	*/
	volatile U8	            mBuffFull;				
};

///
///@defgroup Module_Info 模块信息
///@{
///@par 获取模块版本号
///
void GetRingBuffModuleVersion(U8 *const VersionBuf, U32 *const VersionLen);
///
///@}
///

///
///@defgroup Module_Init 模块初始化
///@{
///@par 初始化模块
///

S32 RingBuffModuleInit(struct RINGBUFF *Buff,U8 * Start, const U32 MaxSize, const U32 FreetofullNum, const U32 FulltofreeNum);

///
///@}
///

///
///@defgroup Module_Status_Check 模块状态检测
///@{
///@par 检测缓冲区的状态，并置相应标志
///
U32 CheckRingBuffFull(struct RINGBUFF *Buff);
U32 CheckRingBuffFree(struct RINGBUFF *Buff);
U32 GetFreeDataSize(const struct RINGBUFF * Buff);
U32 GetUsedDataSize(const struct RINGBUFF *Buff);
U32 CheckRingBuffState(const struct RINGBUFF *Buff);
///	
///@}
///

///@defgroup Module_NormalFunction NormalFunction 
///@{
///@}
///
///@ingroup Module_NormalFunction 
///@defgroup Module_NormalWrite NormalWrite
///@{
///@par 写缓冲区
///
U32 WriteBuffMovePtr(struct RINGBUFF *Buff,const U8 *Src, const U32 DataNum);
U32 WriteBuffByMemcpyMovePtr(struct RINGBUFF *Buff,const U8 *Src, const U32 DataNum);
///	
///@}
///

///
///@defgroup Module_NormalRead NormalRead
///@{
///@ingroup Module_NormalFunction
///@par 读缓冲区
///
U32 ReadBuffMovePtr(struct RINGBUFF *Buff, U8 *Dst, const U32 DataNum);
U32 ReadBuffByMemcpyMovePtr(struct RINGBUFF *Buff, U8 *Dst, const U32 DataNum);
///
///@}
///


///@defgroup Module_SpecialFunction  SpecialFunction
///@{
///@}
///
///
///@ingroup Module_SpecialFunction
///@defgroup Module_SpecialWrite SpecialWrite
///@{
///@par 写缓冲区
///
U32 WriteBuffNotMovePtr(const struct RINGBUFF *Buff,const U8 *Src, U32 DataNum);
U32 WriteBuffAccordToFreeSize(struct RINGBUFF *Buff,const U8 *Src, U32 DataNum);
U32 WriteBuffByMemcpyNotMovePtr(const struct RINGBUFF *Buff,const U8 *Src, U32 DataNum);
U32 WriteBuffByMemcpyAccordToFreeSize(struct RINGBUFF *Buff,const U8 *Src, U32 DataNum);
///
///@}
///

///
///@ingroup Module_SpecialFunction
///@defgroup Module_SpecialRead SpecialRead
///@{
///@par 读缓冲区
///
U32 ReadBuffNotMovePtr(const struct RINGBUFF *Buff, U8 *Dst, U32 DataNum);
U32 ReadBuffAccordToUsedSize(struct RINGBUFF *Buff, U8 *Dst, U32 DataNum);
U32 ReadBuffByMemcpyNotMovePtr(const struct RINGBUFF *Buff, U8 *Dst, U32 DataNum);
U32 ReadBuffByMemcpyAccordToUsedSize(struct RINGBUFF *Buff, U8 *Dst, U32 DataNum);
///
///@}
///


///
///@defgroup Updata_Ptr 更新读写指针
///@{
///@par 更新指针
///
S32 WritePtrUpdateByNum(struct RINGBUFF *Buff, const U32 DataNum);
S32 WritePtrUpdateByPtr(struct RINGBUFF *Buff, U8 *const Ptr);
S32 ReadPtrUpdateByNum(struct RINGBUFF *Buff, const U32 DataNum);
S32 ReadPtrUpdateByPtr(struct RINGBUFF *Buff, U8 *const Ptr);
///
///@}
///

#endif

