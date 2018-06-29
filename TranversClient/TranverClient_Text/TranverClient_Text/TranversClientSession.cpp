#include "stdafx.h"
#include "TranversClientSession.h"

#define  SEND_TIME_SPAN  (1*1000)
int	  TranversClientSession::OnStart()
{
	
	SetTimer(1,0);
	return 0;
}
void  TranversClientSession::OnTerminate(U32 type)
{

}

TranversClientSession::TranversClientSession(const TranversParam & param,int type,ON_TCP_UDP on_type)
{
	m_param.m_dominId=param.m_dominId;
	m_param.m_userId=param.m_userId;
	m_param.m_serverType=param.m_serverType;
	m_param.m_tranversServerAddr.m_port=param.m_tranversServerAddr.m_port;
	memcpy(	m_param.m_tranversServerAddr.m_ip,param.m_tranversServerAddr.m_ip,16);

	m_param.m_stpRcList=param.m_stpRcList;
	m_TranversNodeList = m_param.m_stpRcList;
	std::list<Server>::iterator iter=m_TranversNodeList.begin();

	m_TranversClientType=type;
	if (m_TranversClientType==2)
	{
		m_on_type=on_type;
	} 
	else if(m_TranversClientType==1)
	{
		m_on_type=TCP_AND_UDP;
	}
	else
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[TranversClientSession]参数2传入错误");			
	}
	while (iter!=m_TranversNodeList.end())
	{
		IPPort m_temp;
		m_temp.Port=(*iter).m_port;
		memcpy(	m_temp.Ip,(*iter).m_ip,16);
        m_TranversNodemap[m_temp]=0;
		iter++;
	}
	memcpy(m_OuterAddr.m_ip,"0",16);
	m_OuterAddr.m_port=0;

	m_localIP=inet_addr("192.168.1.101");
	m_localPort=htons(2468);
}

