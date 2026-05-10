# 固件编程规范 V1.14

**编码**：重要重要重要，非常重要，修改代码之前先预览文件的编码，一定不要导致乱码，不要改变代码使用的编码字体，默认使用GB2312，不要导致乱码


## ? 目录

- [一、基础原则](#一基础原则)
- [二、文件结构规范](#二文件结构规范)
- [三、代码版式规范](#三代码版式规范)
- [四、注释规范](#四注释规范)
- [五、命名规范](#五命名规范)
- [六、表达式与语句规范](#六表达式与语句规范)
- [七、函数设计规范](#七函数设计规范)
- [八、特殊编程要求](#八特殊编程要求)
- [九、架构设计模式](#九架构设计模式)
- [十、开发工具与流程](#十开发工具与流程)
- [十一、代码审查检查表](#十一代码审查检查表)

---

## 一、基础原则

### 1.1 核心原则
- **设计思想**：采用面向对象的设计方式
- **统一风格**：所有C/汇编代码采用统一版式风格
- **ANSI C标准**：遵循ANSI C标准，避免编译器扩展功能，确保跨平台兼容性
- **简单直观**：代码应简单易读，先求正确性，再考虑效率
- **减少全局变量**：避免模块间紧耦合，影响函数可重入性，优先使用局部变量和参数传递
- **明确错误码**：所有函数必须有清晰的返回值定义
- **无副作用宏**：避免宏定义中的副作用操作
- **编码**：重要重要重要，非常重要，不改变代码使用的编码字体，默认使用GB2312，不要导致乱码
### 1.2 编程理念

- **最少化全局变量**：优先使用局部变量和参数传递
- **防御性编程**：所有指针必须初始化为NULL，使用前检查
- **可重入性**：减少全局变量和static局部变量
- **性能优先**：循环优化、运算优化、数据驱动
- **可维护性**：清晰注释、统一命名、模块化设计
- **安全性**：互斥锁正确使用、防止死锁

---

## 二、文件结构规范

### 2.1 文件分工

- **头文件（.h）**：存放声明
- **定义文件（.c）**：存放实现
- **汇编文件（.s/.asm）**：汇编代码

### 2.2 版权声明

位于头文件和定义文件开头，包含以下信息：

```c
/**
 * @par      All rights reserved.
 * @file     [文件名].c
 * @brief    [模块功能描述]
 * @note     [特殊说明]
 * @version  [版本号]
 * @author   [作者]
 * @date     [创建日期]
 */
```

### 2.3 头文件保护

```c
#ifndef __MODULE_NAME_H__
#define __MODULE_NAME_H__

// 防止重复包含
#ifdef __cplusplus
extern "C" {
#endif

// 头文件内容

#ifdef __cplusplus
}
#endif

#endif /* __MODULE_NAME_H__ */
```

### 2.4 包含顺序

```c
// 1. 标准C库
#include <stdint.h>
#include <string.h>

// 2. 系统头文件
#include "SystemConfig.h"

// 3. 其他模块头文件
#include "DriverHal.h"

// 4. 本模块头文件
#include "ModuleInternal.h"
```

---

## 三、代码版式规范

### 3.1 空行规则

- 结构体声明、函数定义结束后加空行
- 变量定义说明后加空行
- 逻辑密切语句间不加空行
- 逻辑段落之间加空行

### 3.2 代码行规范

- **一行只做一件事**：只定义一变量或一条语句
- **控制语句独占一行**：`if`、`for`、`while`、`do`等独占一行，执行语句必须加大括号
- **相关代码对齐**：相关代码尽量相邻，相关赋值语句对齐

```c
// 控制语句独占一行，必须使用大括号
if (condition)
{
    // 执行语句
}

// 一行只定义一个变量
U8 index = 0;
U8 count = 10;
```

### 3.3 空格规则

- **关键字后留空格**：`if (condition)`
- **函数名后不留空格**：`Function()`
- **二元操作符前后加空格**：`a = b + c`
- **一元操作符前后不加空格**：`i++`、`*p`

```c
// 正确示例
if (flag)          // 关键字后有空格
{
    Function();    // 函数名后无空格
    a = b + c;     // 二元操作符有空格
    i++;           // 一元操作符无空格
}
```

### 3.4 对齐规范

- **大括号独占一行**：`{`和`}`独占一行并左对齐
- **缩进**：使用TAB缩进（4空格）
- **矩阵对齐**：结构型/多维数组按矩阵结构分行对齐

```c
// 大括号独占一行
void Function(void)
{
    // 使用4空格缩进
    if (condition)
    {
        DoSomething();
    }
}

// 数组初始化对齐
U8 matrix[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
```

### 3.5 长行拆分

- 代码行 ≤ 80字符
- 在低优先级操作符处拆分
- 操作符放新行之首

```c
// 长表达式换行规则
result = LongFunctionName(param1, param2, param3) 
         + AnotherFunction(param4) 
         - CalculateValue(param5);
```

### 3.6 修饰符位置

- `*`和`&`紧靠变量名：`int *x, y;`（注意：y不是指针）
- 避免：`int* x, y;`（易误解y为指针）

```c
// 正确
int *x, y;      // x是指针，y不是指针

// 错误理解
int* x, y;      // 容易误认为y也是指针
```

---

## 四、注释规范

### 4.1 注释量要求

- 源程序有效注释量 ≥ 20%

### 4.2 注释类型

- **函数内部**：`//`
- **函数外部块注释**：`/** ... */`
- **函数外部行注释**：`//`

### 4.3 注释位置

- 放代码**上方**或**右方**
- 不可放**下方**

### 4.4 注释语言

- 使用**中文**
- 英文需流利易懂

### 4.5 函数注释模板

```c
/**
 * @brief    函数功能简要描述
 * @param[in]   param1   参数1说明
 * @param[out]  pResult  输出结果指针
 * @return    错误码：0-成功，其他-错误
 * @note     注意事项和使用限制
 * @see      相关函数
 */
```

### 4.6 变量/常量注释

放在右方，说明含义、功能

```c
// 变量注释
U32 timeoutCount = 0;     ///< 超时计数器，单位：毫秒
U8  deviceStatus = 0;     ///< 设备状态：0-空闲，1-忙碌
```

### 4.7 结构体注释

结构体注释放在上方，成员注释放右方

```c
/// 设备配置结构体
typedef struct
{
    DEV_GPIO_PORT mPort;  ///< GPIO端口号
    DEV_GPIO_PIN  mPin;   ///< GPIO引脚号
    U32           mSpeed; ///< 通信速率，单位：bps
} GPIO_CONFIG;
```

### 4.8 分支语句注释

分支语句必须注释

```c
// 分支注释
if (dataReady)              // 检查数据是否就绪
{
    ProcessData();          // 处理接收到的数据
}
else                        // 数据未就绪情况
{
    WaitForData();          // 等待数据到达
}
```

---

## 五、命名规范

### 5.1 通用规则

- **直观可拼读**：标识符直观可拼读（英文单词组合）
- **最小长度最大信息**：遵循"min-length && max-information"原则
- **禁用数字序号**：禁用数字序号区分相似标识符（除非逻辑需要）
- **禁用大小写区分**：禁用仅大小写区分的标识符

### 5.2 命名规则表

| 类型 | 前缀 | 示例 | 说明 |
|------|------|------|------|
| 变量/参数 | 无 | `VariableName` | 大写字母开头 |
| 全局变量 | `g` | `gSystemStatus` | 避免使用，必须加前缀 |
| 静态变量 | `s` | `sRs232Config` | 模块内部使用 |
| 指针变量 | `p` | `pDataBuffer` | 明确指针类型 |
| 结构体成员 | `m` | `mPort`, `mPin` | 成员标识 |
| 函数参数 | 无 | `Id`, `pConfig` | 小写开头 |
| 宏/常量 | 全大写 | `MAX_BUFFER_SIZE` | 下划线分隔 |
| 预定义开关 | `_`开头 | `_ENABLE_DEBUG` | 配置宏 |
| 函数 | 无 | `FunctionName()` | 大写字母开头 |
| 结构体类型 | 大写开头 | `RS232_CONFIG` | typedef定义 |

### 5.3 缩写原则

- **min-length && max-information**：最短长度包含最大信息
- 缩写规则：去掉元音字母或取前几个字母
- 禁止：`value1`, `value2`（除非逻辑上需要编号）

### 5.4 数据类型规范

使用自定义可移植类型：

```c
U8   // 无符号8位
U16  // 无符号16位
U32  // 无符号32位
S8   // 有符号8位
S16  // 有符号16位
S32  // 有符号32位
F32  // 单精度浮点
F64  // 双精度浮点
```

### 5.5 结构体定义规范

- 必须使用`typedef`
- 字段从大到小定义：32位/指针 → 16位 → 8位
- 显式字节对齐：`ALIGN(4)`
- 手动填充使字节数为4倍数

```c
// 字段从大到小排列（32位→16位→8位）
typedef struct
{
    U32   mBaseAddress;     // 32位在前
    U16   mDeviceId;        // 16位在中
    U8    mChannel;         // 8位在后
    U8    mReserved;        // 对齐填充
} DEVICE_CONFIG ALIGN(4);   // 显式对齐

// 指针立即初始化为NULL
U8 *pBuffer = STD_NULL;
DEVICE_HANDLE hDevice = STD_NULL;
```

---

## 六、表达式与语句规范

### 6.1 运算优化

#### 6.1.1 使用移位代替乘除

```c
// 使用移位代替乘除（性能优化）
value = value << 3;      // 替代 value * 8
value = value >> 2;      // 替代 value / 4
```

#### 6.1.2 浮点除法优化

```c
// 浮点除法优化（计算倒数）
result = a * (1.0f / b);  // 替代 a / b
```

### 6.2 比较规则

```c
// 布尔变量
if (flag)           // 正确
if (!flag)          // 正确

// 整型变量
if (value == 0)     // 正确

// 浮点变量
if ((x >= -EPSINON) && (x <= EPSINON))  // 正确

// 指针变量
if (p == NULL)      // 正确
if (p != NULL)      // 正确
```

### 6.3 循环优化

#### 6.3.1 循环嵌套优化

外层短循环，内层长循环（减少跳转）

```c
// 外层短循环，内层长循环
for (i = 0; i < 10; i++)         // 外层：10次
{
    for (j = 0; j < 1000; j++)   // 内层：1000次
    {
        Process(i, j);
    }
}
```

#### 6.3.2 逻辑判断外移

逻辑判断移到循环体外（减少判断次数）

```c
// 逻辑判断外移
if (needProcessing)              // 只判断一次
{
    for (i = 0; i < count; i++)
    {
        ProcessData(data[i]);    // 循环内无判断
    }
}
```

### 6.4 控制语句规范

#### 6.4.1 if语句

- if语句尽量加else分支
- 必须使用大括号

```c
if (condition)
{
    // 执行语句
}
else
{
    // 默认处理
}
```

#### 6.4.2 for循环

采用半开半闭区间：`for (x=0; x<N; x++)`

```c
for (i = 0; i < count; i++)  // 半开半闭区间
{
    Process(data[i]);
}
```

#### 6.4.3 switch语句

- 必须有`default`分支
- 每个case必须有`break`

```c
switch (command)
{
    case CMD_START:
        StartProcess();
        break;
        
    case CMD_STOP:
        StopProcess();
        break;
        
    default:                     // 必须包含default分支
        LogError("Unknown command");
        break;
}
```

#### 6.4.4 多分支处理

多分支时考虑数据驱动方式

#### 6.4.5 goto语句

慎用goto语句，仅用于资源清理

```c
void ProcessWithResources(void)
{
    RESOURCE *pRes1 = STD_NULL;
    RESOURCE *pRes2 = STD_NULL;
    
    // 申请资源
    pRes1 = AllocateResource1();
    if (pRes1 == STD_NULL) goto cleanup;
    
    pRes2 = AllocateResource2();
    if (pRes2 == STD_NULL) goto cleanup;
    
    // 使用资源...
    
cleanup:
    // 逆序释放（后申请的先释放）
    if (pRes2 != STD_NULL) FreeResource2(pRes2);
    if (pRes1 != STD_NULL) FreeResource1(pRes1);
}
```

---

## 七、函数设计规范

### 7.1 基本原则

- **功能单一**：函数功能单一，规模小（≤ 50行）
- **避免"记忆"功能**：减少static局部变量
- **输入检查**：检查所有输入有效性（包括全局变量）

### 7.2 参数规范

#### 7.2.1 参数命名

参数命名直观：`strSource`、`strDestination`

#### 7.2.2 参数顺序

参数顺序：源 → 目标 → 配置 → 返回

#### 7.2.3 参数传递

- 结构体参数传递指针
- 输入指针加`const`
- 参数个数 ≤ 4个
- 避免不确定参数

```c
// 参数规范示例
S32 CopyData(const U8 *pSource,    // 源数据（输入，const修饰）
             U8 *pDestination,     // 目标缓冲区（输出）
             U32 size,             // 数据大小
             const CONFIG *pCfg)   // 配置参数（输入，const修饰）
{
    // 实现...
}
```

### 7.3 返回值规范

- **明确返回类型**：无返回用`void`
- **函数名与返回值语义一致**
- **正常值与错误标志分离**
- **正确返回0，异常返回非0**

```c
// 返回值定义
#define SUCCESS             0
#define ERROR_NULL_POINTER  1
#define ERROR_INVALID_SIZE  2
#define ERROR_DEVICE_BUSY   3

S32 FunctionName(void)
{
    // ...
    return SUCCESS;          // 成功返回0
    // return ERROR_CODE;    // 失败返回非0错误码
}
```

### 7.4 内部实现规范

#### 7.4.1 入口检查

入口检查参数有效性（指针非空检查）

```c
S32 FunctionName(U8 *pData, U32 size)
{
    // 1. 指针有效性检查
    if (pData == STD_NULL)
    {
        return ERROR_NULL_POINTER;
    }
    
    // 2. 参数范围检查
    if ((size == 0) || (size > MAX_SIZE))
    {
        return ERROR_INVALID_SIZE;
    }
    
    // 3. 状态检查
    if (!IsDeviceReady())
    {
        return ERROR_DEVICE_BUSY;
    }
    
    // 正常处理流程
    // ...
    
    return SUCCESS;
}
```

#### 7.4.2 出口检查

出口避免返回栈内存指针

```c
// 错误示例
U8* GetLocalBuffer(void)
{
    U8 buffer[100];  // 栈上分配
    // ...
    return buffer;   // 错误：返回栈内存指针
}

// 正确示例
U8* GetBuffer(void)
{
    static U8 sBuffer[100];  // 静态存储
    // ...
    return sBuffer;          // 正确
}
```

#### 7.4.3 互斥锁使用

互斥锁使用避免死锁

---

## 八、特殊编程要求

### 8.1 标志使用

- 特殊功能标志使用后及时清除
- 标志功能单一，避免交叉引用

```c
// 标志使用示例
volatile U8 gDataReadyFlag = 0;

void HandleData(void)
{
    if (gDataReadyFlag)
    {
        ProcessData();
        gDataReadyFlag = 0;  // 使用后立即清除
    }
}
```

### 8.2 数据存储

数据存储地址与宽度严格对应：

- **32位数据**：4字节对齐
- **16位数据**：2字节对齐
- **8位数据**：无要求
- 连续存储时按宽度从大到小

```c
// 结构体对齐
typedef struct
{
    U32 data32;    // 4字节对齐
    U16 data16;    // 2字节对齐
    U8  data8;     // 无对齐要求
    U8  padding;   // 手动填充
} ALIGNED_DATA ALIGN(4);
```

### 8.3 中断编程

- 避免使用延时，如需延时 ≤ 30us
- 长延时使用定时器实现
- 防止中断独占CPU资源

```c
// 中断服务函数示例
void ISR_Handler(void)
{
    // 快速处理，避免长时间占用
    if (DataReady())
    {
        SetFlag();           // 仅设置标志
        // 实际处理放在主循环
    }
}

// 主循环处理
void MainLoop(void)
{
    if (GetFlag())
    {
        ProcessData();       // 在主循环中处理
        ClearFlag();
    }
}
```

---

## 九、架构设计模式

### 9.1 面向对象设计

使用C语言实现面向对象的设计思想

```c
// 设备操作接口（类似虚函数表）
typedef struct
{
    S32 (*Initialize)(void *pDev);
    S32 (*Read)(void *pDev, U8 *pBuffer, U32 size);
    S32 (*Write)(void *pDev, const U8 *pData, U32 size);
    S32 (*Close)(void *pDev);
} DEVICE_OPERATIONS;

// 设备实例结构
typedef struct
{
    DEVICE_ID           mId;
    DEVICE_OPERATIONS   mOps;          // 操作接口
    void               *pPrivateData;  // 私有数据
} DEVICE_INSTANCE;

// 使用示例
S32 UseDevice(DEVICE_INSTANCE *pDev, U8 *pData, U32 size)
{
    if (pDev->mOps.Write != STD_NULL)
    {
        return pDev->mOps.Write(pDev, pData, size);
    }
    return ERROR_NOT_SUPPORTED;
}
```

### 9.2 状态机实现

```c
// 状态定义
typedef enum
{
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_ERROR
} PROCESS_STATE;

// 状态上下文
typedef struct
{
    PROCESS_STATE mCurrentState;
    U32           mStepCounter;
    U8           *pWorkBuffer;
} PROCESS_CONTEXT;

// 状态处理函数
S32 HandleStateMachine(PROCESS_CONTEXT *pCtx)
{
    switch (pCtx->mCurrentState)
    {
        case STATE_IDLE:
            // 空闲状态处理
            if (HasNewTask())
            {
                pCtx->mCurrentState = STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            // 处理状态
            if (ProcessComplete())
            {
                pCtx->mCurrentState = STATE_IDLE;
            }
            break;
            
        case STATE_WAITING:
            // 等待状态
            break;
            
        case STATE_ERROR:
            // 错误状态
            HandleError();
            pCtx->mCurrentState = STATE_IDLE;
            break;
            
        default:
            // 未知状态重置
            pCtx->mCurrentState = STATE_IDLE;
            break;
    }
    
    return SUCCESS;
}
```

### 9.3 数据驱动设计

使用数据驱动方式替代长的switch-case

```c
// 命令映射表（替代长的switch-case）
typedef struct
{
    U8    commandCode;
    char *commandName;
    void (*handler)(void);
} COMMAND_ENTRY;

// 命令表定义
static const COMMAND_ENTRY sCommandTable[] = {
    {0x01, "START", HandleStartCommand},
    {0x02, "STOP",  HandleStopCommand},
    {0x03, "RESET", HandleResetCommand},
    {0x04, "QUERY", HandleQueryCommand},
    // 添加新命令只需在此扩展
};

// 命令查找与执行
void ExecuteCommand(U8 cmdCode)
{
    U32 i;
    U32 tableSize = sizeof(sCommandTable) / sizeof(COMMAND_ENTRY);
    
    for (i = 0; i < tableSize; i++)
    {
        if (sCommandTable[i].commandCode == cmdCode)
        {
            sCommandTable[i].handler();
            return;
        }
    }
    
    // 未找到命令
    HandleUnknownCommand();
}
```

### 9.4 配置与条件编译

#### 9.4.1 硬件配置分离

```c
/**************************************** 硬件配置区域 ****************************************/
#define BSP_RS232_NUM               1       ///< RS232接口数量

static const RS232_CONFIG sRs232Config[] = {
    {
        .mUartId = DRV_UART_ID_8,
        .mRtsPin = {DEV_PORTC, DEV_PIN_16},
        .mCtsPin = {DEV_PORT_INVALID, DEV_PIN_INVALID},
    },
};
/**************************************** 配置结束 ****************************************/
```

#### 9.4.2 条件编译规范

```c
// 调试日志配置
#ifdef _ENABLE_DEBUG_LOG
    #define LOG_DEBUG(fmt, ...)  DEBUG_LOG_PRINTF("[DEBUG] " fmt, ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...)  DEBUG_LOG_PRINTF("[ERROR] " fmt, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(...)
    #define LOG_ERROR(...)
#endif

// 功能模块开关
#if defined(_FEATURE_A) && defined(_FEATURE_B)
    // 同时支持A和B功能的代码
#endif
```

---

## 十、开发工具与流程

### 10.1 开发工具

- **代码编辑**：Visual C++ 2005 Express Edition
- **编译**：打开所有告警和错误开关
- **审查**：使用代码检查工具

### 10.2 流程要求

- 排除所有编译告警和错误
- 代码审查制度化
- 单元测试覆盖关键功能
- 文档与代码同步更新

---

## 十一、代码审查检查表

### ? 必须遵守的规则

1. ? 所有函数必须有返回值定义和错误码
2. ? 指针变量必须立即初始化为`STD_NULL`
3. ? `if/for/while`语句必须使用大括号
4. ? `switch`语句必须有`default`分支
5. ? 函数参数必须进行有效性检查
6. ? 注释遵循Doxygen标准
7. ? 宏定义避免副作用
8. ? 结构体字段按大小顺序排列
9. ? 循环逻辑判断移到外部
10. ? 使用数据驱动替代长的分支语句

### ? 推荐实践

1. ? 优先使用状态机处理复杂逻辑
2. ? 采用面向对象设计思想
3. ? 硬件配置与代码逻辑分离
4. ? 重要代码段添加安全注释

### ? 性能优化要点

1. ? 使用移位代替乘除法
2. ? 优化循环嵌套顺序
3. ? 减少浮点运算
4. ? 合理使用内联函数
5. ? 避免不必要的内存拷贝

### ? 审查检查项

- [ ] 命名规范检查（前缀、大小写）
- [ ] 注释完整性和准确性
- [ ] 错误处理是否完善
- [ ] 资源管理是否正确（申请/释放成对）
- [ ] 性能优化点是否落实
- [ ] 可移植性问题
- [ ] 代码重复度检查
- [ ] 安全漏洞检查（缓冲区溢出等）
- [ ] 指针有效性检查
- [ ] 边界条件处理
- [ ] 测试用例覆盖率
- [ ] 文档同步更新

---

## 附录：常用代码模板

### A.1 函数模板

```c
/**
 * @brief    函数功能描述
 * @param[in]   pInput   输入参数说明
 * @param[out]  pOutput  输出参数说明
 * @return    错误码：0-成功，其他-错误
 * @note     使用注意事项
 */
S32 FunctionTemplate(const U8 *pInput, U8 *pOutput)
{
    // 参数有效性检查
    if (pInput == STD_NULL)
    {
        return ERROR_NULL_POINTER;
    }
    
    if (pOutput == STD_NULL)
    {
        return ERROR_NULL_POINTER;
    }
    
    // 功能实现
    // ...
    
    return SUCCESS;
}
```

### A.2 模块初始化模板

```c
/**
 * @brief    模块初始化
 * @return    错误码：0-成功，其他-错误
 */
S32 ModuleInit(void)
{
    S32 result = SUCCESS;
    
    // 1. 硬件初始化
    result = HardwareInit();
    if (result != SUCCESS)
    {
        return result;
    }
    
    // 2. 变量初始化
    InitializeVariables();
    
    // 3. 注册回调
    RegisterCallbacks();
    
    return SUCCESS;
}
```

### A.3 错误码定义模板

```c
// 错误码定义（模块专用）
#define MODULE_SUCCESS              0   ///< 成功
#define MODULE_ERROR_NULL_POINTER   1   ///< 空指针错误
#define MODULE_ERROR_INVALID_PARAM  2   ///< 无效参数
#define MODULE_ERROR_TIMEOUT        3   ///< 超时错误
#define MODULE_ERROR_BUSY           4   ///< 设备忙
#define MODULE_ERROR_NOT_READY      5   ///< 设备未就绪
```

---

## 总结

本规范旨在建立统一的固件编程标准，确保代码的：

- **可读性**：清晰的命名和注释
- **可维护性**：模块化设计和统一风格
- **可靠性**：完善的错误处理和参数检查
- **高效性**：合理的性能优化
- **可移植性**：遵循ANSI C标准

**遵守本规范是每位固件工程师的职责，也是团队协作的基础。**

---

*最后更新：V1.14*
