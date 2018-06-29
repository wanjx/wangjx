// TranversServerDLL.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "TranversServerDllApi.h"
#include "configParser.h"
#include "TranversWorkSession.h"
#include "TranversServerPreProcessor.h"

extern GMLock            g_TranversPreProcLock;
extern bool              g_TranversPreProcCanWork;

TranversServerPreProcessor g_TranversPreProcessor;
ServerOuterAddrCallBack    g_ServerInfoCallBack = NULL;
GMLock                   g_TranversServerStartLock;
bool                     g_TranversServerCanStart = false;
GMLock                   g_TranversServerStopLock;
bool                     g_TranversServerCanStop = false;
CConfigParser             g_config;

TRANVERS_SERVER_API_EXPORT int  Start()
{
	if(g_TranversServerCanStart)
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器已经开启");
		return 0;
	}
	{
		GMAutoLock<GMLock>  al(&g_TranversServerStartLock);
		g_TranversServerCanStart = true;
	}
	{
		GMAutoLock<GMLock>  al(&g_TranversServerStopLock);
		g_TranversServerCanStop = true;
	}
	//设置Log参数
	Log::open(true,"-dGMfyds",false);
	//初始化异步模型
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int cpuNum = sysinfo.dwNumberOfProcessors;
	AsynModel::AsynResultRet ret;
	if (1 == cpuNum)
	{
		ret = InitAsynModel(cpuNum*2);
	}
	else
	{
		ret = InitAsynModel();
	}
	if(ret != AsynModel::SUCCESS)
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器异步模型初始化失败,ret:%d",ret);
		return -1;
	}
	else
	{
		Log::writeWarning(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器异步模型初始化成功");
	}
	//启动异步模型
	ret = StartAsynModel();
	if(ret != AsynModel::SUCCESS)
	{
		ReleaseAsynModel();
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器异步模型启动失败:ret:%d",ret);
		return false;
	}
	else
	{
		Log::writeWarning(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器异步模型启动成功");
	}
	//异步模型添加监听(解析配置文件)
	if(!g_config.ParseConfigFile())
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器解析配置文件失败");
		Sleep(5*1000);
		return -1;
	}
	if(0 != SetDefaultLocalUdpAddr((char*)g_config.GetTranversServerInfo().ip.c_str(), g_config.GetTranversServerInfo().port))
	{
		StopAsynModel();
		ReleaseAsynModel();
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器设置默认的UDP本地监控端口失败");
		return -1;
	}
	else
	{
		Log::writeWarning(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器设置默认的UDP本地监控端口成功");
	}
	//创建worksession
	TranversWorkSession * pSession = new(nothrow) TranversWorkSession;
	if(pSession == NULL)
	{
		StopAsynModel();
		ReleaseAsynModel();
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器new TranversWorkSession失败");
		return -1;
	}
	//开启session，start()
	pSession->Start();
	//获取sessionId
	// = pSession->GetSessionID();
	//注册预处理器
	if(!RegistIOPreProcessor(&g_TranversPreProcessor))//注册预处理器,process 外部维护
	{
		StopAsynModel();
		ReleaseAsynModel();
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器注册预处理器失败");
		return -1;
	}
	//setworksession
	g_TranversPreProcessor.SetWorkSession(pSession);
	{
		GMAutoLock<GMLock> al(&g_TranversPreProcLock);
		g_TranversPreProcCanWork = true;
	}
	return 0;
}

TRANVERS_SERVER_API_EXPORT void  Stop()
{
	if(!g_TranversServerCanStop)
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "打洞服务器已经停止或还未开启");
		return;
	}
	Log::writeWarning(TRANVERSSERVER_LOG,LOG_TYPE_ID,"进入打洞服务器Stop()");
	StopAsynModel();
	ReleaseAsynModel();
	{
		GMAutoLock<GMLock>  al(&g_TranversServerStartLock);
		g_TranversServerCanStart = false;
	}
	{
		GMAutoLock<GMLock>  al(&g_TranversServerStopLock);
		g_TranversServerCanStop = false;
	}
	{
		GMAutoLock<GMLock> al(&g_TranversPreProcLock);
		g_TranversPreProcCanWork = false;
	}

	return ;
}

TRANVERS_SERVER_API_EXPORT void  RegStpRCOuterAddrCallBack(ServerOuterAddrCallBack pCallBack)
{
	g_ServerInfoCallBack = pCallBack;
}