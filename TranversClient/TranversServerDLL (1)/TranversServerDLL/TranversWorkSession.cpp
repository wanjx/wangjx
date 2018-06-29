#include "stdafx.h"
#include "TranversWorkSession.h"

int TranversWorkSession::OnStart()
{
	
	return 0;
}

void TranversWorkSession::OnTerminate(U32 type)
{
}

//���Ļ�ִ
int TranversWorkSession::OnPostMsgRecept(AsynModel::ReceptType receptType,
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
	bool isReliable)
{
	printf("receptType:%d,receptResult:%d,msgId:%d---client_session::OnPostMsgRecept",receptType,receptResult,msgId);
	Log::writeWarning(TRANVERSSERVER_LOG,LOG_TYPE_ID,"receptType:%d,receptResult:%d,msgId:%d---client_session::OnPostMsgRecept",receptType,receptResult,msgId);

	return 0;
}

int  TranversWorkSession::onTranverrsClientSReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	if(NULL == buffer || length <= 0)
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷��������յ��򶴿ͻ��ˣ�ҵ��������������������bufferΪ�ջ�length<=0 --TranversWorkSession::onTranverrsClientSReqCmdArrived");
		return 0;
	}
	
	//reqCmd_S �����л�
	TranversClientReqCmd_S  reqCmd_S;
	reqCmd_S.UnSeralize(buffer, length);

	//��sender������ip��port����װTranversClientRespCmd_S��ԭ·����
	unsigned long  remoteIp;	
	unsigned short remotePort;
	sender->GetRemoteIPPort(remoteIp, remotePort);
	//��IP��unsigned long תΪ ���ʮ���ƴ�
	char IP[16] = {0};
	in_addr IPTemp;
	IPTemp.S_un.S_addr = htonl(remoteIp);
	_snprintf(IP,16,"%s",inet_ntoa(IPTemp));
	//��IPport���л�
	TranversClientRespCmd_S respCmd_S;
	int len = respCmd_S.GetRealSize();
	char *buf = new(nothrow) char[len];
	if(NULL == buf)
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�ظ��򶴿ͻ���ʱ����������ռ�ʧ�� --TranversWorkSession::onTranverrsClientSReqCmdArrived");
		return -1;
	}
	strcpy(respCmd_S.OuterIP, IP);
	respCmd_S.OuterPort = remotePort;
	respCmd_S.Seralize(buf, len);

	AsynModel::AsynResultRet ret = PostMsg((AsynModel::SptrISender&)sender, 
										srcid, TRANVERS_CLIENT_RESP_CMD_S, 
										(char*)buf, len, true);
	if(ret != AsynModel::SUCCESS)
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷������ظ��򶴿ͻ��ˣ�ҵ���������ʧ�� --TranversWorkSession::onTranverrsClientSReqCmdArrived");
		delete []buf;
		return -1;
	}
	else
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷������ظ��򶴿ͻ��ˣ�ҵ����������ɹ� --TranversWorkSession::onTranverrsClientSReqCmdArrived");
	}
	//���serverType��stpRC����Ҫ�ص����ⲿģ��
	std::string temp_type;
	temp_type.assign(reqCmd_S.serverType.c_str());
	transform(temp_type.begin(), temp_type.end(), temp_type.begin(), ::tolower);//��תΪСд
	if(0 == strcmp(temp_type.c_str(), "stprc"))//�ٽ��бȽ�
	{
		if(NULL != g_ServerInfoCallBack)
		{
			ServerInfo ser_info;
			ser_info.m_servertype.assign(reqCmd_S.serverType.c_str());
			ser_info.m_Outport = remotePort;
			strcpy(ser_info.m_Outip, IP);
			g_ServerInfoCallBack(&ser_info);
			Log::writeWarning(TRANVERSSERVER_LOG, LOG_TYPE_ID, "");
		}
	}
	delete []buf;
	SetTimer(1, 0);
	return 0;
}
int  TranversWorkSession::onTranverrsClientCReqCmdArrived(const char* buffer,int length ,uint32 srcid, const AsynModel::SptrISender& sender)
{
	if(NULL == buffer || length <= 0)
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷��������յ��򶴿ͻ��ˣ�ҵ��������������������bufferΪ�ջ�length<=0 --TranversWorkSession::onTranverrsClientCReqCmdArrived");
		return 0;
	}
	
	//reqCmd_C �����л�
	TranversClientReqCmd_C reqCmd_C;
	reqCmd_C.UnSeralize(buffer, length);
	//��sender������ip��port��
	unsigned long  remoteIp;
	unsigned short remotePort;
	sender->GetRemoteIPPort(remoteIp, remotePort);
	//��IP��unsigned long תΪ ���ʮ���ƴ�
	char IP[16] = {0};
	in_addr IPTemp;
	IPTemp.S_un.S_addr = htonl(remoteIp);
	_snprintf(IP,16,"%s",inet_ntoa(IPTemp));
	//��װTranversServerNotifyCmd��
	TranversServerNotifyCmd notifycmd;
	int len = notifycmd.GetRealSize();
	char *buf = new(nothrow) char[len];
	if(NULL == buf)
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�ظ��򶴿ͻ���ʱ����������ռ�ʧ�� --TranversWorkSession::onTranverrsClientCReqCmdArrived");
		return -1;
	}

	////��¼��Ŀ��IPPort
	//strcpy(m_destIp, reqCmd_C.interestIP);
	//m_destPort = reqCmd_C.interestPort;
	////��¼�·��͵�����
	//strcpy(m_ServerNotifyCmd.interestIP, IP);
	//m_ServerNotifyCmd.interestPort = remotePort;
	//m_ServerNotifyCmd.userID = reqCmd_C.userID;
	//m_ServerNotifyCmd.dominId = reqCmd_C.dominId;
	
	strcpy(notifycmd.interestIP, IP);
	notifycmd.interestPort = remotePort;
	notifycmd.userID = reqCmd_C.userID;
	notifycmd.dominId = reqCmd_C.dominId;
	notifycmd.Seralize(buf, len);
	//����Ŀ���ַ��interestIP��interestPort