int  TranversClientSession::RepeatSendCmdTimer(GMCustomTimerCallBackType onTimerType, AsynModel::UserDataInfo* pUserTimeDataInfo)
{
	if(m_TranversClientType == 1)
	{
		//封装TranversClientReqCmd_S ，序列化，发送给m_tranversServerAddr 暂时只发udp
		TranversClientReqCmd_S reqCmd_S;
		reqCmd_S.dominId=m_param.m_dominId;
		reqCmd_S.serverType=m_param.m_serverType;

		char pathbuff[1024]={};
		
		if(pathbuff==NULL)
		{
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[RepeatSendCmdTimer]1 内存分配失败");		
			return -1;
		}
		
		int ret=reqCmd_S.Seralize(pathbuff,reqCmd_S.GetRealSize());
		if(ret<0)
		{
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer1 序列化命令失败");
			return -1;
		}

		AsynModel::ConnID connID;
		connID.hosttype=AsynModel::ConnID::CONNID_OPEN_;
		memcpy(connID.openhost.openUDPIP,m_param.m_tranversServerAddr.m_ip,16);
		memcpy(connID.openhost.openTCPIP,m_param.m_tranversServerAddr.m_ip,16);
		connID.openhost.openUdpPort=m_param.m_tranversServerAddr.m_port;
		connID.openhost.openTcpPort=m_param.m_tranversServerAddr.m_port;

		AsynModel::AsynResultRet asynRet = AsynModel::SUCCESS;
		asynRet=PostMsg(connID,0,
			            TRANVERS_CLIENT_REQ_CMD_S,
						pathbuff,reqCmd_S.GetRealSize(),
						TRUE,SENMSGDEFAULTTIMROUT,
						AsynModel::POSTMSG_NO_SUBSCRIBE,
						AsynModel::Protocol::UDP);
		if (asynRet== AsynModel::FATAL_ERROR)
		{
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer 发送UDP消息命令失败");
			
		}
		cout<<"stprc向打洞服务器发送UDP消息命令"<<endl;
		asynRet=PostMsg(connID,0,
			            TRANVERS_CLIENT_REQ_CMD_S,
			            pathbuff,reqCmd_S.GetRealSize(),
			            TRUE,
						SENMSGDEFAULTTIMROUT,
			            AsynModel::POSTMSG_NO_SUBSCRIBE,
			            AsynModel::Protocol::TCP,
						NULL,
						m_localIP,
						m_localPort);
		if (asynRet== AsynModel::FATAL_ERROR)
		{
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer 发送TCP消息命令失败");

		}
        cout<<"stprc向打洞服务器发送TCP消息命令"<<endl;
	}
	else if(m_TranversClientType == 2)
	{
		//遍历m_TranversNodeList，分别封装TranversClientReqCmd_C ，序列化，发送给m_tranversServerAddr 暂时只发udp
		  std::list<Server>::iterator iter=m_TranversNodeList.begin();
		  while (iter!=m_TranversNodeList.end())
		  {
			  TranversClientReqCmd_C reqCmd_C;
			  reqCmd_C.dominId=m_param.m_dominId;
			  reqCmd_C.userID=m_param.m_userId;
			  memcpy(reqCmd_C.interestIP,(*iter).m_ip,16);
			  reqCmd_C.interestPort=(*iter).m_port;

			  char pathbuff[1024]={};

			  if(pathbuff==NULL)
			  {
				  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[RepeatSendCmdTimer]2 内存分配失败");		
				  return -1;
			  }

			  int ret=reqCmd_C.Seralize(pathbuff,reqCmd_C.GetRealSize());
			  if(ret<0)
			  {
				  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer2 序列化命令失败");
				  return -1;
			  }

			  AsynModel::ConnID connID;
			  connID.hosttype=AsynModel::ConnID::CONNID_OPEN_;
			  memcpy(connID.openhost.openUDPIP,m_param.m_tranversServerAddr.m_ip,16);
			  connID.openhost.openUdpPort=m_param.m_tranversServerAddr.m_port;
			  memcpy(connID.openhost.openTCPIP,m_param.m_tranversServerAddr.m_ip,16);
			  connID.openhost.openTcpPort=m_param.m_tranversServerAddr.m_port;

			  AsynModel::AsynResultRet asynRet = AsynModel::SUCCESS;

              if (m_on_type==TCP||m_on_type==TCP_AND_UDP)
              {
				  asynRet=PostMsg(connID,0,
					              TRANVERS_CLIENT_REQ_CMD_C,
								  pathbuff,
								  reqCmd_C.GetRealSize(),
								  TRUE,
								  SENMSGDEFAULTTIMROUT,
								  AsynModel::POSTMSG_NO_SUBSCRIBE,
								  AsynModel::Protocol::TCP,
								  NULL,
								  m_localIP,
								  m_localPort);
				  if (asynRet== AsynModel::FATAL_ERROR)
				  {
					  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer TCP发送消息命令失败");
				  }
				    cout<<"user向打洞服务器发送TCP消息命令"<<endl;
              } 
              else if(m_on_type==UDP||m_on_type==TCP_AND_UDP)
              {
				  asynRet=PostMsg(connID,0,
					  TRANVERS_CLIENT_REQ_CMD_C,
					  pathbuff,
					  reqCmd_C.GetRealSize(),
					  TRUE,
					  SENMSGDEFAULTTIMROUT,
					  AsynModel::POSTMSG_NO_SUBSCRIBE,
					  AsynModel::Protocol::UDP);
				  if (asynRet== AsynModel::FATAL_ERROR)
				  {
					  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer UDP发送消息命令失败");
				  }
				    cout<<"user向打洞服务器发送UDP消息命令"<<endl;
              }
			  
			  //遍历m_TranversNodemap，发送TRANVERS_CLIENT_TO_CLINET_REQ_CMD

			  IPPort m_temp;
			  m_temp.Port=(*iter).m_port;
			  memcpy(m_temp.Ip,(*iter).m_ip,16);
			  U32 sicrd=m_TranversNodemap[m_temp];

			  memcpy(connID.openhost.openUDPIP,(*iter).m_ip,16);
			  connID.openhost.openUdpPort=(*iter).m_port;
			  memcpy(connID.openhost.openTCPIP,(*iter).m_ip,16);
			  connID.openhost.openTcpPort=(*iter).m_port;

			  if (m_on_type==TCP||m_on_type==TCP_AND_UDP)
			  {
				  asynRet=PostMsg(connID,sicrd,
					  TRANVERS_CLIENT_TO_CLINET_REQ_CMD,
					  0,0,
					  TRUE,
					  SENMSGDEFAULTTIMROUT,
					  AsynModel::POSTMSG_NO_SUBSCRIBE,
					  AsynModel::Protocol::TCP,
					  NULL,
					  m_localIP,
					  m_localPort);
				  if (asynRet== AsynModel::FATAL_ERROR)
				  {
					  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer TCP2发送消息命令失败");
				  }
				    cout<<"user向业务服务器发送TCP消息命令"<<endl;
			  } 
			  else if(m_on_type==UDP||m_on_type==TCP_AND_UDP)
			  {
				  asynRet=PostMsg(connID,sicrd,
					  TRANVERS_CLIENT_TO_CLINET_REQ_CMD,
					  0,
					  0,
					  TRUE,
					  SENMSGDEFAULTTIMROUT,
					  AsynModel::POSTMSG_NO_SUBSCRIBE,
					  AsynModel::Protocol::UDP);
				  if (asynRet== AsynModel::FATAL_ERROR)
				  {
					  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer UDP2发送消息命令失败");
				  }
				      cout<<"user向业务服务器发送UDP消息命令"<<endl;
			  }
			  iter++;
		  }
	}
	else
	{

	}
	SetTimer(1,SEND_TIME_SPAN);

	return 0;
}

