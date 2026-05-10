////////////////////////////////////////////////////////////////////
///@mainpage The Ring Buffer Module 
///
///@par 模块功能
///提供环形缓冲区基本操作函数
///
///@par 所含文件
///-# RingBufferModule.c
///-# RingBufferModule.h
///-# RingBuffTypeDef.h
///
///@par 注意事项
///-# 该模块主要应用于通讯接口缓冲区
///-# CheckRingBuffFull必须在写操作之后调用，其他情况可能得到相反结果
///-# CheckRingBuffFree必须在读缓冲区后，缓冲区满的情况下调用。否则可能得到相反结果
///-# 所提供的函数均为基本操作，应用时需考虑多线程等情况
///
///@par 组成模块
///-# 版本
///-# 初始化
///-# 状态检测
///-# 写操作
///-# 读操作
///	
///@par 所需资源
///-# 代码量:小于2k
///-# 缓冲区大小:自定义
///
///@par 相对上一版本更改点
///-# 保留16字节的空间，使缓冲区永远不会被写满，避免GetFreeDataSize函数误判
///-# 开放GetFreeDataSize、GetUsedDataSize函数
///-# 增加CheckRingBuffState函数，该函数不操作满标志
///-# 增加CheckRingBuffFull、CheckRingBuffFree、CheckRingBuffState应用环境说明
///-# 头文件结尾增加回车
///
///@par 环形缓冲区示意图
///@image html test.png 
///
///@par Copyright (c)2016, 山东新北洋信息技术股份有限公司
///@par All rights reserved.
////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////
/// Copyright (c)2016, 山东新北洋信息技术股份有限公司
/// All rights reserved.
/// @file         ringbuffermodule.c
/// @brief      环形缓冲区模块基本操作	
/// @author   arlen	
/// @version  V1.0.0
/// @date       2016-11-1
////////////////////////////////////////////////////////////////////

#include <string.h>
#include "ringbufftypedef.h"
#include "ringbuffermodule.h"


#define  FORBID_SPACE   16 ///< 缓冲区模块始终会有至少16个字节的空间，保证缓冲区永远不会被写满

///
///@ingroup Module_Info
///
static const char  sModuleVersion[] = "MV1.000.03";

/////////////////////////////////////////////////////////////////////
/// @brief		获取模块版本号
/// @author        arlen
/// @version      v1.00
/// @date           2016-11-1
/// @param [in]  VersionBuf       :缓冲区结构体
/// @param [in]  VersionLen       :缓冲区起始地址指针
/// @return        void 
/// @attention	无
/// @see		无
/// @todo         注意及时更新版本号
/////////////////////////////////////////////////////////////////////
void GetRingBuffModuleVersion(U8 *const VersionBuf, U32 *const VersionLen)
{
	*VersionLen = strlen((char *)sModuleVersion);  // 计算数组长度
	
	memcpy(VersionBuf,sModuleVersion,*VersionLen);
	
	return;
}

/////////////////////////////////////////////////////////////////////
/// @brief          环形缓冲区初始化函数
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-1
/// @param [in]  Buff               :缓冲区结构体
/// @param [in]  Start              :缓冲区起始地址指针
/// @param [in]  MaxSize         :缓冲区所占区域字节数
/// @param [in]  FreetofullNum :缓冲区满时预留字节数
/// @param [in]  FulltofreeNum :缓冲区由满状态变为非满状态时空闲字节数
/// @return        S32 :
///					  -1:初始化失败
///					  0 :初始化成功
/// @attention    无
/// @note           无
/// @see            无  
/////////////////////////////////////////////////////////////////////
S32 RingBuffModuleInit(struct RINGBUFF *Buff, U8 *Start, const U32 MaxSize, const U32 FreetofullNum, const U32 FulltofreeNum)
{
	if ((Start == 0) || (MaxSize == 0)
		|| (FreetofullNum == 0) || (FulltofreeNum == 0)
		|| (FreetofullNum >= FulltofreeNum)
		|| (FreetofullNum >= MaxSize) || (FulltofreeNum >= MaxSize))
	{
		return (-1);
	}

	Buff->mStart = Start;
	Buff->mEnd = Start + MaxSize;
	Buff->mWrite = Start;
	Buff->mRead = Buff->mWrite;
	Buff->mBuffFull = 0;
	Buff->mSize = MaxSize;
	Buff->mFreetofullnum = FreetofullNum;
	Buff->mFulltofreenum = FulltofreeNum;

	return 0;
}

