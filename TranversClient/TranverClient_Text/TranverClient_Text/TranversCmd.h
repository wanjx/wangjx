#pragma once
//终端已经知道公网IP，现在处于打洞阶段（点对点打洞）
#define  TRANVERS_CLIENT_REQ_CMD_S            4001//业务服务器作为打洞客户端的请求命令
#define  TRANVERS_CLIENT_RESP_CMD_S           4002

#define  TRANVERS_CLIENT_REQ_CMD_C            4003//业务终端作为打洞客户端的请求命令
#define  TRANVERS_CLIENT_RESP_CMD_C           4004

#define  TRANVERS_SERVER_NOTIFY_CLINET_CMD    4005 //打洞服务器 转发 业务终端发来的消息

#define  TRANVERS_CLIENT_TO_CLINET_REQ_CMD    4007
#define  TRANVERS_CLIENT_TO_CLINET_RESP_CMD   4008


struct  TranversClientReqCmd_S
{
	int              dominId;
	std::string    serverType;
	int Seralize(char* buff, size_t len)
	{
		if(buff == NULL || len < GetRealSize())
			return -1;

		int serLen = 0;
		memcpy(buff+serLen, &dominId, sizeof(dominId));
		serLen += sizeof(dominId);

		int serverTypeLen = (int)serverType.length();
		memcpy(buff+serLen, &serverTypeLen, sizeof(serverTypeLen));
		serLen += sizeof(serverTypeLen);

		memcpy(buff+serLen, serverType.c_str(), serverTypeLen);
		serLen += serverTypeLen;

		return serLen;
	}
	int UnSeralize(const char * buff, size_t len)
	{
		if(buff == NULL || len <  sizeof(dominId)  )
			return -1;
		int unSerLen = 0;
		memcpy(&dominId, buff+unSerLen, sizeof(dominId));
		unSerLen += sizeof(dominId);

		int serverTypeLen = 0;
		memcpy(& serverTypeLen, buff+unSerLen, sizeof(serverTypeLen));
		unSerLen += sizeof(serverTypeLen);

		serverType.assign(buff+unSerLen,serverTypeLen);
		unSerLen += serverTypeLen;

		return unSerLen;
	}
	unsigned int GetRealSize()
	{
		return sizeof(int) + sizeof(int) + serverType.length();
	}
};

struct  TranversClientRespCmd_S
{
	char            OuterIP[16];
	unsigned short     OuterPort;
	int Seralize(char* buff, size_t len)
	{
		if(buff == NULL || len < GetRealSize())
			return -1;

		int serLen = 0;

		memcpy(buff + serLen, OuterIP, sizeof(OuterIP));
		serLen += sizeof(OuterIP);

		memcpy(buff + serLen, &OuterPort, sizeof(OuterPort));
		serLen += sizeof(OuterPort);

		return serLen;
	}
	int UnSeralize(const char * buff, size_t len)
	{
		if(NULL == buff || len < sizeof(OuterIP) + sizeof(OuterPort))
			return -1;

		int unSerLen = 0;
		memcpy(OuterIP, buff + unSerLen, sizeof(OuterIP));
		unSerLen += sizeof(OuterIP);

		memcpy(&OuterPort, buff + unSerLen, sizeof(OuterPort));
		unSerLen += sizeof(OuterPort);

		return unSerLen;
	}
	unsigned int GetRealSize()
	{
		return sizeof(OuterPort) + sizeof(OuterIP);
	}
};

struct  TranversClientReqCmd_C
{
	char            interestIP[16];
	unsigned short   interestPort;
	int             SessionId;
	std::string      userID;           //逻辑上不需要，但打日志时会用到
	int             dominId;
	