int  TranversClientSession::onTranversClientSRespCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	cout<<"收到打洞服务器回复m_OuterAddr"<<endl;
	TranversClientRespCmd_S  respCmd;
	int ret;
	//respCmd反序列化，赋值给m_OuterAddr，等待查询
	ret=respCmd.UnSeralize(buffer,length);
	if(ret <= 0)
	{
		unsigned long remoteIP;
		unsigned short remotePort;
		char ip[16] = {0};
		sender->GetRemoteIPPort(remoteIP,remotePort);
		GMConvertIP_n2a(remoteIP,ip);
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientSRespCmdArrived 收到请求，反序列化命令失败,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
		
		return -1;
	}
	else
	{
		memcpy(m_OuterAddr.m_ip,respCmd.OuterIP,sizeof(respCmd.OuterIP));
		m_OuterAddr.m_port=respCmd.OuterPort;
		
	}
	return 0;
}
int  TranversClientSession::onTranversClientCRespCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	cout<<"User收到打洞服务器的回复"<<endl;
	TranversClientRespCmd_C  respCmd;
	int ret;
	U32 m_sicid;

	//respCmd反序列化，
	ret=respCmd.UnSeralize(buffer,length);
	if(ret <= 0)
	{
		unsigned long remoteIP;
		unsigned short remotePort;
		char ip[16] = {0};
		sender->GetRemoteIPPort(remoteIP,remotePort);
		GMConvertIP_n2a(remoteIP,ip);
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientCRespCmdArrived 收到请求，反序列化命令失败,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
	   
		return -1;
	}
	else
	{		    
		m_sicid=respCmd.SessionId;
		IPPort m_temp;
		m_temp.Port=respCmd.interestPort;
		memcpy(m_temp.Ip,respCmd.interestIP,16);
		U32 sicrd=m_TranversNodemap[m_temp];
		//判断SessionId与map中的是否改变，改变则改变map的值
         if (m_sicid==m_TranversNodemap[m_temp])
         {
	     
         } 
         else
         {
			 //发送TRANVERS_CLIENT_TO_CLINET_REQ_CMD，改变map
			 m_TranversNodemap[m_temp]=m_sicid;
			 AsynModel::ConnID connID;
			 connID.hosttype=AsynModel::ConnID::CONNID_OPEN_;
			 memcpy(connID.openhost.openUDPIP,respCmd.interestIP,16);
			 connID.openhost.openUdpPort=respCmd.interestPort;
			 memcpy(connID.openhost.openTCPIP,respCmd.interestIP,16);
			 connID.openhost.openTcpPort=respCmd.interestPort;

			 AsynModel::AsynResultRet asynRet = AsynModel::SUCCESS;

			 if (m_on_type==TCP||m_on_type==TCP_AND_UDP)
			 {
				 asynRet=PostMsg(connID,m_sicid,
					 TRANVERS_CLIENT_TO_CLINET_REQ_CMD,
					 0,
					 0,
					 TRUE,
					 SENMSGDEFAULTTIMROUT,
					 AsynModel::POSTMSG_NO_SUBSCRIBE,
					 AsynModel::Protocol::TCP,
					 NULL,
					 m_localIP,
					 m_localPort);
				 if (asynRet== AsynModel::FATAL_ERROR)
				 {
					 Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientCRespCmdArrived UDP发送消息命令失败");
				 }
			 } 
			 else if(m_on_type==UDP||m_on_type==TCP_AND_UDP)
			 {
				 asynRet=PostMsg(connID,m_sicid,
					 TRANVERS_CLIENT_TO_CLINET_REQ_CMD,
					 0,
					 0,
					 TRUE,
					 SENMSGDEFAULTTIMROUT,
					 AsynModel::POSTMSG_NO_SUBSCRIBE,
					 AsynModel::Protocol::UDP);
				 if (asynRet== AsynModel::FATAL_ERROR)
				 {
					 Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientCRespCmdArrived TCP发送消息命令失败");
				 }
			 }
         }
	}	
	return 0;
}