/////////////////////////////////////////////////////////////////////
/// @brief           获取环形缓冲区剩余空间大小
/// @author        arlen
/// @version       v1.00
/// @date            2016-11-1
/// @param [in]   Buff 
/// @return         U32 :空闲用缓冲区大小,单位为字节,最大值不超过缓冲区最大容量
/// @attention     该函数作为模块内部函数，不能为外部函数调用
/// @note            无
/// @see             无
/// @ingroup Module_Status_Check
/////////////////////////////////////////////////////////////////////
U32 GetFreeDataSize(const struct RINGBUFF * Buff)
{
	S32 Size = 0;

	/*lint -e838*/
	Size = (S32)(Buff->mWrite - Buff->mRead);  
	/*lint +e838*/
	
	if ( Size < 0)                            
	{
		Size += (S32)Buff->mSize;
	}

	Size = (S32)(Buff->mSize - (U32)Size);
	if((U32)Size < FORBID_SPACE)	//加上禁写的几个字节
	{
		return 0;
	}
	else
	{
		return (U32)((U32)Size - FORBID_SPACE);
	}
}

/////////////////////////////////////////////////////////////////////
/// @brief          获取环形缓冲区已使用空间大小
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-1
/// @param [in]  Buff
/// @return        U32 :已使用缓冲区大小,单位为字节,最大值不超过缓冲区最大容量
/// @attention    无
/// @note            无
/// @see             无
/// @ingroup     Module_Status_Check
/////////////////////////////////////////////////////////////////////
U32 GetUsedDataSize(const struct RINGBUFF *Buff)
{
	S32 Size = 0;
	/*lint -e838*/
	Size = (S32)(Buff->mWrite - Buff->mRead);
	/*lint +e838*/
	if ( Size  <  0)                     
	{
		Size += (S32)Buff->mSize;
	}

	return((U32)Size);
}

/////////////////////////////////////////////////////////////////////
/// @brief          查询缓冲区是否已写满（剩余空间小于mFreetofullnum）
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-5
/// @param [in]  Buff
/// @return        U32 Buff->mBuffFul
/// @attention    该函数必须在写操作之后调用，其他地方调用有可能得到相反的结果
/// @note            无
/// @see             无
/// @exception  无	 
/////////////////////////////////////////////////////////////////////
U32 CheckRingBuffFull(struct RINGBUFF *Buff)
{
	if (GetFreeDataSize(Buff) <=  Buff->mFreetofullnum)
	{
		Buff->mBuffFull = 1;  
	}

	return Buff->mBuffFull;
}

/////////////////////////////////////////////////////////////////////
/// @brief        查询缓冲区是否已有足够空间（剩余空间大于mFulltofreenum）
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-5
/// @param [in]  Buff
/// @return        U32 !Buff->mBuffFul
/// @attention   该函数必须在缓冲区已满时，在读操作之后调用。其他情况调用有可能得到相反的结果
/// @note           无
/// @see           无
/////////////////////////////////////////////////////////////////////
U32 CheckRingBuffFree(struct RINGBUFF *Buff)
{
	if(GetFreeDataSize(Buff) >  Buff->mFulltofreenum)
	{
		Buff->mBuffFull = 0;  
	}
	
	return (!Buff->mBuffFull);
}

