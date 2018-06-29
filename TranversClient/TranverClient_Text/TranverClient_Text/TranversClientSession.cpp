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
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[TranversClientSession]����2�������");			
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
		//��װTranversClientReqCmd_S �����л������͸�m_tranversServerAddr ��ʱֻ��udp
		TranversClientReqCmd_S reqCmd_S;
		reqCmd_S.dominId=m_param.m_dominId;
		reqCmd_S.serverType=m_param.m_serverType;

		char pathbuff[1024]={};
		
		if(pathbuff==NULL)
		{
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[RepeatSendCmdTimer]1 �ڴ����ʧ��");		
			return -1;
		}
		
		int ret=reqCmd_S.Seralize(pathbuff,reqCmd_S.GetRealSize());
		if(ret<0)
		{
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer1 ���л�����ʧ��");
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
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer ����UDP��Ϣ����ʧ��");
			
		}
		cout<<"stprc��򶴷���������UDP��Ϣ����"<<endl;
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
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer ����TCP��Ϣ����ʧ��");

		}
        cout<<"stprc��򶴷���������TCP��Ϣ����"<<endl;
	}
	else if(m_TranversClientType == 2)
	{
		//����m_TranversNodeList���ֱ��װTranversClientReqCmd_C �����л������͸�m_tranversServerAddr ��ʱֻ��udp
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
				  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[RepeatSendCmdTimer]2 �ڴ����ʧ��");		
				  return -1;
			  }

			  int ret=reqCmd_C.Seralize(pathbuff,reqCmd_C.GetRealSize());
			  if(ret<0)
			  {
				  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer2 ���л�����ʧ��");
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
					  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer TCP������Ϣ����ʧ��");
				  }
				    cout<<"user��򶴷���������TCP��Ϣ����"<<endl;
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
					  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer UDP������Ϣ����ʧ��");
				  }
				    cout<<"user��򶴷���������UDP��Ϣ����"<<endl;
              }
			  
			  //����m_TranversNodemap������TRANVERS_CLIENT_TO_CLINET_REQ_CMD

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
					  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer TCP2������Ϣ����ʧ��");
				  }
				    cout<<"user��ҵ�����������TCP��Ϣ����"<<endl;
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
					  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::RepeatSendCmdTimer UDP2������Ϣ����ʧ��");
				  }
				      cout<<"user��ҵ�����������UDP��Ϣ����"<<endl;
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
	cout<<"�յ��򶴷������ظ�m_OuterAddr"<<endl;
	TranversClientRespCmd_S  respCmd;
	int ret;
	//respCmd�����л�����ֵ��m_OuterAddr���ȴ���ѯ
	ret=respCmd.UnSeralize(buffer,length);
	if(ret <= 0)
	{
		unsigned long remoteIP;
		unsigned short remotePort;
		char ip[16] = {0};
		sender->GetRemoteIPPort(remoteIP,remotePort);
		GMConvertIP_n2a(remoteIP,ip);
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientSRespCmdArrived �յ����󣬷����л�����ʧ��,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
		
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
	cout<<"User�յ��򶴷������Ļظ�"<<endl;
	TranversClientRespCmd_C  respCmd;
	int ret;
	U32 m_sicid;

	//respCmd�����л���
	ret=respCmd.UnSeralize(buffer,length);
	if(ret <= 0)
	{
		unsigned long remoteIP;
		unsigned short remotePort;
		char ip[16] = {0};
		sender->GetRemoteIPPort(remoteIP,remotePort);
		GMConvertIP_n2a(remoteIP,ip);
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientCRespCmdArrived �յ����󣬷����л�����ʧ��,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
	   
		return -1;
	}
	else
	{		    
		m_sicid=respCmd.SessionId;
		IPPort m_temp;
		m_temp.Port=respCmd.interestPort;
		memcpy(m_temp.Ip,respCmd.interestIP,16);
		U32 sicrd=m_TranversNodemap[m_temp];
		//�ж�SessionId��map�е��Ƿ�ı䣬�ı���ı�map��ֵ
         if (m_sicid==m_TranversNodemap[m_temp])
         {
	     
         } 
         else
         {
			 //����TRANVERS_CLIENT_TO_CLINET_REQ_CMD���ı�map
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
					 Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientCRespCmdArrived UDP������Ϣ����ʧ��");
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
					 Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientCRespCmdArrived TCP������Ϣ����ʧ��");
				 }
			 }
         }
	}	
	return 0;
}

