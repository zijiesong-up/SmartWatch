////////////////////////////////////////////////////////////////////
///@mainpage The DebugLog Module
///
///@par 模块功能
///-# 该日志模块将日志信息输出到指定的日志缓冲区中，该缓冲区为环形缓冲区形式，提供读取日志缓冲区信息的接口；
///-# 提供断言，用于主动检查代码缺陷；
///-# 添加SEGGER_RTT模块，支持Cortex 内核的日志打印功能；
///
///@par 所含文件
///-# DebugConfig.h
///-# DebugLog.h
///-# DebugType.h
///-# PrintService.c
///-# PrintService.h
///
///@par 注意事项
///-# 日志单条打印的最大长度为255字节；
///-# F32类型数据,即float类型的数据，日志无法打印；
///-# 八进制和十六进制的数据格式，日志模块以十六进制形式存放在日志缓冲区里；
///-# 为了方便的通过日志查找问题，可以根据各自系统资源，增加一个计数器。
/// 比如使用一个计数定时器，将该定时器的计数添加的日志中为用户显示相对时间，
/// 或者在系统中断维护一个全局计数变量，用户可根据需要自行增加。
///-# 日志等级的作用是减少代码量和运行时间，将日志分模块的意义是减少编译代码空间。
/// 将对应模块的日志设置为DEBUG_LOG_LEVEL_NULL时，则对应模块的日志将不占用代码空间。
///-# 在仿真环境下，可以尝试使用控制台，调用的编译器printf，但是不同编译器对printf支持可能会存在问题。
/// 比如KEIL平台下，因其会由于<stdio.h>导致程序跑飞，不可使用控制台。在CCS环境下，可以使用控制台。
///
///@par 所需资源
///-# 支持ARM系列编译器，可以运行在KEIL、CCS、XILINX、ADSP++等编译环境；
///-# 需要用户提供日志缓冲区地址、大小。根据经验一般设置在32K~4096K之间，实际大小以用户空间为准；
///-# 该日志模块将日志信息输出到指定的日志缓冲区中，该缓冲区为环形缓冲区形式，提供读取日志缓冲区信息的接口。
/// 调用者需要自行将该数据通过USB/串口等接口返回给上位机分析，或者存储到FLASH等器件中。
///
///@par 相对上一版本更改点
///-# 根据环形缓冲区模块进行更新。
///-# 新增获取当前已经记录的日志大小功能
///-# 新增获取日志当前缓冲区读写指针功能
///
///@par 相对上一版本更改点  -V1.0.2
///-# 添加SEGGER_RTT模块，支持Cortex 内核的日志打印功能；
///-# 优化日志模块组织形式，DebugLog.h文件包含所有可外部调用的函数，DebugConfig.h文件包含所有可配置的宏；
///
///@par 相对上一版本更改点  -V1.0.4
///-# 删除日志模块中的SEGGER RTT代码，引用SEGGER RTT 模块
///
///@par 相对上一版本更改点  -V1.0.5
///-# 增加debuglog输出回调函数，可通过DebugLogPrintfCallbackSet()挂接函数，实现日志加密等功能;
///
///@par 相对上一版本更改点  -V1.0.6
///-# DEBUG_LOG_ERR\DEBUG_LOG_INFO\DEBUG_LOG_TRACE输出时DEBUG_LOG_PRINTF调用3次，改为调用2次，提高日志输出效率。
///
///@par 相对上一版本更改点  -V1.0.7
///-# 数据类型引用StdSnbc.h
///
///@par 相对上一版本更改点  -V1.0.8
///-# 解决多线程下日志被打断错行问题
///
///@par 相对上一版本更改点  -V1.0.9
///-# 解决RtVsnprintf返回长度大于日志缓冲区大小导致的溢出的问题
//
///@par Copyright (c)2016, 山东新北洋信息技术股份有限公司
///@par All rights reserved.
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
/// Copyright (c)2016, 山东新北洋信息技术股份有限公司
/// All rights reserved.
/// @file         PrintService.c
/// @brief      实现调试信息格式化功能,实现printf,移植自rtt
/// @author   xuchunkai
/// @version  V1.0.1
/// @date       2016-11-1
////////////////////////////////////////////////////////////////////
//#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "PrintService.h"
#include "RingBufferModule.h"

#if defined (_TEST_DEBUG_OUTPUT_TYPE_WITH_RAM)