/////////////////////////////////////////////////////////////////////
/// @brief          查询缓冲区状态(不置满标志)
/// @author       arlen
/// @version      v1.00
/// @date           2017-3-10
/// @param [in]  Buff
/// @return        U32 
///				1:满
/// 				0:不满
/// @attention   直接返回满标志
/// @note           该函数不操作满标志mBuffFull
/// @see           无
/////////////////////////////////////////////////////////////////////
U32 CheckRingBuffState(const struct RINGBUFF *Buff)
{
	return Buff->mBuffFull;
}

/////////////////////////////////////////////////////////////////////
/// @brief          向缓冲区中写入数据且移动写指针
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-5
/// @param [in]  Buff
/// @param [in]  Src             源地址
/// @param [in]  DataNum     需要写入的字节个数
/// @return        U32 返回已写入字节数
/// @attention   在写缓冲区前，先查询剩余空间是否足够。
///                     如果足够就执行写操作，返回值为已写入的字节数；
///                     否则直接退出写函数 ，返回值为0。             
/// @note         
/// @see         
/////////////////////////////////////////////////////////////////////
U32 WriteBuffMovePtr(struct RINGBUFF *Buff, const U8 *Src, const U32 DataNum)
{
	U8 *pWrite = Buff->mWrite;
	U32 Count = DataNum;
	
	if (GetFreeDataSize(Buff) < Count)   //数据超界，退出写函数
	{
		return 0;  
	}
	
	while (Count > 0)   
	{
		*pWrite = *Src; 
		Src++;
		pWrite++;
		if ( pWrite >= Buff->mEnd)
		{
			pWrite = Buff->mStart;
		}
		Count--;
	}
	Buff->mWrite = pWrite;

	return DataNum;
}
/////////////////////////////////////////////////////////////////////
/// @brief          向缓冲区中写入数据但不移动写指针
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-5
/// @param [in]  Buff
/// @param [in]  Src             源地址
/// @param [in]  DataNum     需要写入的字节个数 
/// @return        U32 返回已写入字节数
/// @attention   如果剩余空间不足，按剩余空间大小写入，
///                     写入数据后不移动写指针
/// @note          无
/// @see            无
/////////////////////////////////////////////////////////////////////
U32 WriteBuffNotMovePtr(const struct RINGBUFF *Buff,const  U8 *Src, U32 DataNum)
{
	U8 *pWrite = Buff->mWrite;
	U32 Count = 0;

	if(GetFreeDataSize(Buff) < DataNum) 	
	{
		DataNum = GetFreeDataSize(Buff);
	}

	while(Count < DataNum)
	{
		*pWrite = *Src; 
		Src++;
		pWrite++;
		if ( pWrite >= Buff->mEnd)
		{
			pWrite = Buff->mStart;
		}
		Count++;
	}
    //数据写入后不移动写指针

	return DataNum;  
}
/////////////////////////////////////////////////////////////////////
/// @brief          向缓冲区中写入数据,如果剩余空间不足,按剩余空间大小写入
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-5
/// @param [in]  Buff
/// @param [in]  Src           源地址
/// @param [in]  DataNum   需要写入的字节个数
/// @return        U32 返回已写入字节数
/// @attention   写入后移动指针
/// @note          无
/// @see           无
/////////////////////////////////////////////////////////////////////
U32 WriteBuffAccordToFreeSize(struct RINGBUFF *Buff,const U8 *Src, U32 DataNum)
{
	U8 *pWrite = Buff->mWrite;
	U32 Count = 0;

	if(GetFreeDataSize(Buff) < DataNum) 	  //获取空闲空间大小
	{
		DataNum = GetFreeDataSize(Buff);   //若剩余空间小于需要写入数据量，按剩余空间写
	}

	while(Count < DataNum)
	{
		*pWrite = *Src; 
		Src++;
		pWrite++;
		if ( pWrite >= Buff->mEnd)
		{
			pWrite = Buff->mStart;
		}
		Count++;
	}
	Buff->mWrite = pWrite;

	return DataNum;   //返回实际写入的字节数
}