/*
	char interestIP[16] = {0};
	unsigned short interestPort = 0;
	strcpy(interestIP, reqCmd_C.interestIP);
	interestPort = reqCmd_C.interestPort;
	sender->;*/
	
	AsynModel::ConnID connid;
	connid.hosttype = AsynModel::ConnID::CONNID_OPEN_;
	memcpy(connid.openhost.openTCPIP, reqCmd_C.interestIP, 16);
	connid.openhost.openTcpPort = reqCmd_C.interestPort;

	AsynModel::AsynResultRet ret = PostMsg((AsynModel::ConnID&)connid, 
										srcid, 
										TRANVERS_SERVER_NOTIFY_CLINET_CMD, 
										buf, len, true,
										SENMSGDEFAULTTIMROUT,
										AsynModel::POSTMSG_NO_SUBSCRIBE,AsynModel::UDP);
	if (ret != AsynModel::SUCCESS)
	{
		Log::writeError(TRANVERSSERVER_LOG,LOG_TYPE_ID,"");
		delete []buf;
		return -1;
	}
	else
	{
		Log::writeError(TRANVERSSERVER_LOG,LOG_TYPE_ID,"");
	}

/*
	AsynModel::AsynResultRet ret = PostMsg((AsynModel::SptrISender&)sender, srcid, TRANVERS_SERVER_NOTIFY_CLINET_CMD, (char*)buf, len, true);
	if(ret != AsynModel::SUCCESS)
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷�����ת���򶴿ͻ��ˣ�ҵ���նˣ��Ĺ���IPportʧ�� --TranversWorkSession::onTranverrsClientSReqCmdArrived");
		delete []buf;
		return -1;
	}
	else
	{
		Log::writeError(TRANVERSSERVER_LOG, LOG_TYPE_ID, "�򶴷�����ת���򶴿ͻ��ˣ�ҵ���նˣ��Ĺ���IPport�ɹ� --TranversWorkSession::onTranverrsClientSReqCmdArrived");
	}
*/
	delete []buf;
	SetTimer(1, 0);
	return 0;
}