int   TranversClientSession::onInnerQueryOuterAddrReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{

	//返回 m_OuterAddr 起始地址，长度为sizeof取值，注意m_OuterAddr构造函数值
	if (m_OuterAddr.m_port==0&&!strcmp(m_OuterAddr.m_ip,"0"))
	{
		  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onInnerQueryOuterAddrReqCmdArrived 失败");
		return -1;
	}
	
	AsynModel::AsynResultRet asynRet = AsynModel::SUCCESS;
	asynRet=PostMsg((AsynModel::SptrISender)sender,srcid,INNER_QUERY_LOCAL_OUTER_ADDR_RESP_CMD,false,(char*)&m_OuterAddr,sizeof(m_OuterAddr));
	if (asynRet== AsynModel::FATAL_ERROR)
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onInnerQueryOuterAddrReqCmdArrived 发送消息命令失败");
	
		return -1;
	}

	return 0;
}

int  TranversClientSession::onTranversServerNotifyClientCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	cout<<"收到打洞服务器的转发"<<endl;
	TranversServerNotifyCmd  cmd;
	int ret;
	//cmd反序列化
	ret=cmd.UnSeralize(buffer,length);
	U32 m_sicid=cmd.SessionId;
	unsigned long remoteIP;
	unsigned short remotePort;
	char ip[16] = {0};
	sender->GetRemoteIPPort(remoteIP,remotePort);
	GMConvertIP_n2a(remoteIP,ip);
	if(ret <= 0)
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversServerNotifyClientCmdArrived 收到请求，反序列化命令失败,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
		return -1;
	}
	else
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversServerNotifyClientCmdArrived 收到请求，反序列化命令成功,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
	}

	int senderType = sender->GetType();

	if(senderType == AsynModel::ISender::ASYNUDPSENDER)
	{
         m_on_type=UDP;
	}

	if(senderType == AsynModel::ISender::ASYNTCPSENDER)
	{
          m_on_type=TCP;
	}

	//发送TRANVERS_CLIENT_TO_CLINET_REQ_CMD 给 解析出来的interestIP和port

	AsynModel::ConnID connID;
	connID.hosttype=AsynModel::ConnID::CONNID_OPEN_;
	memcpy(connID.openhost.openUDPIP,cmd.interestIP,16);
	connID.openhost.openUdpPort=cmd.interestPort;
	memcpy(connID.openhost.openTCPIP,cmd.interestIP,16);
	connID.openhost.openTcpPort=cmd.interestPort;

	AsynModel::AsynResultRet asynRet = AsynModel::SUCCESS;

	if (m_on_type==TCP||m_on_type==TCP_AND_UDP)
	{
		asynRet=PostMsg(connID,
			m_sicid,
			TRANVERS_CLIENT_TO_CLINET_REQ_CMD,
			0,
			0,
			TRUE,
			SENMSGDEFAULTTIMROUT,
			AsynModel::POSTMSG_NO_SUBSCRIBE,
			AsynModel::Protocol::TCP,
			NULL,
			m_localIP,
			m_localPort);
		if (asynRet== AsynModel::FATAL_ERROR)
		{
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversServerNotifyClientCmdArrived TCP发送消息命令失败");
			return -1;
		}
	} 
	else if(m_on_type==UDP||m_on_type==TCP_AND_UDP)
	{
		asynRet=PostMsg(connID,
			m_sicid,
			TRANVERS_CLIENT_TO_CLINET_REQ_CMD,
			0,
			0,
			TRUE,
			SENMSGDEFAULTTIMROUT,
			AsynModel::POSTMSG_NO_SUBSCRIBE,
			AsynModel::Protocol::UDP);
		if (asynRet== AsynModel::FATAL_ERROR)
		{
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversServerNotifyClientCmdArrived UDP发送消息命令失败");
			return -1;
		}
	}
	return 0;
}