/**
 * @param		调用memcpy()按照CPU字长对环形缓冲区进行快速拷贝写入
 * @author  	lwg
 * @date		2020-12-03
 * @param		Buff	环形缓冲指针(目的地址)
 * @param		Src		源地址
 * @param		DataNum	需写入数据长度
 * @return		返回环形缓冲写指针地址
 * @attention	调用该接口函数的前提是已确定缓冲剩余空间不小于需写入数据的长度
 * @note		当单次拷贝数据比较多时,调用memcpy()按照CPU字长进行拷贝,可有效提高效率(单次拷贝200个字节时,在K64上实测效率可提高7倍);
 *				当单次拷贝数据比较少时,memcpy()函数调用的开销会大于数据拷贝所节省的开销;
 */
static inline U8* RingBuffWriteByMemcpy(const struct RINGBUFF *Buff, const U8 *Src, const U32 DataNum)
{
	U8 *pWrite = Buff->mWrite;
	U32 lBufLen = (U32)(Buff->mEnd) - (U32)(pWrite);	/*获取写指针到缓冲区结束地址的连续长度*/
	
	if(lBufLen > DataNum)
	{
		memcpy(pWrite, Src, DataNum);					/*拷贝数据比较多时,按照CPU字长进行拷贝,可有效提高效率*/
		pWrite += DataNum;
	}
	else
	{
		memcpy(pWrite, Src, lBufLen);
		if(lBufLen < DataNum)
		{
			memcpy(Buff->mStart, &Src[lBufLen], DataNum - lBufLen);
			pWrite = Buff->mStart + (DataNum - lBufLen);
		}
		else
		{
			pWrite = Buff->mStart;
		}
	}
	
	return pWrite;
}

/**
 * @param		使用memcpy()向缓冲区中写入数据且移动写指针
 * @author  	lwg
 * @date		2020-12-17
 * @param		Buff	环形缓冲指针(目的地址)
 * @param		Src		源地址
 * @param		DataNum	需写入数据长度
 * @return		返回环形缓冲写指针地址
 * @attention	在写缓冲区前，先查询剩余空间是否足够。
 *				如果足够就执行写操作，返回值为已写入的字节数；
 *				否则直接退出写函数 ，返回值为0。
 * @note		
 */
U32 WriteBuffByMemcpyMovePtr(struct RINGBUFF *Buff, const U8 *Src, const U32 DataNum)
{
	if (GetFreeDataSize(Buff) < DataNum)   //数据超界，退出写函数
	{
		return 0;  
	}

	Buff->mWrite = RingBuffWriteByMemcpy(Buff, Src, DataNum);

	return DataNum;
}

/**
 * @param		使用memcpy()向缓冲区中写入数据但不移动写指针
 * @author  	lwg
 * @date		2020-12-17
 * @param		Buff	环形缓冲指针(目的地址)
 * @param		Src		源地址
 * @param		DataNum	需要写入的字节个数 
 * @return		U32 返回已写入字节数
 * @attention	如果剩余空间不足，按剩余空间大小写入，写入数据后不移动写指针
 * @note		
 */
U32 WriteBuffByMemcpyNotMovePtr(const struct RINGBUFF *Buff,const  U8 *Src, U32 DataNum)
{
	U32 TempLen = GetFreeDataSize(Buff);
	
	if(TempLen == 0)
	{
		return 0;
	}
	else if(TempLen < DataNum) 	
	{
		DataNum = TempLen;
	}

	RingBuffWriteByMemcpy(Buff, Src, DataNum);
	//数据写入后不移动写指针

	return DataNum;  
}

/**
 * @param		使用memcpy()向缓冲区中写入数据并移动写指针
 * @author  	lwg
 * @date		2020-12-17
 * @param		Buff	环形缓冲指针(目的地址)
 * @param		Src		源地址
 * @param		DataNum	需要写入的字节个数 
 * @return		U32 返回已写入字节数
 * @attention	如果剩余空间不足，按剩余空间大小写入，写入数据后移动写指针
 * @note		
 */
