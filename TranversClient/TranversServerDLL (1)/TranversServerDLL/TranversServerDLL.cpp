// TranversServerDLL.cpp : ���� DLL Ӧ�ó���ĵ���������
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
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷������Ѿ�����");
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
	//����Log����
	Log::open(true,"-dGMfyds",false);
	//��ʼ���첽ģ��
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
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷������첽ģ�ͳ�ʼ��ʧ��,ret:%d",ret);
		return -1;
	}
	else
	{
		Log::writeWarning(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷������첽ģ�ͳ�ʼ���ɹ�");
	}
	//�����첽ģ��
	ret = StartAsynModel();
	if(ret != AsynModel::SUCCESS)
	{
		ReleaseAsynModel();
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷������첽ģ������ʧ��:ret:%d",ret);
		return false;
	}
	else
	{
		Log::writeWarning(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷������첽ģ�������ɹ�");
	}
	//�첽ģ����Ӽ���(���������ļ�)
	if(!g_config.ParseConfigFile())
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷��������������ļ�ʧ��");
		Sleep(5*1000);
		return -1;
	}
	if(0 != SetDefaultLocalUdpAddr((char*)g_config.GetTranversServerInfo().ip.c_str(), g_config.GetTranversServerInfo().port))
	{
		StopAsynModel();
		ReleaseAsynModel();
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷���������Ĭ�ϵ�UDP���ؼ�ض˿�ʧ��");
		return -1;
	}
	else
	{
		Log::writeWarning(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷���������Ĭ�ϵ�UDP���ؼ�ض˿ڳɹ�");
	}
	//����worksession
	TranversWorkSession * pSession = new(nothrow) TranversWorkSession;
	if(pSession == NULL)
	{
		StopAsynModel();
		ReleaseAsynModel();
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷�����new TranversWorkSessionʧ��");
		return -1;
	}
	//����session��start()
	pSession->Start();
	//��ȡsessionId
	// = pSession->GetSessionID();
	//ע��Ԥ������
	if(!RegistIOPreProcessor(&g_TranversPreProcessor))//ע��Ԥ������,process �ⲿά��
	{
		StopAsynModel();
		ReleaseAsynModel();
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷�����ע��Ԥ������ʧ��");
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
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷������Ѿ�ֹͣ��δ����");
		return;
	}
	Log::writeWarning(TRANVERSSERVER_LOG,LOG_TYPE_ID,"����򶴷�����Stop()");
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