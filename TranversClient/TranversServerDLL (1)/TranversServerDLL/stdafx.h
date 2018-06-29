// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

#include "gmhelperLibPub.h"
#include "AsynModelPublish.h"
#include "QNLog\Log_Remote\LogPub.h"

#define  TRANVERSSERVER_LOG  (1<<20)
#define  LOG_TYPE_ID     (3000)

#define TRANVERS_SERVER_API_EXPORT

// TODO: 在此处引用程序需要的其他头文件