U32 WriteBuffByMemcpyAccordToFreeSize(struct RINGBUFF *Buff,const U8 *Src, U32 DataNum)
{
	U32 TempLen = GetFreeDataSize(Buff);
	
	if(TempLen == 0)
	{
		return 0;
	}
	else if(TempLen < DataNum) 	
	{
		DataNum = TempLen;
	}

	Buff->mWrite = RingBuffWriteByMemcpy(Buff, Src, DataNum);

	return DataNum;   //返回实际写入的字节数
}

/////////////////////////////////////////////////////////////////////
/// @brief          通过数据个数更新环形缓冲区写指针
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-6
/// @param [in]  Buff 
/// @param [in]  DataNum    指针偏移量
/// @return        S32  
///                          -1:更新失败
///                           0:更新成功
/// @attention   无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
S32 WritePtrUpdateByNum(struct RINGBUFF *Buff, const U32 DataNum)
{
	U8 *pWrite = Buff->mWrite;
	//参数异常，直接退出
	if(DataNum > Buff->mSize)   
	{
		return (-1);   
	}

	pWrite += DataNum;
	if ( pWrite >= Buff->mEnd )
	{
		pWrite -= Buff->mSize;
	}
	Buff->mWrite = pWrite;

	return 0;
}

/////////////////////////////////////////////////////////////////////
/// @brief          通过数指针更新环形缓冲区写指针
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-6
/// @param [in]  Buff 
/// @param [in]  Ptr  指定的更新后的指针
/// @return        S32  
///                          -1:更新失败
///                           0:更新成功
/// @attention   无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
S32 WritePtrUpdateByPtr(struct RINGBUFF *Buff, U8 *const Ptr)
{
	//参数异常，直接退出
	if((Ptr >= Buff->mEnd) || (Ptr < Buff->mStart) || (Ptr == NULL))
	{
		return (-1);   
	}

	Buff->mWrite = Ptr;
	return 0;
}
/////////////////////////////////////////////////////////////////////
/// @brief          读缓冲区且移动读指针
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-5
/// @param [in]  Buff
/// @param [in]  Dst            目的地址
/// @param [in]  DataNum   需要读取的字节个数
/// @return        U32 返回值为已读取的字节数
/// @attention   如果现有数据量小于需要读取的字节数，不执行读操作
/// @note          无
/// @see            无
/////////////////////////////////////////////////////////////////////
U32 ReadBuffMovePtr(struct RINGBUFF *Buff, U8 *Dst, const U32 DataNum)
{
	U8 *pRead = Buff->mRead;
	U32 Count = DataNum;
	
	if (GetUsedDataSize(Buff) < Count)  //数据超界，退出读函数
	{
		return 0;
	}

	while (Count > 0)  
	{
		*Dst = *pRead; 
		Dst++; 
		pRead++;
		if ( pRead >= Buff->mEnd)
		{
			pRead = Buff->mStart;
		}
		Count--;
	}
	Buff->mRead = pRead;

	return DataNum;
}

