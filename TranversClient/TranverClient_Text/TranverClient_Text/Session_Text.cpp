#include "stdafx.h"
#include "Session_Text.h"

#define  SEND_TIME_SPAN  (1*1000)
int	  ClientSession::OnStart()
{

	SetTimer(1,0);
	return 0;
}
void  ClientSession::OnTerminate(U32 type)
{

}

ClientSession::ClientSession()
{
	m_insterPort=4786;
	memcpy(m_insterIP,"10.160.71.49",16);
	Server pServer;
	pServer.m_port=111;
	memcpy(pServer.m_ip,"999999",16);
	PostMsg(1,INNER_ADD_NEW_TRANVERS_NODE_REQ_CMD,(char*)&pServer,sizeof(Server));
}

int  ClientSession::RepeatSendCmdTimer(GMCustomTimerCallBackType onTimerType, AsynModel::UserDataInfo* pUserTimeDataInfo)
{
	
		AsynModel::ConnID connID;
		connID.hosttype=AsynModel::ConnID::CONNID_OPEN_;
		memcpy(connID.openhost.openUDPIP,m_insterIP,16);
		connID.openhost.openUdpPort=m_insterPort;

		AsynModel::AsynResultRet asynRet = AsynModel::SUCCESS;


		asynRet=PostMsg(connID,2,TEXT_CLIENT_REQ,0,0,TRUE);
		if (asynRet== AsynModel::FATAL_ERROR)
		{
			cout<<"发送消息失败："<<endl;
		}
	    cout<<"发送消息成功："<<endl;
	    SetTimer(1,SEND_TIME_SPAN);

	return 0;
}
int  ClientSession::OnGetInterest(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{




	return 0;
}
int ClientSession:: OnGetMsg(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	cout<<"===================jieshou====================="<<endl;
	return 0;
}