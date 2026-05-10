/// Copyright (c)2016, 山东新北洋信息技术股份有限公司
/// All rights reserved.
/// @file        DebugMacro.h
/// @brief       日志记录相关的宏开关头文件,使用时将此文件拷贝到用户工程的根目录(或头文件包含的其他指定的目录)
/// @version     V1.0.0
/// @author      xuchunkai
/// @date        2016-11-10
////////////////////////////////////////////////////////////////////

#ifndef __DEBUGMACRO_H__
#define __DEBUGMACRO_H__
#include "DebugType.h"

#define _TEST_DEBUG//调试模式使能总开关

#ifdef _TEST_DEBUG//调试子模块

#define LOG_HEAD ""
/*调试信息返回模块使能*/
#define _TEST_DEBUG_PRINT
#define DEBUG_CONSOLEBUF_SIZE	512//单个字符串的最大长度

/*日志是否输入到RAM中(环形缓冲区)*/
//#define _TEST_DEBUG_OUTPUT_TYPE_WITH_RAM

/*日志是否输入到Segger RTT,real time terminal,需要连接jlink,只支持ARM ContexM系列单片机*/
#define _TEST_DEBUG_OUTPUT_TYPE_WITH_SEGGER

/*日志是否输出到控制台,需要仿真环境支持printf*/
//#define _TEST_DEBUG_OUTPUT_TYPE_WITH_CONSOL

/*日志信息返回模块使能,调试信息返回使能时有效*/
#ifdef _TEST_DEBUG_PRINT
#define _TEST_DEBUG_LOG
#define _TEST_DEBUG_ASSERT
#endif

///*ASSERT功能,调试信息返回使能时有效*/
//#ifdef _TEST_DEBUG_PRINT
//#define _TEST_DEBUG_ASSERT
//#endif

/*日志信息返回子模块,日志信息返回模块使能时有效
子模块定义为1时进行记录,定义为0时不记录;*/
#ifdef _TEST_DEBUG_LOG

/*使用运行态调整日志级别,运行代码运行时重新设置日志级别,注意开启此选项后所有模块日志将被编译到代码空间中,同时有可能
会影响代码运行时间，代码空间足够的可以使用*/
#define _TEST_USE_RUNTIME_LEVEL

///
///@defgroup Module_LevelSet 用户可用的日志级别
///@{
///@par 用户根据实际情况可以设置不同的日志级别
///
#define DEBUG_LOG_LEVEL_NULL       0
#define DEBUG_LOG_LEVEL_ERR        1
#define DEBUG_LOG_LEVEL_INFO	   2//1全开时设置1
#define DEBUG_LOG_LEVEL_TRACE	   3//1全开时设置1
///
///@}
///

extern U8 gDebugLogGlobalLevel;

#define DEBUG_LOG_GLOBAL_LEVEL(x) STD_MAXVAR(x,gDebugLogGlobalLevel)
///#define DEBUG_LOG_GLOBAL_LEVEL(x) x


#if defined (_TEST_DEBUG_OUTPUT_TYPE_WITH_RAM)

#ifndef DEBUG_LOG_FLOW
#define DEBUG_LOG_FLOW			   	DEBUG_LOG_LEVEL_INFO
#endif

#ifndef DEBUG_LOG_GRID
#define DEBUG_LOG_GRID			   	DEBUG_LOG_LEVEL_INFO
#endif

#ifndef DEBUG_LOG_UPINSTR
#define DEBUG_LOG_UPINSTR		   DEBUG_LOG_LEVEL_TRACE
#endif

#ifndef DEBUG_LOG_DOWNINSTR
#define DEBUG_LOG_DOWNINSTR		   DEBUG_LOG_LEVEL_TRACE
#endif

#ifndef DEBUG_LOG_LOG
#define DEBUG_LOG_LOG			   DEBUG_LOG_LEVEL_INFO
#endif

#else

#ifndef DEBUG_LOG_FLOW
#define DEBUG_LOG_FLOW			   	DEBUG_LOG_LEVEL_TRACE
#endif

#ifndef DEBUG_LOG_GRID
#define DEBUG_LOG_GRID			   	DEBUG_LOG_LEVEL_INFO
#endif

#ifndef DEBUG_LOG_UPINSTR
#define DEBUG_LOG_UPINSTR		   DEBUG_LOG_LEVEL_TRACE
#endif

#ifndef DEBUG_LOG_DOWNINSTR
#define DEBUG_LOG_DOWNINSTR		   DEBUG_LOG_LEVEL_TRACE
#endif

#ifndef DEBUG_LOG_LOG
#define DEBUG_LOG_LOG			   DEBUG_LOG_LEVEL_TRACE
#endif
    
#endif


#endif


#endif///调试子模块

#endif