/////////////////////////////////////////////////////////////////////
/// @brief          读缓冲但不移动读指针
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-5
/// @param [in]  Buff
/// @param [in]  Dst              目的地址
/// @param [in]  DataNum     需要读取的字节个数
/// @return        U32 返回值为已读取的字节数
/// @attention   如果现有数据量小于需要读取的字节数，按现有数据量读取，
///                     读取数据后不移动读指针
/// @note          无
/// @see           //lzs重新读前，若被中断打断会存在新长度超过DataNum
/////////////////////////////////////////////////////////////////////
U32 ReadBuffNotMovePtr(const struct RINGBUFF *Buff, U8 *Dst, U32 DataNum)
{
	U8 *pRead = Buff->mRead;
	U32 Count = 0;
	U32 TempLen = GetUsedDataSize(Buff);//LZS
	if (TempLen < DataNum)//GetUsedDataSize(Buff) < DataNum)  //数据超界，按照现有数据量读取
	{
		DataNum = TempLen;//GetUsedDataSize(Buff);  //lzs重新读前，若被中断打断会存在新长度超过DataNum
	}

	while (Count < DataNum)  
	{
		*Dst = *pRead; 
		Dst++; 
		pRead++;
		if ( pRead >= Buff->mEnd)
		{
			pRead = Buff->mStart;
		}
		Count++;
	}
    //读取完成后不移动读指针

	return DataNum;  
}
//////////////////////////////////////////////////////////////////////////
/// @brief         读缓冲区且移动指针,如果现有数据量小于需要读取的字节数,按现有数据量读取
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-5
/// @param [in]  Buff
/// @param [in]  Dst            目的地址
/// @param [in]  DataNum    需要读取的字节个数 
/// @return        U32 返回值为已读取的字节数
/// @attention    读取后移动指针
/// @note           无
/// @see            无
///////////////////////////////////////////////////////////////////////////
U32 ReadBuffAccordToUsedSize(struct RINGBUFF *Buff, U8 *Dst, U32 DataNum)
{
	U8 *pRead = Buff->mRead;
	U32 Count = 0;
	U32 TempLen = GetUsedDataSize(Buff);//LZS
	if (TempLen < DataNum)//	if (GetUsedDataSize(Buff) < DataNum)   //数据超界后，按照现有的数据量读取
	{
		DataNum = TempLen;//GetUsedDataSize(Buff);
	}

	while (Count < DataNum)  
	{
		*Dst = *pRead; 
		Dst++; 
		pRead++;
		if ( pRead >= Buff->mEnd)
		{
			pRead = Buff->mStart;
		}
		Count++;
	}
	Buff->mRead = pRead;

	return DataNum;    //返回实际读取的字节数
}

/**
 * @param		调用memcpy()按照CPU字长对环形缓冲区进行快速拷贝读取
 * @author  	laiweiguang
 * @date		2020-12-03
 * @param		Buff	环形缓冲指针(源地址)
 * @param		Dst		目标地址
 * @param		DataNum	需读取数据长度
 * @return		返回环形缓冲读指针地址
 * @attention	调用该接口函数的前提是已确定缓冲内的有效数据长度不小于需读取数据的长度
 * @note		当单次拷贝数据比较多时,调用memcpy()按照CPU字长进行拷贝,可有效提高效率(单次拷贝200个字节时,在K64上实测效率可提高7倍);
 *				当单次拷贝数据比较少时,memcpy()函数调用的开销会大于数据拷贝所节省的开销;
 */
static inline U8* RingBuffReadByMemcpy(const struct RINGBUFF *Buff, U8 *Dst, const U32 DataNum)
{
	U8 *pRead = Buff->mRead;
	U32 lBufLen = (U32)(Buff->mEnd) - (U32)(pRead);
	
	if(lBufLen > DataNum)
	{
		memcpy(Dst, pRead, DataNum);		/*拷贝数据比较多时,按照CPU字长进行拷贝,可有效提高效率*/
		pRead += DataNum;
	}
	else
	{
		memcpy(Dst, pRead, lBufLen);
		if(lBufLen < DataNum)
		{
			memcpy(&Dst[lBufLen], Buff->mStart, DataNum - lBufLen);
			pRead = Buff->mStart + (DataNum - lBufLen);
		}
		else
		{
			pRead = Buff->mStart;
		}
	}
	
	return pRead;
}

/**
 * @param		调用memcpy()读缓冲区且移动读指针
 * @author  	laiweiguang
 * @date		2020-12-17
 * @param		Buff	环形缓冲指针(源地址)
 * @param		Dst		目标地址
 * @param		DataNum	需要读取的字节个数
 * @return		U32 返回值为已读取的字节数
 * @attention	如果现有数据量小于需要读取的字节数，不执行读操作
 * @note		
 */
