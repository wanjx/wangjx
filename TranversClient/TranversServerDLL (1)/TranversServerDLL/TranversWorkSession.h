#pragma once
#include "Session.h"
#include "TranversCmd.h"
#include "TranversServerServerData.h"
#include <algorithm> 
#define TRANVERS_SEND_CMD_TIME_SPAN (1*1000)

extern ServerOuterAddrCallBack g_ServerInfoCallBack;

class TranversWorkSession : public AsynModel::Session
{
	int	  OnStart();
	void  OnTerminate(U32 type);

	BEGIN_MSG_MAP(TranversWorkSession)
		MSG_HANDLER(TRANVERS_CLIENT_REQ_CMD_S,	  onTranverrsClientSReqCmdArrived);
		MSG_HANDLER(TRANVERS_CLIENT_REQ_CMD_C,	  onTranverrsClientCReqCmdArrived);
	END_MSG_MAP()

protected:
	int  onTranverrsClientSReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);
	int  onTranverrsClientCReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);

	//¶©ÔÄ»ØÖ´
	int OnPostMsgRecept(AsynModel::ReceptType receptType,
		AsynModel::ReceptResult receptResult,
		U32 msgId,
		const char *data, 
		U32 len, 
		U32 destSid,
		AsynModel::Session::ContactType contactType, 
		AsynModel::ConnID& connid,
		const AsynModel::SptrISender& sender,
		AsynModel::UserDataInfo* userPara,
		U32 localIP,
		U16 localPort,
		bool isReliable);
public:
	int m_TranversWorkSid;
};