struct RINGBUFF DebugLogBufCtrl;
static U8 sDebugLogFullMode = DEBUG_LOG_FULL_REWRITE;
static U32 DebugLogDataBufPut(void *pData, const U32 Length);

#elif defined (_TEST_DEBUG_OUTPUT_TYPE_WITH_SEGGER)

#include "SeggerRtt.h"
#define SEGGER_RTT_LOG_CHANNEL			0

#elif defined (_TEST_DEBUG_OUTPUT_TYPE_WITH_CONSOL)
    //do nothing
#else
    #error "Debug output defined error!"
#endif

U8 gDebugLogGlobalLevel = DEBUG_LOG_LEVEL_NULL;

pVoidFun pDebugLogDI = NULL;
pVoidFun pDebugLogEI = NULL;
pU32Fun pfDebugLogTime = NULL;

///
///@ingroup Module_Info
///
static const U8  sModuleVersion[] = "MV1.000.10";///< 模块版本号，修订模块后升级版本号

static U32 RtStrlen(const char *s);
static U32 DebugLogTimeDefaultFun(void);

static void (*sDebugLogPrintfCallback)(U8* DataBuffer, U32 DataLength);
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
/// @todo         注意及时更新版本号
/////////////////////////////////////////////////////////////////////
void DebugLogGetVersion(U8 *const VersionBuf, U32 *const VersionLen)
{
	*VersionLen = strlen((char *)sModuleVersion);  // 计算数组长度

	memcpy(VersionBuf,sModuleVersion,*VersionLen);

	return;
}

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
U32 DebugLogInit(U8 *pLogBufStart, U32 LogBufSize, U32 LogBufFullReserveSize, U8 DebugLogFullMode
				 ,pVoidFun pDisableInt, pVoidFun pEnableInt, pU32Fun pDebugLogTime)
{
	if (NULL == pLogBufStart
		|| 0 == LogBufSize
		|| 0 == LogBufFullReserveSize
		|| (LogBufFullReserveSize*2) >= LogBufSize
		|| DebugLogFullMode > DEBUG_LOG_FULL_STOP)
	{
		return 1;
	}

	sDebugLogPrintfCallback = NULL;

#if defined (_TEST_DEBUG_OUTPUT_TYPE_WITH_RAM)
    sDebugLogFullMode = DebugLogFullMode;
	if(0 != RingBuffModuleInit(&DebugLogBufCtrl,pLogBufStart,LogBufSize,LogBufFullReserveSize,LogBufFullReserveSize*2))
	{
		return 1;
	}
#elif defined (_TEST_DEBUG_OUTPUT_TYPE_WITH_SEGGER)
	SeggerRttInit();
	if(SeggerRttConfigUpBuffer(SEGGER_RTT_LOG_CHANNEL, "SeggerLog", pLogBufStart, LogBufSize, SEGGER_RTT_MODE_NO_BLOCK_TRIM) < 0)
	{
		return 1;
	}
#endif

	if (NULL != pDisableInt)
	{
		pDebugLogDI = pDisableInt;
	}

	if (NULL != pEnableInt)
	{
		pDebugLogEI = pEnableInt;
	}

	if(NULL == pDebugLogTime)
	{
		pfDebugLogTime = DebugLogTimeDefaultFun;
	}
	else
	{
		pfDebugLogTime = pDebugLogTime;
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////
/// @brief          日志输出回调函数设置
/// @author       zhangkaifa
/// @version      v1.01
/// @date         2019/2/15
/// @param [in]   void (*CallbackFunction):需要挂接的函数
/// @return
/// @attention      无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
void DebugLogPrintfCallbackSet(void (*CallbackFunction)(U8* DataBuffer, U32 DataLength))
{
    sDebugLogPrintfCallback = CallbackFunction;
}
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
void DebugLogGlobalLevelSet(U8 Level)
{
	if(Level > DEBUG_LOG_LEVEL_TRACE)
	{
		return;
	}

	gDebugLogGlobalLevel = Level;
}

#ifdef _TEST_DEBUG_OUTPUT_TYPE_WITH_RAM
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
U32 DebugLogGetUsedSize(U32 *pSize)
{
	if(pSize != NULL)
	{
		*pSize = GetUsedDataSize(&DebugLogBufCtrl);
		return 0;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////
/// @brief        获取日志当前缓冲区读写指针
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
U32 DebugLogGetDataPt(U8 *pReadPt, U8 *pWritePt)
{
	if(pReadPt != NULL
	  && pWritePt != NULL)
	{
		pReadPt = (U8*)DebugLogBufCtrl.mRead;
		pWritePt = (U8*)DebugLogBufCtrl.mWrite;
		return 0;
	}
	return 1;
}
#endif
/////////////////////////////////////////////////////////////////////
/// @brief          日志默认时间函数
/// @author       xuchunkai
/// @version      v1.01
/// @date           2016/11/21
/// @param [in]  void
/// @return        时间计数值
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
static U32 DebugLogTimeDefaultFun(void)
{
	static U32 DebugLogTimeCounter = 0;
	return (DebugLogTimeCounter++);
}

/////////////////////////////////////////////////////////////////////
/// @brief          两个内存区域的比较
/// @author
/// @version      v1.01
/// @date           2016/11/21
/// @param [in]  cs        :内存区域1
/// @param [in]  ct         :内存区域2
/// @param [in]  count   : 内存区域的大小
/// @return    比较结果  0：相等，非0：不相等
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
S32 RtMemcmp(const void *cs, const void *ct, U32 count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

/////////////////////////////////////////////////////////////////////
/// @brief          返回字符串首次出现的位置
/// @author
/// @version      v1.01
/// @date           2016/11/21
/// @param [in]  s1    :源字符串
/// @param [in]  s2    :待查找的字符串
/// @return                返回s2在s1首次出现的位置，或者NULL :未找到
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
char *RtStrStr(const char *s1, const char *s2)
{
	U32 l1, l2;

	l2 = RtStrlen(s2);
	if (!l2)
		return (char *)s1;
	l1 = RtStrlen(s1);
	while (l1 >= l2)
	{
		l1 --;
		if (!RtMemcmp(s1, s2, l2))
			return (char *)s1;
		s1 ++;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////
/// @brief      返回字符串的长度
/// @author
/// @version      v1.01
/// @date           2016/11/21
/// @param [in]   s   :字符串
/// @return              字符串的长度
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
static U32 RtStrlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		;

	return (U32)(sc - s);
}

// private function
#define IsDigit(c)  ((unsigned)((c) - '0') < 10)

/////////////////////////////////////////////////////////////////////
/// @brief      	按照十进制或十六进制形式拆分数据的最低位
/// @author
/// @version      v1.01
/// @date           2016/11/21
/// @param [in]   n       :数值
/// @param [in]   base  :进制
/// @return              返回数据的最低位
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
S32 Divide(S32 *n, S32 base)
{
	S32 res;

	// optimized for processor which does not support Divide instructions.
	if (base == 10)
	{
		res = ((U32)*n) % 10U;
		*n = ((U32)*n) / 10U;
	}
	else
	{
		res = ((U32)*n) % 16U;
		*n = ((U32)*n) / 16U;
	}

	return res;
}

/////////////////////////////////////////////////////////////////////
/// @brief      	字符类型的数字转换为整数类型数字
/// @author
/// @version      v1.01
/// @date           2016/11/21
/// @param [in]   s       :字符串
/// @return              返回字符对应的数字
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
int SkipAtoi(const char **s)
{
	register int i=0;
	while (IsDigit(**s)) i = i*10 + *((*s)++) - '0';

	return i;
}

#define ZEROPAD     (1 << 0)	//* pad with zero */
#define SIGN        (1 << 1)	//* unsigned/signed long */
#define PLUS        (1 << 2)	//* show plus */
#define SPACE       (1 << 3)	//* space if plus */
#define LEFT        (1 << 4)	        //* left justified */
#define SPECIAL     (1 << 5)	//* 0x */
#define LARGE       (1 << 6)	//* use 'ABCDEF' instead of 'abcdef' */

/////////////////////////////////////////////////////////////////////
/// @brief      打印数字
/// @author
/// @version      v1.01
/// @date           2016/11/21
/// @param [in]   buf            :数组首地址
/// @param [in]   end   	   :数组结束地址
/// @param [in]   num          :数据
/// @param [in]   base         :进制类型
/// @param [in]   s              :数据的宽度
/// @param [in]   precision   : 数字精度
/// @param [in]   type         :大、小写类型
/// @return    返回数字的字符串形式
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
static char *PrintNumber(char *buf, char *end, S32 num, S32 base, S32 s, S32 precision, S32 type)
{
	char c, sign;
	char tmp[16];
	const char *digits;
	static const char small_digits[] = "0123456789abcdef";
	static const char large_digits[] = "0123456789ABCDEF";
	S32 i;
	S32 size;

	size = s;

	digits = (type & LARGE) ? large_digits : small_digits;
	if (type & LEFT) type &= ~ZEROPAD;

	c = (type & ZEROPAD) ? '0' : ' ';

	// get sign
	sign = 0;
	if (type & SIGN)
	{
		if (num < 0)
		{
			sign = '-';
			num = -num;
		}
		else if (type & PLUS) sign = '+';
		else if (type & SPACE) sign = ' ';
	}


	i = 0;
	if (num == 0) tmp[i++]='0';
	else
	{
		while (num != 0) tmp[i++] = digits[Divide(&num, base)];
	}

	if (i > precision) precision = i;
	size -= precision;

	if (!(type&(ZEROPAD | LEFT)))
	{
		while (size-->0)
		{
			if (buf <= end) *buf = ' ';
			++ buf;
		}
	}

	if (sign)
	{
		if (buf <= end)
		{
			*buf = sign;
			-- size;
		}
		++ buf;
	}

	//* no align to the left */
	if (!(type & LEFT))
	{
		while (size-- > 0)
		{
			if (buf <= end) *buf = c;
			++ buf;
		}
	}

	while (i < precision--)
	{
		if (buf <= end) *buf = '0';
		++ buf;
	}

	//* put number in the temporary buffer */
	while (i-- > 0)
	{
		if (buf <= end) *buf = tmp[i];
		++ buf;
	}

	while (size-- > 0)
	{
		if (buf <= end) *buf = ' ';
		++ buf;
	}

	return buf;
}

/////////////////////////////////////////////////////////////////////
/// @brief           字符串内容格式化
/// @author
/// @version      v1.01
/// @date           2016/11/21
/// @param [in]   buf            :存放字符串的数组
/// @param [in]   size   	   :存放字符串数组的大小
/// @param [in]   fmt           :字符串的格式
/// @param [in]   args          :va_list类型变量
/// @return    字符串的长度
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
static S32 RtVsnprintf(char *buf, U32 size, const char *fmt, va_list args)
{
	U32 num;
	int i, len;
	char *str, *end, c;
	const char *s;

	U8 base;			//* the base of number */
	U8 flags;			//* flags to print number */
	U8 qualifier;		//* 'h', 'l', or 'L' for integer fields */
	S32 field_width;	//* width of output field */

	int precision;		//* min. # of digits for integers and max for a string */

	str = buf;
	end = buf + size - 1;

	//* Make sure end is always >= buf */
	if (end < buf)
	{
		end = ((char *)-1);
		size = end - buf;
	}

	for (; *fmt ; ++fmt)
	{
		if (*fmt != '%')
		{
			if (str <= end) *str = *fmt;
			++ str;
			continue;
		}

		//* process flags */
		flags = 0;

		while (1)
		{
			//* skips the first '%' also */
			++ fmt;
			if (*fmt == '-') flags |= LEFT;
			else if (*fmt == '+') flags |= PLUS;
			else if (*fmt == ' ') flags |= SPACE;
			else if (*fmt == '#') flags |= SPECIAL;
			else if (*fmt == '0') flags |= ZEROPAD;
			else break;
		}

		//* get field width */
		field_width = -1;
		if (IsDigit(*fmt)) field_width = SkipAtoi(&fmt);
		else if (*fmt == '*')
		{
			++ fmt;
			//* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0)
			{
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		//* get the precision */
		precision = -1;
		if (*fmt == '.')
		{
			++ fmt;
			if (IsDigit(*fmt)) precision = SkipAtoi(&fmt);
			else if (*fmt == '*')
			{
				++ fmt;
				//* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0) precision = 0;
		}
		//* get the conversion qualifier */
		qualifier = 0;
		if (*fmt == 'h' || *fmt == 'l')
		{
			qualifier = *fmt;
			++ fmt;
		}

		//* the default base */
		base = 10;

		switch (*fmt)
		{
		case 'c':
			if (!(flags & LEFT))
			{
				while (--field_width > 0)
				{
					if (str <= end) *str = ' ';
					++ str;
				}
			}

			// get character
			c = (U8)va_arg(args, int);
			if (str <= end) *str = c;
			++ str;

			// put width
			while (--field_width > 0)
			{
				if (str <= end) *str = ' ';
				++ str;
			}
			continue;

		case 's':
			s = va_arg(args, char *);
			if (!s) s = "(NULL)";

			len = RtStrlen(s);
			if (precision > 0 && len > precision) len = precision;

			if (!(flags & LEFT))
			{
				while (len < field_width--)
				{
					if (str <= end) *str = ' ';
					++ str;
				}
			}

			for (i = 0; i < len; ++i)
			{
				if (str <= end) *str = *s;
				++ str;
				++ s;
			}

			while (len < field_width--)
			{
				if (str <= end) *str = ' ';
				++ str;
			}
			continue;

		case 'p':
			if (field_width == -1)
			{
				field_width = sizeof(void *) << 1;
				flags |= ZEROPAD;
			}
			str = PrintNumber(str, end,
							   (long)va_arg(args, void *),
							   16, field_width, precision, flags);
			continue;

		case '%':
			if (str <= end) *str = '%';
			++ str;
			continue;

			// integer number formats - set up the flags and "break"
		case 'o':
			base = 8;
			break;

		case 'X':
			flags |= LARGE;
		case 'x':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			break;

		default:
			if (str <= end) *str = '%';
			++ str;

			if (*fmt)
			{
				if (str <= end) *str = *fmt;
				++ str;
			}
			else
			{
				-- fmt;
			}
			continue;
		}

		if (qualifier == 'l')
		{
			num = va_arg(args, U32);
			if (flags & SIGN) num = (S32)num;
		}
		else if (qualifier == 'h')
		{
			num = (U16)va_arg(args, S32);
			if (flags & SIGN) num = (S16)num;
		}
		else
		{
			num = va_arg(args, U32);
			if (flags & SIGN) num = (S32)num;
		}
		str = PrintNumber(str, end, num, base, field_width, precision, flags);
	}

	if (str <= end) *str = '\0';
	else *end = '\0';

	// the trailing null byte doesn't count towards the total
	// ++str;
	return str-buf;
}

//S32 RtVsprintf(char *buf, const char *format, va_list arg_ptr)
//{
//	return RtVsnprintf(buf, (U32) -1, format, arg_ptr);
//}

// This function will fill a formatted string to buffer

// @param buf the buffer to save formatted string
// @param size the size of buffer
// @param fmt the format

// S32 RtSnprintf(char *buf, U32 size, const char *fmt, ...)
// {
	// S32 n;
	// va_list args;

	// va_start(args, fmt);
	// n = RtVsnprintf(buf, size, fmt, args);
	// va_end(args);

	// return n;
// }


//
 //  This function will fill a formatted string to buffer
 //
 //  @param buf the buffer to save formatted string
 //  @param format the format

// S32 RtSprintf(char *buf, const char *format, ...)
// {
	// S32 n;
	// va_list arg_ptr;

	// va_start(arg_ptr, format);
	// n = RtVsprintf(buf ,format, arg_ptr);
	// va_end(arg_ptr);

	// return n;
// }
//

#ifdef _TEST_DEBUG_PRINT
/////////////////////////////////////////////////////////////////////
/// @brief           日志格式化输出到缓冲区
/// @author        xuchunkai
/// @version       v1.00
/// @date           2014-3-17
/// @param [in]   fmt   :日志的参数列表
/// @return
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
void DEBUG_LOG_PRINTF(const char *fmt, ...)
{
	va_list args;
	U32 length;
	static char rt_log_buf[DEBUG_CONSOLEBUF_SIZE];

	va_start(args, fmt);

	length = RtVsnprintf(rt_log_buf, sizeof(rt_log_buf), fmt, args);

	if(length > DEBUG_CONSOLEBUF_SIZE)
    {
        length = DEBUG_CONSOLEBUF_SIZE;
    }

	if(sDebugLogPrintfCallback != NULL)
	{
		sDebugLogPrintfCallback((U8 *)rt_log_buf,length);
	}

#if defined (_TEST_DEBUG_OUTPUT_TYPE_WITH_RAM)
	DebugLogDataBufPut(&rt_log_buf[0],length);
#elif defined (_TEST_DEBUG_OUTPUT_TYPE_WITH_SEGGER)
    SeggerRttWrite(SEGGER_RTT_LOG_CHANNEL, rt_log_buf, length);
#elif defined (_TEST_DEBUG_OUTPUT_TYPE_WITH_CONSOL)
	printf(rt_log_buf);
#endif

	va_end(args);
}
#else
void DEBUG_LOG_PRINTF(const char *fmt, ...)
{
}
#endif

#ifdef _TEST_DEBUG_PRINT
/////////////////////////////////////////////////////////////////////
/// @brief           直接数据输出
/// @author        xuchunkai
/// @version       v1.00
/// @date           2014-3-26
/// @param [in]   Buf   :源地址
/// @param [in]   Len   :数据长度
/// @return
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
void DEBUG_DATA_PRINTF(U8 *Buf,U32 Len)
{
#ifdef _TEST_DEBUG_OUTPUT_TYPE_WITH_RAM
	if(0 != WriteBuffMovePtr(&DebugLogBufCtrl,Buf,Len))
	{
		;
	}
#endif
}
#else
void DEBUG_DATA_PRINTF(U8 *Buf,U32 Len)
{
}
#endif

#ifdef _TEST_DEBUG_OUTPUT_TYPE_WITH_RAM
/////////////////////////////////////////////////////////////////////
/// @brief           调试数据置入
/// @author        xuchunkai
/// @version       v1.00
/// @date            2016-11-6
/// @param [in]   pData   :源地址
/// @param [in]   Length   :数据长度
/// @return     1：源数据长度超标，2：新数据丢失，其他：写入数据的长度
/// @attention    无
/// @note           无
/// @see            无
/////////////////////////////////////////////////////////////////////
static U32 DebugLogDataBufPut(void *pData, const U32 Length)
{
	U8 *pIn = (U8*)pData;
	U32 Len = Length;
	if (Len >= DEBUG_CONSOLEBUF_SIZE)
	{
		return 1;
	}

	if (0 == Len)
	{
		while (0 != *(pIn + Len)
			   && (Len < DEBUG_CONSOLEBUF_SIZE))
		{
			Len++;
		}
		Len = Len + 1;
	}

	if (Len >= GetFreeDataSize(&DebugLogBufCtrl))
	{
		U32 ClearSize = 0;
		if (DEBUG_LOG_FULL_STOP == sDebugLogFullMode)
		{
			return 2;
		}
		else
		{
			ClearSize = GetUsedDataSize(&DebugLogBufCtrl);
			ClearSize = STD_MINVAR(ClearSize,DEBUG_CONSOLEBUF_SIZE);
			if( 0 != ReadPtrUpdateByNum(&DebugLogBufCtrl, ClearSize))
			{
				;
			}
		}
	}

	return WriteBuffMovePtr(&DebugLogBufCtrl,pIn,Len);
}

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
U32 DebugLogDataBufOut(U8 *pbuf, U32 ReadLength, U32 *RealLength)
{
	U32 Length = 0;
	if (NULL == pbuf
		|| 0 == ReadLength)
	{
		return 1;
	}

	Length = GetUsedDataSize(&DebugLogBufCtrl);
	if (0 == Length)
	{
		*RealLength = 0;
		return 1;
	}

	*RealLength = STD_MINVAR(Length,ReadLength);
	*RealLength = ReadBuffMovePtr(&DebugLogBufCtrl,pbuf,*RealLength);
	return 0;
}

#ifdef Test1
U8 LogBuf[100] = {0};
U8 LogBuf2[300] = {0};

U32 DebugLogDataBufPut1(void *pData, const U32 Length)
{
	return DebugLogDataBufPut(pData, Length);
}

U8 DebugLogDataBufPutOtherTest1(void)
{
	U32 index = 0;
	U8  data = 0x33;
	U32 Length = sizeof(LogBuf);
	U8 ret = 0;

	memset(LogBuf,0x34,Length);
	for(index=0; index < 4000; index++)
	{
		DebugLogDataBufPut(&data,1);
	}

	sDebugLogFullMode = DEBUG_LOG_FULL_STOP;
	ret = DebugLogDataBufPut(LogBuf,100);

	return ret;
}

U8 DebugLogDataBufPutOtherTest2(void)
{
	U32 index = 0;
	U8  data = 0x33;
	U32 Length = sizeof(LogBuf);
	U8 ret = 0;

	memset(LogBuf,0x34,Length);
	for(index=0; index < 4000; index++)
	{
		DebugLogDataBufPut(&data,1);
	}

	sDebugLogFullMode = DEBUG_LOG_FULL_REWRITE;
	ret = DebugLogDataBufPut(LogBuf,100);

	return ret;
}

U32 RtVsnprintfTest(const char *fmt, ...)
{
	va_list args1;
	U32 length;
	char LogBuf[100] = {0};

	va_start(args1, fmt);
	length = RtVsnprintf(LogBuf,0,fmt, args1);
	va_end(args1);
	return length;
}
#endif
#endif