U32 ReadBuffByMemcpyMovePtr(struct RINGBUFF *Buff, U8 *Dst, const U32 DataNum)
{
	if (GetUsedDataSize(Buff) < DataNum)  //数据超界，退出读函数
	{
		return 0;
	}

	Buff->mRead = RingBuffReadByMemcpy(Buff, Dst, DataNum);

	return DataNum;
}

/**
 * @param		调用memcpy()读缓冲区但不移动读指针
 * @author  	laiweiguang
 * @date		2020-12-17
 * @param		Buff	环形缓冲指针(源地址)
 * @param		Dst		目标地址
 * @param		DataNum	需要读取的字节个数
 * @return		U32 返回值为已读取的字节数
 * @attention	如果现有数据量小于需要读取的字节数，按现有数据量读取,读取数据后不移动读指针
 * @note		
 */
U32 ReadBuffByMemcpyNotMovePtr(const struct RINGBUFF *Buff, U8 *Dst, U32 DataNum)
{
	U32 TempLen = GetUsedDataSize(Buff);
	if (TempLen == 0)
	{
		return 0;
	}
	else if (TempLen < DataNum)
	{
		DataNum = TempLen;
	}

	RingBuffReadByMemcpy(Buff, Dst, DataNum);
	//读取完成后不移动读指针

	return DataNum;  
}

/**
 * @param		调用memcpy()读缓冲区且移动指针,如果现有数据量小于需要读取的字节数,按现有数据量读取
 * @author  	laiweiguang
 * @date		2020-12-17
 * @param		Buff	环形缓冲指针(源地址)
 * @param		Dst		目标地址
 * @param		DataNum	需要读取的字节个数
 * @return		U32 返回值为已读取的字节数
 * @attention	读取后移动指针
 * @note		
 */
U32 ReadBuffByMemcpyAccordToUsedSize(struct RINGBUFF *Buff, U8 *Dst, U32 DataNum)
{
	U32 TempLen = GetUsedDataSize(Buff);
	if (TempLen == 0)
	{
		return 0;
	}
	else if (TempLen < DataNum)
	{
		DataNum = TempLen;
	}

	Buff->mRead = RingBuffReadByMemcpy(Buff, Dst, DataNum);

	return DataNum;    //返回实际读取的字节数
}

/////////////////////////////////////////////////////////////////////
/// @brief          通过数据量更新环形缓冲区读指针
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-6
/// @param [in]  Buff    
/// @param [in]  DataNum    指针偏移量
/// @return        S32 
///                           -1:更新失败
///                           0:更新成功
/// @attention    无
/// @note           无
/// @see             无
/////////////////////////////////////////////////////////////////////
S32 ReadPtrUpdateByNum(struct RINGBUFF *Buff, const U32 DataNum)
{
	U8 *pRead = Buff->mRead;
	
	if(DataNum > Buff->mSize)
	{
		return (-1);//参数异常，直接退出
	}

	pRead += DataNum;
	if ( pRead >= Buff->mEnd)
	{
		pRead -= Buff->mSize;
	}
	Buff->mRead = pRead;

	return 0;
}
/////////////////////////////////////////////////////////////////////
/// @brief          通过指针更新环形缓冲区读指针
/// @author       arlen
/// @version      v1.00
/// @date           2016-11-6
/// @param [in]  Buff    
/// @param [in]  Ptr   指定的更新后的指针
/// @return        S32 
///                           -1:更新失败
///                           0:更新成功
/// @attention    无
/// @note           无
/// @see             无
/////////////////////////////////////////////////////////////////////
S32 ReadPtrUpdateByPtr(struct RINGBUFF *Buff, U8 *const Ptr)
{
	if((Ptr >= Buff->mEnd) || (Ptr < Buff->mStart) || (Ptr == NULL))
	{
		return (-1);//参数异常，直接退出
	}
	
	Buff->mRead = Ptr;
	return 0;
}
