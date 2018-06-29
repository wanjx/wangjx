// TranverClient_Text.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TranversClientSession.h"
#include "Session_Text.h"


int _tmain(int argc, _TCHAR* argv[])
{
	TranversParam param;
	param.m_dominId=0;
	param.m_tranversServerAddr.m_port=20044;
	memcpy(param.m_tranversServerAddr.m_ip,"10.160.71.47",16);
	Server  m_stprc;
	m_stprc.m_port=4786;
	memcpy(m_stprc.m_ip,"10.160.71.49",16);
	param.m_stpRcList.push_back(m_stprc);


	//Log::open(true,"-dGMfyds",false);
	AsynModel::AsynResultRet ret = InitAsynModel();
	if(ret != AsynModel::SUCCESS)
	{
	    cout<<"InitAsynModel::error"<<endl;
		system("pause");
		return -1;

	}
	ret = StartAsynModel();
	if(ret != AsynModel::SUCCESS)
	{
		cout<<"StartAsynModel::error"<<endl;
		system("pause");
		return -1;
	}


	//服务器端使用UDP时需要在启动异步模型后，设置默认的本地utp地址，使用TCP则不需要设置默认的本地tcp地址
	if(0 != SetDefaultLocalUdpAddr("192.168.1.101", 2468))
	{
		StopAsynModel();
		ReleaseAsynModel();
		cout<<"SetDefaultLocalUdpAddr::error"<<endl;
	    system("pause");
		return -1;
	}

	if(0 != SetDefaultLocalTcpAddr("192.168.1.101", 2468))
	{
		StopAsynModel();
		ReleaseAsynModel();
		cout<<"SetDefaultLocalTcpAddr::error"<<endl;
		system("pause");
		return -1;
	}


	TranversClientSession *C_user1=new TranversClientSession( param,2,TCP);
	
    C_user1->Start();

	U32 sirid=C_user1->GetSessionID();
	
	//ClientSession*C_Text=new ClientSession;

	//C_Text->Start();


	//预处理器
   //  TranversClientPreProcessor m_Client_pre;
  //   RegistIOPreProcessor(&m_Client_pre);

	//m_Client_pre.SetWorkSession(C_user1);



	

	while(true)
	{}

	system("pause");
	return 0;
}