int   TranversClientSession::onInnerQueryOuterAddrReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{

	//���� m_OuterAddr ��ʼ��ַ������Ϊsizeofȡֵ��ע��m_OuterAddr���캯��ֵ
	if (m_OuterAddr.m_port==0&&!strcmp(m_OuterAddr.m_ip,"0"))
	{
		  Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onInnerQueryOuterAddrReqCmdArrived ʧ��");
		return -1;
	}
	
	AsynModel::AsynResultRet asynRet = AsynModel::SUCCESS;
	asynRet=PostMsg((AsynModel::SptrISender)sender,srcid,INNER_QUERY_LOCAL_OUTER_ADDR_RESP_CMD,false,(char*)&m_OuterAddr,sizeof(m_OuterAddr));
	if (asynRet== AsynModel::FATAL_ERROR)
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onInnerQueryOuterAddrReqCmdArrived ������Ϣ����ʧ��");
	
		return -1;
	}

	return 0;
}

int  TranversClientSession::onTranversServerNotifyClientCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	cout<<"�յ��򶴷�������ת��"<<endl;
	TranversServerNotifyCmd  cmd;
	int ret;
	//cmd�����л�
	ret=cmd.UnSeralize(buffer,length);
	U32 m_sicid=cmd.SessionId;
	unsigned long remoteIP;
	unsigned short remotePort;
	char ip[16] = {0};
	sender->GetRemoteIPPort(remoteIP,remotePort);
	GMConvertIP_n2a(remoteIP,ip);
	if(ret <= 0)
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversServerNotifyClientCmdArrived �յ����󣬷����л�����ʧ��,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
		return -1;
	}
	else
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversServerNotifyClientCmdArrived �յ����󣬷����л�����ɹ�,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
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

	//����TRANVERS_CLIENT_TO_CLINET_REQ_CMD �� ����������interestIP��port

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
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversServerNotifyClientCmdArrived TCP������Ϣ����ʧ��");
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
			Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversServerNotifyClientCmdArrived UDP������Ϣ����ʧ��");
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
	cout<<"==============================���յ�����ͻ��˻ظ�����Ϣ================="<<endl;
	/////////////////////////////////////////////////////////////////////////////////////////////
	TranversClientToClientRespCmd  respCmd;
	//respCmd�����л�����ӡ��־
	int ret;
	ret=respCmd.UnSeralize(buffer,length);

	unsigned long remoteIP;
	unsigned short remotePort;
	char ip[16] = {0};
	sender->GetRemoteIPPort(remoteIP,remotePort);
	GMConvertIP_n2a(remoteIP,ip);
	
	if(ret <= 0)
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientToClientRespCmdArrived �յ����󣬷����л�����ʧ��,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
		return -1;
	}
	else
	{
		Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientToClientRespCmdArrived �յ����󣬷����л�����ɹ�,ret:%d,srcIP:%s,srcPort:%d",ret,ip,remotePort);
		
	}
	return 0;
}

int  TranversClientSession::onTranversClientToClientReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	cout<<"�յ����������"<<endl;
	TranversClientToClientRespCmd  respCmd;
	//��װrespCmd�����л����ͣ����m_TranversClientTypeΪ2����dominId��UserId���ϣ����m_TranversClientTypeΪ1����dominId��serverType����
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
		 Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[TranversClientSession::onTranversClientToClientReqCmdArrived] m_TranversClientType����");
		 return -1;
	 }

	 char pathbuff[1024]={0};
	 if(pathbuff==NULL)
	 {
	    	Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"[onTranversClientToClientReqCmdArrived] ��������ʱ���ڴ����ʧ��");
	 	return -1;
	 }

	 int ret=respCmd.Seralize(pathbuff,respCmd.GetRealSize());
	 if(ret<0)
	 {
	 	Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientToClientReqCmdArrived ���л�����ʧ��");
	 	return -1;
	 }

	 AsynModel::AsynResultRet asynRet = AsynModel::SUCCESS;
	 asynRet=PostMsg((AsynModel::SptrISender)sender,srcid,TRANVERS_CLIENT_TO_CLINET_RESP_CMD,pathbuff,respCmd.GetRealSize(),true);
	 if (asynRet== AsynModel::FATAL_ERROR)
	 {
		 Log::writeError(TRANVERS_CLIENT_LOG,LOG_TYPE_ID,"TranversClientSession::onTranversClientToClientReqCmdArrived ������Ϣ����ʧ��");
		 return -1;
	 }
	 cout<<"ԭ·����"<<endl;
	return 0;
}

int  TranversClientSession::onInnerAddNewTanversNodeReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{                                                               
	Server*  pServer =(Server*) buffer;

	m_TranversNodeList.push_back(*pServer);
	
	//pServer->m_ip
	//pServer->m_port
	//����m_TranversNodeList
	return 0;
}
