#pragma once
#include "TranversServerServerData.h"

#ifdef __cplusplus
#	ifdef WIN32
#		ifdef   TRANVERS_SERVER_API_EXPORT
#			define  TRANVERS_SERVER_API_EXPORT  extern "C" __declspec(dllexport)
#		else
#			define  TRANVERS_SERVER_API_EXPORT  extern "C" __declspec(dllimport)
#			pragma comment(lib,"TranversServerDLL.lib")
#		endif
#	else
#		define TRANVERS_SERVER_API_EXPORT extern "C"
#	endif
#else
#	define TRANVERS_SERVER_API_EXPORT
#endif



TRANVERS_SERVER_API_EXPORT int  Start();

TRANVERS_SERVER_API_EXPORT void  Stop();

TRANVERS_SERVER_API_EXPORT void  RegStpRCOuterAddrCallBack(ServerOuterAddrCallBack pCallBack);

