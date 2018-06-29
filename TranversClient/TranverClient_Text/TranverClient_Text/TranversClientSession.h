#pragma once
#include "Session.h"
#include "TranversCmd.h"


#define   INNER_QUERY_LOCAL_OUTER_ADDR_REQ_CMD     2201
#define   INNER_QUERY_LOCAL_OUTER_ADDR_RESP_CMD    2202

#define   INNER_ADD_NEW_TRANVERS_NODE_REQ_CMD      2203

enum ON_TCP_UDP {UDP=1,TCP,TCP_AND_UDP};

struct Server
{
	char           m_ip[16];
	unsigned short m_port;
};

//struct AddNewTranversNode
//{
//	Server  m_serverAddr;
//};

struct TranversParam
{
	Server            m_tranversServerAddr;
	int               m_dominId;
	std::string       m_serverType;

	std::string       m_userId;
	std::list<Server> m_stpRcList;
};
class TranversClientSession : public AsynModel::Session
{
	BEGIN_MSG_MAP(TranversClientSession)		
		MSG_HANDLER(TRANVERS_CLIENT_RESP_CMD_S,	  onTranversClientSRespCmdArrived);         
		MSG_HANDLER(TRANVERS_CLIENT_RESP_CMD_C,   onTranversClientCRespCmdArrived);
		MSG_HANDLER(TRANVERS_SERVER_NOTIFY_CLINET_CMD,	  onTranversServerNotifyClientCmdArrived);
		MSG_HANDLER(TRANVERS_CLIENT_TO_CLINET_RESP_CMD,	  onTranversClientToClientRespCmdArrived);
		            
		MSG_HANDLER(INNER_QUERY_LOCAL_OUTER_ADDR_REQ_CMD,	  onInnerQueryOuterAddrReqCmdArrived);

		MSG_HANDLER(INNER_ADD_NEW_TRANVERS_NODE_REQ_CMD,	  onInnerAddNewTanversNodeReqCmdArrived);

		MSG_HANDLER(TRANVERS_CLIENT_TO_CLINET_REQ_CMD,	  onTranversClientToClientReqCmdArrived);
	END_MSG_MAP()

	BEGIN_TIMER_MAP(TranversClientSession)
		TIMER_HANDLER(1,RepeatSendCmdTimer);
	END_TIMER_MAP()

	int	  OnStart();
	void  OnTerminate(U32 type);


	TranversClientSession(const TranversParam & param,int type,ON_TCP_UDP on_type);

	int  onTranversClientSRespCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);
	int  onTranversClientCRespCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);
	int  onTranversServerNotifyClientCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);
	int  onTranversClientToClientRespCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);
	int  onTranversClientToClientReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);
	int  onInnerAddNewTanversNodeReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);
	
	int  onInnerQueryOuterAddrReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender);

	int  RepeatSendCmdTimer(GMCustomTimerCallBackType onTimerType, AsynModel::UserDataInfo* pUserTimeDataInfo);

	//订阅回执
	 //   int OnPostMsgRecept(AsynModel::ReceptType receptType,
		//AsynModel::ReceptResult receptResult,
		//U32 msgId,
		//const char *data, 
		//U32 len, 
		//U32 destSid,
		//AsynModel::Session::ContactType contactType, 
		//AsynModel::ConnID& connid,
		//const AsynModel::SptrISender& sender,
		//AsynModel::UserDataInfo* userPara,
		//U32 localIP,
		//U16 localPort,
		//bool isReliable);

	TranversParam  m_param;
	int            m_TranversClientType; //1业务服务器，2业务终端
	ON_TCP_UDP     m_on_type;
	//该client实例的outerIP和port
	Server         m_OuterAddr;

	//std::string    m_userId;

	std::list<Server>  m_TranversNodeList;

	IPPORT_SID  m_TranversNodemap;

	    unsigned long m_localIP ;
		unsigned short m_localPort ;

public:

};