#pragma once
#include "Session.h"
#include"TranversClientSession.h"

#define  TEXT_CLIENT_REQ 4371

class ClientSession : public AsynModel::Session
{
	BEGIN_MSG_MAP(ClientSession)	
		MSG_HANDLER(INNER_QUERY_LOCAL_OUTER_ADDR_RESP_CMD,OnGetInterest);
		MSG_HANDLER(TEXT_CLIENT_REQ,OnGetMsg);
	END_MSG_MAP()

	BEGIN_TIMER_MAP(ClientSession)
		TIMER_HANDLER(1,RepeatSendCmdTimer);
	END_TIMER_MAP()

	int	  OnStart();
	void  OnTerminate(U32 type);

	int  OnGetMsg(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);
	int  OnGetInterest(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);
	int  RepeatSendCmdTimer(GMCustomTimerCallBackType onTimerType, AsynModel::UserDataInfo* pUserTimeDataInfo);
	ClientSession();

public:
	char            m_insterIP[16];
	unsigned short   m_insterPort;

};