int  TranversClientSession::onTranversClientToClientRespCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	int senderType = sender->GetType();

	if(senderType == AsynModel::ISender::ASYNUDPSENDER)
	{
		cout<<"UDP";
	}

	if(senderType == AsynModel::ISender::ASYNTCPSENDER)
	{
            cout<<"TCP";
	}
	cout<<"==============================接收到对面客户端回复的信息================="<<endl;
	/////////////////////////////////////////////////////////////////////////////////////////////
	TranversClientToClientRespCmd  respCmd;
	//respCmd反序列化，打印日志
	int ret;
	ret=respCmd.UnSeralize(buffer,length);

	unsigned long remoteIP;
	unsigned short remotePort;
	char ip[16] = {0};
	sender->GetRemoteIPPort(remoteIP,remotePort);
	GMConvertIP_n2a(remoteIP,ip);
	
	if(ret <= 0)
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientToClientRespCmdArrived 收到请求，反序列化命令失败,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
		return -1;
	}
	else
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientToClientRespCmdArrived 收到请求，反序列化命令成功,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
		
	}
	return 0;
}

int  TranversClientSession::onTranversClientToClientReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	cout<<"收到对面的请求"<<endl;
	TranversClientToClientRespCmd  respCmd;
	//封装respCmd，序列化发送；如果m_TranversClientType为2，把dominId和UserId填上；如果m_TranversClientType为1，把dominId和serverType填上
     if (m_TranversClientType==2)
     {
		 respCmd.dominId=m_param.m_dominId;
		 respCmd.nodeId=m_param.m_userId;
     } 
     else if(m_TranversClientType==1)
     {
		 respCmd.dominId=m_param.m_dominId;
		 respCmd.nodeId=m_param.m_serverType;
     }
	 else
	 {
		 Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[TranversClientSession::onTranversClientToClientReqCmdArrived] m_TranversClientType错误");
		 return -1;
	 }

	 char pathbuff[1024]={0};
	 if(pathbuff==NULL)
	 {
	    	Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[onTranversClientToClientReqCmdArrived] 发送命令时，内存分配失败");
	 	return -1;
	 }

	 int ret=respCmd.Seralize(pathbuff,respCmd.GetRealSize());
	 if(ret<0)
	 {
	 	Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientToClientReqCmdArrived 序列化命令失败");
	 	return -1;
	 }

	 AsynModel::AsynResultRet asynRet = AsynModel::SUCCESS;
	 asynRet=PostMsg((AsynModel::SptrISender)sender,srcid,TRANVERS_CLIENT_TO_CLINET_RESP_CMD,pathbuff,respCmd.GetRealSize(),true);
	 if (asynRet== AsynModel::FATAL_ERROR)
	 {
		 Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientToClientReqCmdArrived 发送消息命令失败");
		 return -1;
	 }
	 cout<<"原路返回"<<endl;
	return 0;
}

int  TranversClientSession::onInnerAddNewTanversNodeReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{                                                               
	Server*  pServer =(Server*) buffer;

	m_TranversNodeList.push_back(*pServer);
	
	//pServer->m_ip
	//pServer->m_port
	//填入m_TranversNodeList
	return 0;
}