	int Seralize(char* buff, size_t len)
	{
		if(buff == NULL || len < GetRealSize())
			return -1;

		int serLen = 0;
		memcpy(buff+serLen, interestIP, sizeof(interestIP));
		serLen += sizeof(interestIP);

		memcpy(buff+serLen, &interestPort, sizeof(interestPort));
		serLen += sizeof(interestPort);

		memcpy(buff+serLen, &SessionId, sizeof(SessionId));
		serLen += sizeof(SessionId);

		int userIDLen = (int)userID.length();
		memcpy(buff+serLen, &userIDLen, sizeof(userIDLen));
		serLen += sizeof(userIDLen);

		memcpy(buff+serLen, userID.c_str(), userIDLen);
		serLen += userIDLen;

		memcpy(buff+serLen, &dominId, sizeof(dominId));
		serLen += sizeof(dominId);

		return serLen;
	}
	int UnSeralize(const char * buff, size_t len)
	{
		if(buff == NULL || len <  sizeof(interestIP) + sizeof(interestPort)+sizeof(dominId) + sizeof(SessionId) )
			return -1;

		int unSerLen = 0;
		memcpy(interestIP, buff+unSerLen, sizeof(interestIP));
		unSerLen += sizeof(interestIP);

		memcpy(&interestPort, buff+unSerLen, sizeof(interestPort));
		unSerLen += sizeof(interestPort);

		memcpy(&SessionId, buff+unSerLen, sizeof(SessionId));
		unSerLen += sizeof(SessionId);

		int userIDLen = 0;
		memcpy(& userIDLen, buff+unSerLen, sizeof(userIDLen));
		unSerLen += sizeof(userIDLen);

		userID.assign(buff+unSerLen,userIDLen);
		unSerLen += userIDLen;

		memcpy(&dominId, buff+unSerLen, sizeof(dominId));
		unSerLen += sizeof(dominId);

		return unSerLen;
	}
	unsigned int GetRealSize()
	{
		return (unsigned int)(sizeof(interestIP) + sizeof(interestPort) + sizeof(SessionId) + sizeof(int) + userID.length()+sizeof(dominId));
	}
};

struct TranversClientRespCmd_C
{
	char            interestIP[16];
	unsigned short   interestPort;
	int             SessionId;

	int Seralize(char* buff, size_t len)
	{
		if(buff == NULL || len < GetRealSize())
			return -1;

		int serLen = 0;
		memcpy(buff+serLen, interestIP, sizeof(interestIP));
		serLen += sizeof(interestIP);

		memcpy(buff+serLen, &interestPort, sizeof(interestPort));
		serLen += sizeof(interestPort);

		memcpy(buff+serLen, &SessionId, sizeof(SessionId));
		serLen += sizeof(SessionId);

		return serLen;
	}
	int UnSeralize(const char * buff, size_t len)
	{
		if(buff == NULL || len <  sizeof(interestIP) + sizeof(interestPort)+sizeof(SessionId))
			return -1;

		int unSerLen = 0;
		memcpy(interestIP, buff+unSerLen, sizeof(interestIP));
		unSerLen += sizeof(interestIP);

		memcpy(&interestPort, buff+unSerLen, sizeof(interestPort));
		unSerLen += sizeof(interestPort);

		memcpy(&SessionId, buff+unSerLen, sizeof(SessionId));
		unSerLen += sizeof(SessionId);

		return unSerLen;
	}
	unsigned int GetRealSize()
	{
		return (unsigned int)(sizeof(interestIP) + sizeof(interestPort) + sizeof(SessionId));
	}
};

typedef   TranversClientReqCmd_C  TranversServerNotifyCmd;

struct  TranversClientToClientRespCmd
{
	int              dominId;
	std::string      nodeId;

	int Seralize(char* buff, size_t len)
	{
		if(buff == NULL || len < GetRealSize())
			return -1;
		int serLen = 0;
		memcpy(buff+serLen, &dominId, sizeof(dominId));
		serLen += sizeof(dominId);

		int nodeIdLen = (int)nodeId.length();
		memcpy(buff+serLen, &nodeIdLen, sizeof(nodeIdLen));
		serLen += sizeof(nodeIdLen);

		memcpy(buff+serLen, nodeId.c_str(), nodeIdLen);
		serLen += nodeIdLen;

		return serLen;
	}
	int UnSeralize(const char * buff, size_t len)
	{
		if(buff == NULL || len <  sizeof(dominId)  )
			return -1;
		int unSerLen = 0;
		memcpy(&dominId, buff+unSerLen, sizeof(dominId));
		unSerLen += sizeof(dominId);

		int nodeIdLen = 0;
		memcpy(& nodeIdLen, buff+unSerLen, sizeof(nodeIdLen));
		unSerLen += sizeof(nodeIdLen);

		nodeId.assign(buff+unSerLen,nodeIdLen);
		unSerLen += nodeIdLen;

		return unSerLen;
	}
	unsigned int GetRealSize()
	{
		return (unsigned int)(sizeof(dominId) + sizeof(int) + nodeId.length());
	}
};

struct IPPort
{
	char             Ip[16];
	unsigned short    Port;
	bool operator < (const IPPort &it) const
	{
		if(strcmp(Ip, it.Ip) > 0)
		{
			return false;
		}
		else if(strcmp(Ip, it.Ip) < 0)
		{
			return true;
		}
		else
		{
			return Port < it.Port;
		}
	}  

};

typedef map<IPPort, int> IPPORT_SID;